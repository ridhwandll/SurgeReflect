// Copyright (c) - SurgeTechnologies - All rights reserved
#pragma once
#include <string>
#include <functional>

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
        int64_t GenerateStringHash(const std::string& s) const
        {
            int64_t result = 0;
            const int p = 31;
            const int m = 1e9 + 9;
            long long pPow = 1;
            for (char c : s)
            {
                result = (result + (c - 'a' + 1) * pPow) % m;
                pPow = (pPow * p) % m;
            }
            return result;
        }

    private:
        std::string mFullName;
        int64_t mHashCode;

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