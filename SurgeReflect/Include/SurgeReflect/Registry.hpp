// Copyright (c) - SurgeTechnologies - All rights reserved
#pragma once
#include "Class.hpp"
#include <unordered_map>
#include <string>

namespace SurgeReflect
{
    class Registry
    {
    public:
        static void Initialize();
        static Registry* Get();
        static void Shutdown();

        ~Registry();
        Class* GetClass(const std::string& name);
        void RegisterReflectionClass(Class&& clazz);
        void RemoveClass(const std::string& name);

    private:
        std::unordered_map<std::string, Class*> mClasses;
    };

} // namespace SurgeReflect
