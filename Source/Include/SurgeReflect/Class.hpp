#pragma once
#include "SurgeReflect/Variable.hpp"
#include "SurgeReflect/TypeTraits.hpp"
#include <vector>

namespace SurgeReflect
{
    class Class
    {
    public:
        Class() = default;
        Class(const std::string& name)
            : mName(name) {}

        inline const std::string& GetName() const { return mName; }
        inline const std::vector<Variable>& GetVariables() const { return mVariables; }

        template <auto Var>
        inline Class& AddVariable(const std::string& name, AccessModifier accessModifier)
        {
            using Traits = VariableTraits<decltype(Var)>;

            Variable v(name, accessModifier);
            v.Prepare<Traits::Type>();
            mVariables.push_back(v);
            return *this;
        }

        inline void SetupClass(Class&& clazz)
        {
            if (mSetup)
                return;

            mVariables.reserve(clazz.GetVariables().size());
            for (Variable& variable : clazz.mVariables)
                mVariables.push_back({variable});

            mSetup = true;
        }

    private:
        bool mSetup = false;
        std::string mName;
        std::vector<Variable> mVariables;
    };

} // namespace SurgeReflect
