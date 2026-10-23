<p align="center">
  <picture>
    <img alt="Surge" src="Docs/Images/SurgeReflect.png" width="100%">
  </picture>
</p>

<h1 align="center">Surge Reflect</h1>
<p align="center">
  <b>A Fast, Portable and Easy to use C++17 Reflection Library</b><br>
</p>

## Features

- No RTTI required, you can safely turn it **off**
- Mostly header only
- You can reflect **Classes**, **Structs**
  - **Members** & **Functions** that belongs to a class/struct, with **Access Modifier**
- Iterate over members **(`clazz->GetVariables()`)**
- Iterate over functions **(`clazz->GetFunctions()`)**
- **Enum to string** at compile time, no macros or registration needed **(`SurgeReflect::EnumToString(value)`)**
- Register only what you need
- Compiles with MSVC and Clang (should work with GCC too, but not tested)
- Easy to use API
- It's fast?

## Integrating SurgeReflect in your project
We assume that,
-  You are familiar enough with C++, you know what Static and Dynamic Libraries are in C++
- You know how to link them to an existing project
- You know what include directories are in C++ and you know how to set them

<br>

If you know all the above mentioned things, integrating SurgeReflect is **dead easy**. Copy all the files from `SurgeReflect` except `main.cpp`(you can look in `main.cpp` for examples),
set the include directory  to `SurgeReflect/Include`, compile the project(just one cpp file `SurgeReflect/Source/Registry.cpp`) as a static library, link it to your application and done!

<br>

*Note: Do not use the provided CMakeLists.txt file, it is just for testing purposes*

You can also directly embed the `SurgeReflect` folder in your project and use it without compiling it as a library by adding `SurgeReflect/Source/Registry.cpp` to your project files.

## Examples

To reflect a `class`(or a `struct`),  declare the `SURGE_REFLECTION_ENABLE;` inside the class(or a `struct`) like this
```cpp
// File: Cake.h
#include <SurgeReflect/Reflection.hpp>

class Cake
{
public:
    unsigned int Weight = 100;
    int AddCake(int a, int b) { return a + b; }
private:
    float Price = 20;
    SURGE_REFLECTION_ENABLE;
};
```
Now reflect, the struct, you **must do the following in a source file(.cpp file)**
```cpp
// File: Cake.cpp
#include "Cake.h"
//Other good(?) code ...

// Here 🔽
// IMPORTANT: Must be in Global namespace
SURGE_REFLECT_CLASS_REGISTER_BEGIN(Cake)
    .AddVariable<&Cake::Weight>("Weight", SurgeReflect::AccessModifier::Public)
    .AddVariable<&Cake::Price>("Price", SurgeReflect::AccessModifier::Private)
    .AddFunction<&Cake::AddCake>("AddCake", SurgeReflect::AccessModifier::Public)
SURGE_REFLECT_CLASS_REGISTER_END(Cake)
// Here 🔼
```
It is pretty self explanatory, you just register the members and functions like above with **correct name and access modifier**. Now you can get the juice of your work, everything is ready now, time to use this Reflection!
In `main.cpp`(or any other code place that is executed), you can query if `Cake` class has a member/function or not, if it has, then you can get information about that member/function! Here is a juicy example:
```cpp
//File: main.cpp
#include "Cake.h"
#include <iostream>

int main()
{
    const SurgeReflect::Class* clazz = SurgeReflect::GetReflection<Cake>();
    const SurgeReflect::Variable* var = clazz->GetVariable("Price");
    // var will be nullptr if the variable name "Price" was not registered
    if (var)    
    {
        const SurgeReflect::Type& typee = var->GetType();

        std::string name = var->GetName();      // "Price"
        uint64_t size = var->GetSize();         // 4
        bool isPrimitive = typee.IsPrimitive(); // true
        bool isEnum = typee.IsEnum();           // false
        bool isClass = typee.IsClass();         // false
        bool isUnion = typee.IsUnion();         // false

        // You can check if is equal to another type without RTTI enabled!
        bool isDouble = typee.EqualTo<double>();// false
        bool isFloat = typee.EqualTo<float>();   // true
    }
    
    const SurgeReflect::Function* func = clazz->GetFunction("AddCake");
    const SurgeReflect::Type& retType = func->GetReturnType();

    std::string name = retType.GetFullName(); // "int"
    
    // All the types in "AddCake"'s parameter, in this case it's 'int a' and 'int b'
    const std::vector<SurgeReflect::Type>& types = func->GetParameterTypes();

    // You can iterate through all the registered variables like this
    for (const auto& [name, variable] : clazz->GetVariables())
    {
        const SurgeReflect::Type& typee = variable.GetType();
        std::cout << "Name:           " << variable.GetName() << '\n';
        std::cout << "Size:           " << variable.GetSize() << " bytes" << '\n';
        std::cout << "IsPrimitive:    " << typee.IsPrimitive() << '\n';
        std::cout << "IsEnum:         " << typee.IsEnum() << '\n';
        std::cout << "IsClass:        " << typee.IsClass() << '\n';
        std::cout << "IsUnion:        " << typee.IsUnion() << '\n';
        std::cout << "AccessModifier: " << AccessModifierToString(variable.GetAccessModifier()) << '\n';
        std::cout << "-------------------------------------------------------" << std::endl;
    }
    
} // int main()
```

## Enums

Turn an enum value into its name with `SurgeReflect::EnumToString`. No registration is needed, it works on any enum.
```cpp
#include <SurgeReflect/Enum.hpp>

enum class CakeType { Chocolate, Vanilla, Mud };

constexpr std::string_view name = SurgeReflect::EnumToString(CakeType::Mud); // "Mud"
static_assert(SurgeReflect::EnumToString(CakeType::Vanilla) == "Vanilla");   // Works at compile time too
```
Namespaces are stripped, `Surge::ImageUsage::SAMPLED` gives `"SAMPLED"`.

**Value range:** By default, SurgeReflect looks at the values `0` to `15`. If your enum has values outside of this range,
specialize `SurgeReflect::EnumTraits` for it (the range can have at most 51 values, `Max - Min <= 50`):
```cpp
enum class Cake { CHOCOLATE = 400, VANILLA, BUTTERSCOTCH, BLUEBERRY, CHEESE };

namespace SurgeReflect
{
    template <>
    struct EnumTraits<Cake>
    {
        static constexpr int Min = 400; // First value of the enum
        static constexpr int Max = 404; // Last value of the enum
    };
}
```
Values that are out of range, or that do not have a name, return `"SurgeReflect::EnumToString: Unknown Enum value"`
(and trigger an `assert` in debug builds when out of range).

*Note: Prefer `enum class Foo` or `enum Foo : int`. Unscoped enums without a fixed underlying type (`enum Foo { ... }`) only work if every value in `[Min, Max]` fits in the enum's value range, otherwise the compiler rejects it. Keep `Min`/`Max` tight for these.*

## How is this useful?

It's mainly targeted towards game engines. It can be used for various purposes, for example:
- **C++ Scripting** in a game engine. You can reflect the `class` the user wrote and do stuff with the reflection data, like invoking certain methods in runtime if they exist etc.
- **Serialization** in a game engine. You can reflect the `class` you want to serialize and loop over all the members in that class in your serialization code! The benefit is that you don't have to touch your serialization code every time you modify the serialized `class`
- **Material System** in a game engine, you can reflect a C++ `struct` and query different stuff about a member, calculate the offset and determine the size on the fly!

## Projects and products using SurgeReflect

- [Surge](https://github.com/ridhwandll/Surge_Expired)

Feel free to make  a PR or an issue with your project name and link if you are using SurgeReflect!

