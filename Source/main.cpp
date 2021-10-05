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

class Cake
{
    uint32_t Weight = 100; // In KiloGrams
    uint32_t Price = 20;   // In dollars
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

    SURGE_REFLECT_REGISTER;
};

// clang-format off

SURGE_REFLECT_REGISTER_BEGIN(TestStruct)
    .AddVariable<&TestStruct::X>("X", SurgeReflect::AccessModifier::Public)
    .AddVariable<&TestStruct::Y>("Y", SurgeReflect::AccessModifier::Protected)
    .AddVariable<&TestStruct::Z>("Z", SurgeReflect::AccessModifier::Private)
    .AddVariable<&TestStruct::CakeEnum>("CakeEnum", SurgeReflect::AccessModifier::Private)
    .AddVariable<&TestStruct::TheRealCake>("TheRealCake", SurgeReflect::AccessModifier::Private)
SURGE_REFLECT_REGISTER_END(TestStruct)


int main()
{
    // clang-format on
    const SurgeReflect::Class* clazz = SurgeReflect::GetReflection<TestStruct>();

    for (const SurgeReflect::Variable& variable : clazz->GetVariables())
    {
        std::cout << "Name:           " << variable.GetName() << '\n';
        std::cout << "Size:           " << variable.GetSize() << " bytes" << '\n';
        std::cout << "IsPrimitive:    " << variable.IsPrimitive() << '\n';
        std::cout << "IsEnum:         " << variable.IsEnum() << '\n';
        std::cout << "IsClass:        " << variable.IsClass() << '\n';
        std::cout << "IsUnion:        " << variable.IsUnion() << '\n';
        std::cout << "AccessModifier: " << AccessModifierToString(variable.GetAccessModifier()) << '\n';
        std::cout << "-------------------------------------------------------" << std::endl;
    }

    SurgeReflect::Registry::Shutdown();
}
