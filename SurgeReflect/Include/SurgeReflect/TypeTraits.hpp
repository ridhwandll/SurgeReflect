// Copyright (c) - SurgeTechnologies - All rights reserved
#pragma once
#include <string>

#if defined(__clang__) || defined(__GNUC__) || defined(_MSC_VER)
#define COMPILETIME_TYPENAME_WRAPPER_SUPPORTED 1
#else
#define COMPILETIME_TYPENAME_WRAPPER_SUPPORTED 0
#endif

#include <cstdint>

#include <string>
#include <string_view>
#include <type_traits>

#if !COMPILETIME_TYPENAME_WRAPPER_SUPPORTED
#include <typeinfo>
#endif

namespace SurgeReflect
{
    namespace Detail
    {
        template <class T>
        constexpr std::string_view GetTypeNameWrapper()
        {
#if COMPILETIME_TYPENAME_WRAPPER_SUPPORTED
#if defined(__clang__) || defined(__GNUC__)
            return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
            return __FUNCSIG__;
#endif
#else
            return typeid(T).name();
#endif
        }

    } // namespace Detail

    namespace TypeTraits
    {
        template <class T>
        inline constexpr std::string_view GetTypeName();

        template <>
        inline constexpr std::string_view GetTypeName<void>()
        {
            return "void";
        }

        template <>
        inline constexpr std::string_view GetTypeName<int>()
        {
            return "int";
        }

        template <>
        inline constexpr std::string_view GetTypeName<short>()
        {
            return "short";
        }

        template <>
        inline constexpr std::string_view GetTypeName<double>()
        {
            return "double";
        }

        template <>
        inline constexpr std::string_view GetTypeName<long double>()
        {
            return "long double";
        }

        template <>
        inline constexpr std::string_view GetTypeName<float>()
        {
            return "float";
        }

        template <>
        inline constexpr std::string_view GetTypeName<unsigned int>()
        {
            return "unsigned int";
        }

        template <>
        inline constexpr std::string_view GetTypeName<long>()
        {
            return "long";
        }

        template <>
        inline constexpr std::string_view GetTypeName<long long>()
        {
            return "long long";
        }

        template <>
        inline constexpr std::string_view GetTypeName<unsigned long long>()
        {
            return "unsigned long long";
        }

        template <>
        inline constexpr std::string_view GetTypeName<char>()
        {
            return "char";
        }

        template <>
        inline constexpr std::string_view GetTypeName<wchar_t>()
        {
            return "wchar_t";
        }

        template <class T>
        static constexpr auto IsTypeClass = std::is_class_v<T>;

        template <class T>
        static constexpr auto IsTypeUnion = std::is_union_v<T>;

        template <class T>
        static constexpr auto IsTypeEnum = std::is_enum_v<T>;

        template <class T>
        static constexpr auto IsTypePrimitive = !IsTypeClass<T> && !IsTypeUnion<T> && !IsTypeEnum<T>;

        template <class... Ts>
        struct Tuple;

        template <>
        struct Tuple<>
        {
            using Current = void;
            using Next = Tuple<>;
        };

        template <class T, class... Ts>
        struct Tuple<T, Ts...>
        {
            using Current = T;
            using Next = Tuple<Ts...>;
        };

        //////////////////////////////////////////////////////////////////////////
        // Variable Traits
        template <class V>
        struct VariableTraits;

        template <class T>
        struct VariableTraits<T*>
        {
            using Pointer = T*;
            using Type = T;
        };

        template <class T, class C>
        struct VariableTraits<T C::*>
        {
            using Pointer = T C::*;
            using Type = T;
            using ClassType = C;
        };

        // Function Traits
        template <class F>
        struct FunctionTraits;

        template <class R, class C, typename... Params>
        struct FunctionTraits<R (C::*)(Params...)>
        {
            using ReturnType = R;
            using ClassType = C;
            using ParamTypes = Tuple<Params...>;
            static constexpr size_t ParamCount = sizeof...(Params);
        };

        //////////////////////////////////////////////////////////////////////////

    } // namespace TypeTraits

    namespace Detail
    {
        constexpr size_t GetTypenameWrapperPrefixLength()
        {
            return GetTypeNameWrapper<void>().find(TypeTraits::GetTypeName<void>());
        }

        constexpr size_t GetTypenameWrapperSuffixLength()
        {
            return GetTypeNameWrapper<void>().length() - GetTypenameWrapperPrefixLength() - TypeTraits::GetTypeName<void>().length();
        }

    } // namespace Detail

    namespace TypeTraits
    {
        template <class T>
        constexpr std::string_view GetTypeName()
        {
            constexpr auto typeNameWrapper = Detail::GetTypeNameWrapper<T>();
            constexpr auto prefixLength = Detail::GetTypenameWrapperPrefixLength();
            constexpr auto suffixLength = Detail::GetTypenameWrapperSuffixLength();
            constexpr auto typeNameLength = typeNameWrapper.length() - prefixLength - suffixLength;
            return typeNameWrapper.substr(prefixLength, typeNameLength);
        }

        template <class T>
        constexpr std::string_view GetClassName()
        {
            constexpr auto typeName = GetTypeName<T>();
            constexpr auto templateBegin = typeName.find_first_of('<');
            if constexpr (typeName.find("class") == 0)
                return typeName.substr(6, templateBegin - 6);
            else if constexpr (typeName.find("struct") == 0)
                return typeName.substr(7, templateBegin - 7);
            else if constexpr (typeName.find("union") == 0)
                return typeName.substr(6, templateBegin - 6);
            else
                return typeName.substr(0, templateBegin);
        }

    } // namespace TypeTraits

} // namespace SurgeReflect
