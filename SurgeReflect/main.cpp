#include <iostream>
#include <SurgeReflect/Reflection.hpp>

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
    int EatCake(int cakeCount, bool forceEat, CakeType typeToEat = CakeType::BigChonk) { return cakeCount; }

private:
    CakeType CakeEnum = CakeType::Mud;

    SURGE_REFLECTION_ENABLE;
};

int main()
{
    SurgeReflect::Registry::Initialize();

    TestStruct t;
    t.EatCake(8888, true);

    const SurgeReflect::Class* clazz = SurgeReflect::GetReflection<TestStruct>();
    const SurgeReflect::Variable* var = clazz->GetVariable("CakeEnum");
    if (var)
    {
        const SurgeReflect::Type& typee = var->GetType();

        std::string name = var->GetName();
        uint64_t size = var->GetSize();
        bool isPrimitive = typee.IsPrimitive();
        bool isEnum = typee.IsEnum();
        bool isClass = typee.IsClass();
        bool isUnion = typee.IsUnion();
        bool isCake = typee.EqualTo<CakeType>();
    }

    const SurgeReflect::Function* func = clazz->GetFunction("EatCake");
    const SurgeReflect::Type& typ = func->GetReturnType();
    std::string name = typ.GetFullName();

    const std::vector<SurgeReflect::Type>& types = func->GetParameterTypes();

    SurgeReflect::Registry::Shutdown();
}

// clang-format off

// Reflection Register
SURGE_REFLECT_CLASS_REGISTER_BEGIN(TestStruct)
    .AddVariable<&TestStruct::X>("X", SurgeReflect::AccessModifier::Public)
    .AddFunction<&TestStruct::EatCake>("EatCake", SurgeReflect::AccessModifier::Public)
    .AddVariable<&TestStruct::CakeEnum>("CakeEnum", SurgeReflect::AccessModifier::Private)
SURGE_REFLECT_CLASS_REGISTER_END(TestStruct)