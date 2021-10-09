// Copyright (c) - SurgeTechnologies - All rights reserved
#pragma once
#include "SurgeReflect/Registry.hpp"
#include "SurgeReflect/Type.hpp"
#include <assert.h>

#define SURGE_REFLECTION_ENABLE                         \
private:                                                \
    struct ReflectionRegister                           \
    {                                                   \
        ReflectionRegister();                           \
        void CookClassData(SurgeReflect::Class& clazz); \
    };                                                  \
    inline static ReflectionRegister sReflectionRegister;

#define SURGE_REFLECT_CLASS_REGISTER_BEGIN(ClassName)                             \
    ClassName::ReflectionRegister::ReflectionRegister()                           \
    {                                                                             \
        SurgeReflect::Class& clazz = SurgeReflect::Class(#ClassName);             \
        CookClassData(clazz);                                                     \
        SurgeReflect::Registry::Get()->RegisterReflectionClass(std::move(clazz)); \
    }                                                                             \
    void ClassName::ReflectionRegister::CookClassData(SurgeReflect::Class& clazz) \
    {                                                                             \
        clazz
 

#define SURGE_REFLECT_CLASS_REGISTER_END(ClassName) ;}

namespace SurgeReflect
{
    template <typename T>
    const Class* GetReflection()
    {
        std::string className = std::string(TypeTraits::GetClassName<T>());
        const Class* clazz = Registry::Get()->GetClass(className);
        if (!clazz->IsSetup())
        {
            assert(false && "The class is not registered/setup in reflection engine! Maybe you forgot to Register the class?");
            Registry::Get()->RemoveClass(className);
        }
        return clazz;
    }

} // namespace SurgeReflect
