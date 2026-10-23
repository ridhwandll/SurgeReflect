// Copyright (c) - SurgeTechnologies - All rights reserved
#pragma once
#include "Class.hpp"
#include <shared_mutex>
#include <unordered_map>
#include <string>

namespace SurgeReflect
{
    // Thread safety:
    // - Get(), Initialize(), GetClass(), RegisterReflectionClass() and RemoveClass() can be called from any thread.
    // - Classes are immutable once registered, so the `const Class*` returned by GetClass() can be used freely from any thread.
    // - Shutdown() and RemoveClass() destroy classes, the caller must make sure nobody is still using
    //   a `const Class*` (or a Variable/Function belonging to it) obtained earlier.
    class Registry
    {
    public:
        static void Initialize();
        static Registry* Get();
        static void Shutdown();

        ~Registry();

        // Returns nullptr if the class is not registered
        const Class* GetClass(const std::string& name) const;

        // A class that is already registered is left untouched
        void RegisterReflectionClass(Class&& clazz);
        void RemoveClass(const std::string& name);

    private:
        Registry() = default;
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

    private:
        mutable std::shared_mutex mMutex; // Many concurrent readers (GetClass), one writer (Register/Remove)
        std::unordered_map<std::string, Class*> mClasses;
    };

} // namespace SurgeReflect
