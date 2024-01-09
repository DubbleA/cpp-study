## Back to Basics: Object-Oriented Programming - Jon Kalb - CppCon 2019

Definition of OOP: a programming paradigm in C++ using polymorphism based on runtime function dispatch using virtual functions. 

Alternatives:
- object-based
- static polymorphism
- functional programming

___

### Objects as Libraries

- Derived objects in OOP can be thought of as independent libraries.
    - The libraries all have the same API (defined by base class)
    - These libraries (objects) can impl and extend this API
        - compile time (the type of the derived object)
        - runtime (the state of the derived object)


Example: Logging

- seperation of concerns.
    - app logic code that wants to log events or state
    - code "knows about" and is responsible for logging

- base class: defines the api of logging libraries
- derived classes: provide tdifferent impls of these libraries

```cpp
struct Logger // base class: defines library interface
{
    virtual void LogMessage(char const* message) = 0;
    virtual ~Logger() = default; 
};

struct ConsoleLogger final: Logger // derived class: provides one library impl
{
    virtual void LogMessage(char const* message) override 
    {
        std::cout << message << '\n';
    }
};

int main() 
{
    auto logger {std::make_unique<ConsoleLogger>()};

    Logger* logger_ptr{logger.get()};
    //without virtual function, would call Logger::LogMessage

    logger_ptr->LogMessage("Hello, World!"); 
    // static (compile-time) type of logger_ptr is Logger*
    // dynamic (runtime) type of logger_ptr is ConsoleLogger*
}
```
___
simple example (logging), continued
```cpp
//code neither knows nor depends on dynamic type of logger
void LogHelloWorld(Logger& logger) // base class reference of pointer
{
    logger.LogMessage("Hello, World!"); // code written to base type API can be used with any derived type.
}

int main()
{
    auto logger{std::make_unique<ConsoleLogger>()};

    LogHelloWorld(*logger);
    LogHelloWorld(*static_cast<Logger*>(logger.get()));
}

struct FileLogger final: Logger
{
    FileLogger(char const* filename):
        output_{filename} {}
    virtual void LogMessage(char const* message) override
    {
        output_ << message << '\n';
    }
private:
    std::ofstream output_;
};

int main()
{
    auto cl{ConsoleLogger{}};
    auto fl{FileLogger{"logfile.txt"}};

    LogHelloWorld(cl);
    LogHelloWorld(fl);
}
```

___

## Liskov substitution

- Barbara Liskob defined what she called the Subtype Requirement. 
- Essentially, D is a subtype of B if the provable properties of objects of type B are also proivable properties of objects of type D. 
- In practice, this is interpreted to mean that code written to behave correctly against the API defined by B will also work correctly for objects of type D if D is a proper subtype. 

- Not all derived types are subtypes.

OOP theory from C++ coding standards rule 38: practice safe overriding:
"After the base class guarantees the preconditions and postconditions of an operation, any derived class must respect those guarantees. An override can ask for less and provide more, but it must never require more or promise less because that would break the contract that was promised to calling code."

___
### design best practices:

### inheritence

- many early users of inheritance conceived of it as a technique for enabling code re-use
- inheritance should be applicable to any case of two code paths being similar, but not identical
- this is likely to end in unmaintainable mess. 

- the starting point or OOP as a problem solving tool:
    - public inheritance models "is-a"
    - base class defines an interface for an object that might provide a type of functionality
    - derived classes provide implementations of different expressions of that object type

```cpp
struct Logger 
{
    virtual void LogMessage(char const* message) = 0;
    virtual ~Logger() = default;
};

struct StatusDisplayer final: Logger // displayer not strictly a logger
{
    virtual void LogMessage(char const* message) override
    {
        /* show message as current status on LCD */;
    }
};
```

- Powerful hierarchies are built on well-defined abstractions
- Object types that do two or more only-partially related things
    - make it hard to determine an optimal interface
    - make it hard to write client code
    - result in unmaintainable hierarchies

- **use OOP to model "is-a" relationships, not for code-reuse.**
- **"public inheritence is substitutability. Inherit, not to reuse, but to be reused"**

### make non leaf-classes abstract

```cpp
Lizard liz1; 
Lizard liz2;

Animal *pAnimal1 = &liz1;
Animal *pAnimal2 = &liz2;

*pAnimal1 = *pAnimal2; //slicing only animal part will be assigned not lizard part

sizeof(*pAnimal1); //logic error gonna tell us its an animal not lizard
```
- in OOP we are constantly lying to the compiler about object pointers
- the static type is almost never the dynamic type
- we survive by this rule:
    - **ONLY DEREFERENCE AN OOP POINTER/REF TO ACCESS BASE CLASS MEMBERS**
    - because the (static) type, when dereferenced, is not the actual (dynamic) type.

```cpp

```
