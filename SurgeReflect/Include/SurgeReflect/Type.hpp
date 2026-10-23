// Copyright (c) - SurgeTechnologies - All rights reserved
#pragma once
#include "TypeTraits.hpp"
#include <string>
#include <cstdint>

namespace SurgeReflect
{
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
            case AccessModifier::Public: return "Public";
            case AccessModifier::Private: return "Private";
            case AccessModifier::Protected: return "Protected";
        }
        return "ERROR";
    }

    class Type
    {
    public:
        Type() = default;
        ~Type() = default;

        template <typename T>
        bool EqualTo() const
        {
            const std::string givenTypeName = std::string(TypeTraits::GetTypeName<T>());
            const int64_t givenTypeHash = GenerateStringHash(givenTypeName);

            bool result = mHashCode == givenTypeHash;
            return result;
        }

        const std::string& GetFullName() const { return mFullName; }
        const int64_t& GetHashCode() const { return mHashCode; }
        const bool& IsEnum() const { return mIsEnum; }
        const bool& IsClass() const { return mIsClass; }
        const bool& IsUnion() const { return mIsUnion; }
        const bool& IsPrimitive() const { return mIsPrimitive; }

        template <typename T>
        void Initialize()
        {
            mIsEnum = TypeTraits::IsTypeEnum<T>;
            mIsClass = TypeTraits::IsTypeClass<T>;
            mIsUnion = TypeTraits::IsTypeUnion<T>;
            mIsPrimitive = TypeTraits::IsTypePrimitive<T>;

            mFullName = std::string(TypeTraits::GetTypeName<T>());
            mHashCode = GenerateStringHash(mFullName);
        }

    private:
        constexpr int64_t GenerateStringHash(const std::string_view s) const
        {
            // FNV-1a
            const uint64_t fnv_prime = 0x100000001B3ULL;
            uint64_t hash = 0xCBF29CE484222325ULL;

            for (char c : s)
            {
                hash ^= static_cast<unsigned char>(c);
                hash *= fnv_prime;
            }
            return static_cast<int64_t>(hash);
        }

    private:
        std::string mFullName;
        int64_t mHashCode = 0;

        bool mIsEnum = false;
        bool mIsClass = false;
        bool mIsUnion = false;
        bool mIsPrimitive = false;
    };

    template <class T>
    Type GetType()
    {
        Type type;
        type.Initialize<T>();
        return type;
    }

} // namespace SurgeReflect