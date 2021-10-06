// Copyright (c) - SurgeTechnologies - All rights reserved
#include "SurgeReflect/Registry.hpp"
#include <assert.h>

namespace SurgeReflect
{
    void Registry::RegisterClass(Class&& clazz)
    {
        // Allocate a new class and set that up according to the given class
        GetClass(clazz.GetName())->SetupClass(std::move(clazz));
    }

    SurgeReflect::Registry* Registry::Get()
    {
        static Registry* sInstance = nullptr;
        if (!sInstance)
            sInstance = new Registry();

        return sInstance;
    }

    Class* Registry::GetClass(const std::string& name)
    {
        auto itr = mClasses.find(name);
        if (itr != mClasses.end())
            return itr->second;
        return mClasses.insert({name, new Class(name)}).first->second;
    }

    void Registry::RemoveClass(const std::string& name)
    {
        auto itr = mClasses.find(name);
        if (itr != mClasses.end())
        {
            delete itr->second;
            mClasses.erase(name);
            return;
        }
        assert(false && "Trying to remove a class that is not present!");
    }

    void Registry::Shutdown()
    {
        delete Get();
    }

    Registry::~Registry()
    {
        for (auto& clazz : mClasses)
            delete clazz.second;
    }

} // namespace SurgeReflect
