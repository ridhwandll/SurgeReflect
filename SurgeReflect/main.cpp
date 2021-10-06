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

class TestStruct
{
public:
    int X = 0;

protected:
    VecKek Y = {0.0f, 0.0f};
    void EatCake() {}

private:
    uint64_t Z = 0;
    CakeType CakeEnum = CakeType::Mud;
    SURGE_REFLECTION_ENABLE;
};

int main()
{
    const SurgeReflect::Class* clazz = SurgeReflect::GetReflection<TestStruct>();
    const SurgeReflect::Variable* var = clazz->GetVariable("CakeEnum");
    if (var)
    {
        const SurgeReflect::Type& typee = var->GetType();

        auto name = var->GetName();
        auto size = var->GetSize();
        auto isPrimitive = typee.IsPrimitive();
        auto isEnum = typee.IsEnum();
        auto isClass = typee.IsClass();
        auto isUnion = typee.IsUnion();
        bool isCake = typee.EqualTo<CakeType>();
    }

    const SurgeReflect::Function* func = clazz->GetFunction("EatCake");

    //DumpVariables(testStruct);

    SurgeReflect::Registry::Shutdown();
}

// Reflection Register

// clang-format off
SURGE_REFLECT_CLASS_REGISTER_BEGIN(TestStruct)
    .AddVariable<&TestStruct::X>("X", SurgeReflect::AccessModifier::Public)
    .AddVariable<&TestStruct::Y>("Y", SurgeReflect::AccessModifier::Protected)
    .AddVariable<&TestStruct::Z>("Z", SurgeReflect::AccessModifier::Private)
    .AddVariable<&TestStruct::CakeEnum>("CakeEnum", SurgeReflect::AccessModifier::Private)
    .AddFunction<&TestStruct::EatCake>("EatCake", SurgeReflect::AccessModifier::Protected)
SURGE_REFLECT_CLASS_REGISTER_END(TestStruct)