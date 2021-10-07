// Copyright (c) - SurgeTechnologies - All rights reserved
#pragma once
#include "SurgeReflect/Type.hpp"
#include "SurgeReflect/TypeTraits.hpp"

namespace SurgeReflect
{
    class Function
    {
    public:
        Function() = default;
        Function(const std::string& name, AccessModifier accessModifier)
            : mName(name), mAccessModifier(accessModifier) {}

        const std::string& GetName() const { return mName; }
        const AccessModifier& GetAccessModifier() const { return mAccessModifier; }
        const Type& GetReturnType() const { return mReturnType; }
        const std::vector<Type>& GetParameterTypes() const { return mParameterTypes; }

    private:
        template <auto Func>
        void Initialize()
        {
            using Traits = TypeTraits::FunctionTraits<decltype(Func)>;
            mReturnType.Initialize<typename Traits::ReturnType>();
            SetParameterTypes(typename Traits::ParamTypes {});
        }

    private:
        template <class... Params>
        void SetParameterTypes(TypeTraits::Tuple<Params...>)
        {
            (mParameterTypes.push_back(GetType<Params>()), ...);
        }

    private:
        std::string mName;
        AccessModifier mAccessModifier;

        Type mReturnType;
        std::vector<Type> mParameterTypes;
        friend class Class;
    };
} // namespace SurgeReflect
