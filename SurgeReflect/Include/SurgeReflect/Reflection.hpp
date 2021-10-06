#pragma once
#include "SurgeReflect/Registry.hpp"
#include "SurgeReflect/Type.hpp"

#define SURGE_REFLECTION_ENABLE                         \
private:                                                \
    static struct ReflectionRegister                    \
    {                                                   \
        ReflectionRegister();                           \
        void CookClassData(SurgeReflect::Class& clazz); \
    } sReflectionRegister

#define SURGE_REFLECT_CLASS_REGISTER_BEGIN(ClassName)                             \
    ClassName::ReflectionRegister ClassName::sReflectionRegister;                 \
    ClassName::ReflectionRegister::ReflectionRegister()                           \
    {                                                                             \
        SurgeReflect::Class& clazz = SurgeReflect::Class(#ClassName);             \
        CookClassData(clazz);                                                     \
        SurgeReflect::Registry::Get()->RegisterClass(std::move(clazz));           \
    }                                                                             \
    void ClassName::ReflectionRegister::CookClassData(SurgeReflect::Class& clazz) \
    {                                                                             \
        clazz

// clang-format off

#define SURGE_REFLECT_CLASS_REGISTER_END(ClassName) ;}

// clang-format on

namespace SurgeReflect
{
    template <typename T>
    const Class* GetReflection()
    {
        std::string className = std::string(TypeTraits::GetClassName<T>());
        const Class* clazz = Registry::Get()->GetClass(className);
        return clazz;
    }

} // namespace SurgeReflect
