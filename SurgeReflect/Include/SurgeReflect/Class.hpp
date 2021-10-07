// Copyright (c) - SurgeTechnologies - All rights reserved
#pragma once
#include "SurgeReflect/Variable.hpp"
#include "SurgeReflect/TypeTraits.hpp"
#include "SurgeReflect/Function.hpp"
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
        const std::unordered_map<std::string, Function>& GetFunctions() const { return mFunctions; }

        template <auto Var>
        Class& AddVariable(const std::string& name, AccessModifier accessModifier)
        {
            Variable v(name, accessModifier);
            v.Initialize<Var>();
            mVariables[v.GetName()] = v;
            return *this;
        }

        template <auto Func>
        Class& AddFunction(const std::string& name, AccessModifier accessModifier)
        {
            Function fun(name, accessModifier);
            fun.Initialize<Func>();
            mFunctions[fun.GetName()] = fun;
            return *this;
        }

        const Variable* GetVariable(const std::string& name) const
        {
            auto itr = mVariables.find(name);
            if (itr != mVariables.end())
                return &itr->second;

            return nullptr;
        }

        const Function* GetFunction(const std::string& name) const
        {
            auto itr = mFunctions.find(name);
            if (itr != mFunctions.end())
                return &itr->second;

            return nullptr;
        }

        const bool& IsSetup() const { return mSetup; }

    private:
        void SetupClass(Class&& clazz)
        {
            if (mSetup)
                return;

            mVariables.reserve(clazz.GetVariables().size());
            for (auto& [name, variable] : clazz.mVariables)
                mVariables[name] = variable;

            mFunctions.reserve(clazz.GetFunctions().size());
            for (auto& [name, func] : clazz.mFunctions)
                mFunctions[name] = func;

            mSetup = true;
        }

    private:
        bool mSetup = false;
        std::string mName;
        std::unordered_map<std::string, Variable> mVariables;
        std::unordered_map<std::string, Function> mFunctions;
        friend class Registry;
    };

} // namespace SurgeReflect
