// Copyright (c) - SurgeTechnologies - All rights reserved
#pragma once
#include "SurgeReflect/Type.hpp"

namespace SurgeReflect
{
    class Function
    {
    public:
        Function() = default;
        Function(const std::string& name, AccessModifier accessModifier)
            : mName(name), mAccessModifier(accessModifier) {}

        template <auto Func>
        void Initialize()
        {
            // TODO
        }

        const std::string& GetName() const { return mName; }
        const AccessModifier& GetAccessModifier() const { return mAccessModifier; }

    private:
        std::string mName;
        AccessModifier mAccessModifier;
        // TODO: Return Type, Parameter Type and other good stuff
    };
} // namespace SurgeReflect
