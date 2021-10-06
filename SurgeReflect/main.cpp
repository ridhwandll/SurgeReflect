#include <iostream>
#include "SurgeReflect/Reflection.hpp"

struct VecKek
{
    float x, y;
    float e, r;
    float g, h;
};

enum class CakeType
{
    Mud,
    DarkForest,
    BigChonk
};

void DumpVariables(const SurgeReflect::Class* clazz)
{
    for (const auto& [name, variable] : clazz->GetVariables())
    {
        const SurgeReflect::Type& typee = variable.GetType();
        std::cout << "Name:           " << variable.GetName() << '\n';
        std::cout << "Size:           " << variable.GetSize() << " bytes" << '\n';
        std::cout << "IsPrimitive:    " << typee.IsPrimitive() << '\n';
        std::cout << "IsEnum:         " << typee.IsEnum() << '\n';
        std::cout << "IsClass:        " << typee.IsClass() << '\n';
        std::cout << "IsUnion:        " << typee.IsUnion() << '\n';
        std::cout << "AccessModifier: " << SurgeReflect::AccessModifierToString(variable.GetAccessModifier()) << '\n';
        std::cout << "-------------------------------------------------------" << std::endl;
    }
}

class Cake
{
public:
    uint32_t Weight = 100; // In KiloGrams
    uint32_t Price = 20;   // In dollars
    short TastyMeter = 'a';
    SURGE_REFLECTION_ENABLE;
};

class TestStruct
{
public:
    int X = 0;

protected:
    VecKek Y = {0.0f, 0.0f};

private:
    uint64_t Z = 0;
    CakeType CakeEnum = CakeType::Mud;
    Cake TheRealCake;
    SURGE_REFLECTION_ENABLE;
};

int main()
{
    {
        const SurgeReflect::Class* clazz = SurgeReflect::GetReflection<TestStruct>();
        const SurgeReflect::Variable* var = clazz->GetVariable("TheRealCake");
        if (var)
        {
            const SurgeReflect::Type& typee = var->GetType();

            auto name = var->GetName();
            auto size = var->GetSize();
            auto isPrimitive = typee.IsPrimitive();
            auto isEnum = typee.IsEnum();
            auto isClass = typee.IsClass();
            auto isUnion = typee.IsUnion();
            bool isCake = typee.EqualTo<Cake>();
        }
    }

    {
        const SurgeReflect::Class* clazz = SurgeReflect::GetReflection<Cake>();
        const SurgeReflect::Variable* var = clazz->GetVariable("TastyMeter");
        if (var)
        {
            const SurgeReflect::Type& typee = var->GetType();

            std::string name = var->GetName();
            uint64_t size = var->GetSize();
            bool isPrimitive = typee.IsPrimitive();
            bool isEnum = typee.IsEnum();
            bool isClass = typee.IsClass();
            bool isUnion = typee.IsUnion();
            bool isCake = typee.EqualTo<Cake>();
        }
    }

    //DumpVariables(testStruct);

    SurgeReflect::Registry::Shutdown();
}

// Reflection Register

// clang-format off
SURGE_REFLECT_CLASS_REGISTER_BEGIN(Cake)
    .AddVariable<&Cake::Weight>("Weight", SurgeReflect::AccessModifier::Public)
    .AddVariable<&Cake::Price>("Price", SurgeReflect::AccessModifier::Public)
    .AddVariable<&Cake::TastyMeter>("TastyMeter", SurgeReflect::AccessModifier::Public)
SURGE_REFLECT_CLASS_REGISTER_END(Cake)


SURGE_REFLECT_CLASS_REGISTER_BEGIN(TestStruct)
    .AddVariable<&TestStruct::X>("X", SurgeReflect::AccessModifier::Public)
    .AddVariable<&TestStruct::Y>("Y", SurgeReflect::AccessModifier::Protected)
    .AddVariable<&TestStruct::Z>("Z", SurgeReflect::AccessModifier::Private)
    .AddVariable<&TestStruct::CakeEnum>("CakeEnum", SurgeReflect::AccessModifier::Private)
    .AddVariable<&TestStruct::TheRealCake>("TheRealCake", SurgeReflect::AccessModifier::Private)
SURGE_REFLECT_CLASS_REGISTER_END(TestStruct)