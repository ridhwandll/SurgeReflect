// Copyright (c) - SurgeTechnologies - All rights reserved
#pragma once
#include <string_view>
#include <type_traits>
#include <array>
#include <utility>
#include <cstddef>
#include <cassert>

namespace SurgeReflect
{
    // By default, enums are assumed to be between 0 and 15
    // Specialize this template anywhere in codebase to expand or shift the bounds
    //
    // NOTE: For unscoped enums WITHOUT a fixed underlying type (`enum Foo { ... }`), every value in
    // [Min, Max] must fit the enum's value range, otherwise the compiler rejects the table.
    // Prefer `enum class Foo` or `enum Foo : int` and keep [Min, Max] tight.
    /*
    * Example:
    *
    * enum class Cake
    * {
    *     CHOCOLATE = 400, //EnumTraits<Cake>::Min
    *     VANILLA,
    *     BUTTERSCOTCH,
    *     BLUEBERRY,
    *     CHEESE //404 -> //EnumTraits<Cake>::Max
    * };
    *
    * namespace SurgeReflect
    * {
    *     template <>
    *     struct EnumTraits<Cake>
    *     {
    *         // Define the Min and Max of your enum value here
    *         static constexpr int Min = 400;
    *         static constexpr int Max = 404;
    *     };
    * }
    */
    template <typename T>
    struct EnumTraits
    {
        static constexpr int Min = 0;
        static constexpr int Max = 15;
    };

    namespace Detail
    {
        constexpr int MaxEnumRange = 50;
        constexpr std::string_view UnknownEnumName = "SurgeReflect::EnumToString: Unknown Enum value";

        template <typename T, T Value>
        constexpr std::string_view GetEnumSignature()
        {
#if defined(__clang__) || defined(__GNUC__)
            return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
            return __FUNCSIG__;
#else
#error "SurgeReflect: Unsupported Compiler!"
#endif
        }

        // Extracts the enumerator name out of the compiler generated function signature.
        // Returns an empty string_view if the value has no name (compilers print a number or a cast instead)
        template <typename T, T Value>
        constexpr std::string_view ParseEnumName()
        {
            std::string_view name = GetEnumSignature<T, Value>();
#if defined(__clang__) || defined(__GNUC__)
            // Isolate the "Value = " identifier token, https://godbolt.org/z/a5aP6Ts3v
            constexpr std::string_view target = "Value = ";
            const size_t pos = name.find(target);
            if (pos == std::string_view::npos)
                return {};

            name.remove_prefix(pos + target.length());

            // Chop off everything after the value token (stops at ; or ])
            const size_t end = name.find_first_of(";]");
            if (end != std::string_view::npos)
                name.remove_suffix(name.size() - end);

#elif defined(_MSC_VER)
            // MSVC generates: auto __cdecl GetEnumSignature<enum Color,Color::RED>(void)
            const size_t close = name.rfind('>');
            if (close == std::string_view::npos)
                return {};

            name.remove_suffix(name.size() - close);

            const size_t comma = name.rfind(',');
            if (comma == std::string_view::npos)
                return {};

            name.remove_prefix(comma + 1);

            while (!name.empty() && name.front() == ' ')
                name.remove_prefix(1);
#endif

            // Namespace::Enum::SAMPLED turns into SAMPLED
            const size_t lastColon = name.rfind("::");
            if (lastColon != std::string_view::npos)
                name.remove_prefix(lastColon + 2);

            // Filter out compiler-generated fallback ("5", "-1", "(Color)16") for unmapped values
            if (!name.empty() && ((name[0] >= '0' && name[0] <= '9') || name[0] == '-' || name[0] == '('))
                return {};

            return name;
        }

        template <std::size_t N>
        struct FixedString
        {
            char Data[N + 1] {};
            constexpr FixedString(std::string_view sv)
            {
                for (std::size_t i = 0; i < N; ++i)
                    Data[i] = sv[i];

                Data[N] = '\0';
            }
        };

        template <typename T, T Value, std::size_t N>
        struct NameCache
        {
            // Exactly one instance of the string per enum value
            static constexpr FixedString<N> Buffer { ParseEnumName<T, Value>() };
        };

        template <typename T, T Value>
        constexpr std::string_view GetEnumName()
        {
            constexpr auto raw = ParseEnumName<T, Value>();
            if constexpr (raw.empty())
                return {};
            else
                return { NameCache<T, Value, raw.size()>::Buffer.Data, raw.size() };
        }

        template <typename T, int... Is>
        constexpr auto MakeNameTable(std::integer_sequence<int, Is...>)
        {
            return std::array<std::string_view, sizeof...(Is)> {
                GetEnumName<T, static_cast<T>(Is + EnumTraits<T>::Min)>()...
            };
        }

        template <typename T>
        struct EnumTableCache
        {
            static_assert(std::is_enum_v<T>, "SurgeReflect::EnumToString only works with enums!");
            static constexpr int Min = EnumTraits<T>::Min;
            static constexpr int Max = EnumTraits<T>::Max;

            static_assert(Max >= Min, "Error: Max bound must be greater than or equal to Min bound!");
            static_assert(Max - Min <= MaxEnumRange, "Error: Range exceeds 50 values! Split your enum or adjust EnumTraits to prevent compiler freezing!");

            static constexpr auto Table = MakeNameTable<T>(std::make_integer_sequence<int, Max - Min + 1> {});
        };
    } // namespace Detail

    template <typename T>
    constexpr std::string_view EnumToString(T value)
    {
        constexpr auto& table = Detail::EnumTableCache<T>::Table;

        // 64-bit math, so a value like 0x1'0000'0000 can't wrap around into the table
        const long long index = static_cast<long long>(value) - EnumTraits<T>::Min;
        if (index < 0 || index >= static_cast<long long>(table.size()))
        {
            assert(false && "SurgeReflect::EnumToString Enum value out of reflection bounds! Did you forget to specialize the EnumTraits template?(look at the top of this file)");
            return Detail::UnknownEnumName;
        }

        const std::string_view name = table[static_cast<std::size_t>(index)];
        return name.empty() ? Detail::UnknownEnumName : name;
    }

} // namespace SurgeReflect
