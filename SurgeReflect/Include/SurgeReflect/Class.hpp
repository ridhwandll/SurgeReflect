#pragma once
#include "SurgeReflect/Variable.hpp"
#include "SurgeReflect/TypeTraits.hpp"
#include <unordered_map>

namespace SurgeReflect
{
    class Class
    {
    public:
        Class() = default;
        Class(const std::string& name)
            : mName(name) {}

        const std::string& GetName() const { return mName; }
        const std::unordered_map<std::string, Variable>& GetVariables() const { return mVariables; }

        template <auto Var>
        Class& AddVariable(const std::string& name, AccessModifier accessModifier)
        {
            using Traits = TypeTraits::VariableTraits<decltype(Var)>;

            Variable v(name, accessModifier);
            v.Initialize<Traits::Type>();
            mVariables[v.GetName()] = v; // Store the variable
            return *this;
        }

        const Variable* GetVariable(const std::string& name) const
        {
            auto itr = mVariables.find(name);
            if (itr != mVariables.end())
                return &itr->second;

            return nullptr;
        }

    private:
        void SetupClass(Class&& clazz)
        {
            if (mSetup)
                return;

            mVariables.reserve(clazz.GetVariables().size());
            for (auto& [name, variable] : clazz.mVariables)
            {
                mVariables[name] = variable;
            }

            mSetup = true;
        }

    private:
        bool mSetup = false;
        std::string mName;
        std::unordered_map<std::string, Variable> mVariables;
        friend class Registry;
    };

} // namespace SurgeReflect
