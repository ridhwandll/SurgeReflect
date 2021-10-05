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
    template <class T>
    constexpr std::string_view GetTypeName();

    template <>
    constexpr std::string_view GetTypeName<void>()
    {
        return "void";
    }

    template <>
    constexpr std::string_view GetTypeName<int>()
    {
        return "int";
    }

    template <>
    constexpr std::string_view GetTypeName<double>()
    {
        return "double";
    }

    template <>
    constexpr std::string_view GetTypeName<float>()
    {
        return "float";
    }

    template <>
    constexpr std::string_view GetTypeName<unsigned int>()
    {
        return "unsigned int";
    }

    template <>
    constexpr std::string_view GetTypeName<long>()
    {
        return "long";
    }

    template <>
    constexpr std::string_view GetTypeName<long long>()
    {
        return "long long";
    }

    template <>
    constexpr std::string_view GetTypeName<unsigned long long>()
    {
        return "unsigned long long";
    }

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

        constexpr size_t GetTypenameWrapperPrefixLength()
        {
            return GetTypeNameWrapper<void>().find(GetTypeName<void>());
        }

        constexpr size_t GetTypenameWrapperSuffixLength()
        {
            return GetTypeNameWrapper<void>().length() - GetTypenameWrapperPrefixLength() - GetTypeName<void>().length();
        }

    } // namespace Detail

    enum class AccessModifier
    {
        Public,
        Private,
        Protected
    };

    inline std::string AccessModifierToString(AccessModifier mod)
    {
        switch (mod)
        {
            case SurgeReflect::AccessModifier::Public: return "Public";
            case SurgeReflect::AccessModifier::Private: return "Private";
            case SurgeReflect::AccessModifier::Protected: return "Protected";
        }
        return "ERROR";
    }

    template <class T>
    struct BaseType
    {
        using Type = T;
    };

    template <class T>
    using BaseTypeT = typename BaseType<T>::Type;

    template <class T>
    struct BaseType<const T>
    {
        using Type = BaseTypeT<T>;
    };

    template <class T>
    struct BaseType<volatile T>
    {
        using Type = BaseTypeT<T>;
    };

    template <class T>
    struct BaseType<const volatile T>
    {
        using Type = BaseTypeT<T>;
    };

    template <class T>
    struct BaseType<T*>
    {
        using Type = BaseTypeT<T>;
    };

    template <class T>
    struct BaseType<T&>
    {
        using Type = BaseTypeT<T>;
    };

    template <class T>
    struct BaseType<T&&>
    {
        using Type = BaseTypeT<T>;
    };

    template <class T>
    struct BaseType<T[]>
    {
        using Type = BaseTypeT<T>;
    };

    template <class T, size_t C>
    struct BaseType<T[C]>
    {
        using Type = BaseTypeT<T>;
    };

    template <class T>
    static constexpr auto IsTypeClass = std::is_class_v<T>;

    template <class T>
    static constexpr auto IsTypeUnion = std::is_union_v<T>;

    template <class T>
    static constexpr auto IsTypeEnum = std::is_enum_v<T>;

    template <class T>
    static constexpr auto IsTypePrimitive = !IsTypeClass<T> && !IsTypeUnion<T> && !IsTypeEnum<T>;

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
    constexpr std::string_view GetBaseTypeName()
    {
        return GetTypeName<BaseTypeT<T>>();
    }

    template <class T>
    constexpr std::string_view GetClassName()
    {
        constexpr auto typeName = GetBaseTypeName<T>();
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

} // namespace SurgeReflect
