// Copyright (c) - SurgeTechnologies - All rights reserved
#include "SurgeReflect/Registry.hpp"
#include <assert.h>
#include <atomic>
#include <memory>
#include <mutex>

namespace SurgeReflect
{
    static std::atomic<Registry*> sInstance { nullptr };
    static std::mutex sInstanceMutex; // Only taken while creating/destroying the instance

    void Registry::RegisterReflectionClass(Class&& clazz)
    {
        // Build the class completely before it becomes visible to other threads,
        // so GetClass() can never return a half-setup class
        const std::string name = clazz.GetName();
        auto newClass = std::make_unique<Class>(name);
        newClass->SetupClass(std::move(clazz));

        std::unique_lock lock(mMutex);
        if (mClasses.find(name) != mClasses.end())
            return;

        mClasses.emplace(name, newClass.release());
    }

    void Registry::Initialize()
    {
        Get();
    }

    Registry* Registry::Get()
    {
        Registry* instance = sInstance.load(std::memory_order_acquire);
        if (instance)
            return instance;

        std::lock_guard lock(sInstanceMutex);
        instance = sInstance.load(std::memory_order_relaxed);
        if (!instance)
        {
            instance = new Registry();
            sInstance.store(instance, std::memory_order_release);
        }
        return instance;
    }

    const Class* Registry::GetClass(const std::string& name) const
    {
        std::shared_lock lock(mMutex);
        auto itr = mClasses.find(name);
        if (itr != mClasses.end())
            return itr->second;

        return nullptr;
    }

    void Registry::RemoveClass(const std::string& name)
    {
        std::unique_lock lock(mMutex);
        auto itr = mClasses.find(name);
        if (itr != mClasses.end())
        {
            delete itr->second;
            mClasses.erase(itr);
            return;
        }
        assert(false && "Trying to remove a class that is not present!");
    }

    void Registry::Shutdown()
    {
        std::lock_guard lock(sInstanceMutex);
        delete sInstance.exchange(nullptr, std::memory_order_acq_rel);
    }

    Registry::~Registry()
    {
        for (auto& clazz : mClasses)
            delete clazz.second;
    }

} // namespace SurgeReflect
