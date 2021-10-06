#include "SurgeReflect/Registry.hpp"

namespace SurgeReflect
{
    void Registry::RegisterClass(Class&& clazz)
    {
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
