#pragma once
#include "Class.hpp"
#include <unordered_map>
#include <string>

namespace SurgeReflect
{
    class Registry
    {
    public:
        static Registry* Get();
        static void Shutdown();

        ~Registry();
        Class* GetClass(const std::string& name);
        void RegisterClass(Class&& clazz);

    private:
        std::unordered_map<std::string, Class*> mClasses;
    };

} // namespace SurgeReflect
