#pragma once
#include "SurgeReflect/TypeTraits.hpp"
#include <string>

namespace SurgeReflect
{
    class Variable
    {
    public:
        Variable(const std::string& name, AccessModifier accessModifier)
            : mName(name), mAccessModifier(accessModifier) {}

        template <typename T>
        inline void Prepare()
        {
            mSize = sizeof(T);
            mIsEnum = IsTypeEnum<T>;
            mIsClass = IsTypeClass<T>;
            mIsUnion = IsTypeUnion<T>;
            mIsPrimitive = IsTypePrimitive<T>;
        }

        inline const std::string& GetName() const { return mName; }
        inline const AccessModifier& GetAccessModifier() const { return mAccessModifier; }
        inline const uint64_t& GetSize() const { return mSize; }
        inline const bool& IsEnum() const { return mIsEnum; }
        inline const bool& IsClass() const { return mIsClass; }
        inline const bool& IsUnion() const { return mIsUnion; }
        inline const bool& IsPrimitive() const { return mIsPrimitive; }

    private:
        std::string mName;
        AccessModifier mAccessModifier;
        uint64_t mSize = 0;
        bool mIsEnum = false;
        bool mIsClass = false;
        bool mIsUnion = false;
        bool mIsPrimitive = false;
    };

} // namespace SurgeReflect