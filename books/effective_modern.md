# Introduction

### lvalues vs rvalues
rvalues indicate objects eligible for move operations, while lvalues generally don't. In concept rvalues correspond to temporary objects returned from functions, while lvalues correspond to objects you can refer to (either by name or by following a pointer or lvalue reference).

Useful rule of thumb is to see if you can take its address. If you can, it's probably an lvalue reference otherwise its probably an rvalue reference. Rvalues are usually values on the "right hand side" whereas lvalues are values on the "left hand side" i.e. int x = 4; x is a lvalue 4 is a rvalue. 

Copies of rvalues are generally move constructed while copies of lvalues are usually copy constructed. 

# Chapter 1: Deducing Types

## Item 1: Understand template type deduction

```cpp
template<typename T>
void f(ParamType param);

f(expr); // call f with some expression
```

During compilation, compilers use expr to deduce two types: one for T and one for ParamType. These types are frequently different, because ParamType often contains adornments, e.g. const or reference qualifiers.
i.e.

```cpp
template<typename T>
void f(const T& param); // ParamType is const T&

int x = 0;
f(x); //call f with an int
```
T is deduced to be int, but ParamType is deduced to be const int&.

### Case 1: ParamType is a Reference or Pointer, but not a Universal Reference. 
When ParamType is a reference or pointer type, but not a universal reference. In that case, type deduction works like this:
1. If expr's type is a reference, ignore the reference part.
2. Then pattern-match expr's type against ParamType to determine T.

i.e. 
```cpp
template<typename T>
void f(T& param); // param is a reference

int x = 27; // x is an int
f(x); //T is int, param's type is int&

const int cx x = // cx is a const int
f(cx); //T is const int, param's type is const int&

const int& rx = x; // rx is a reference to x as a const int
f(rx); //T is const int, param's type is const int&
```

If we change the type of f's parameter from T& to const T&, the constness of cx and rx continues to be respected, but because we're now assuming that param is a reference-to-const, tehre's no longer a need for const to be deduced as part of T:

```cpp
void f(const T& param); // param is a now ref-to-const
f(x); // T is int, param's type is const int&
f(cx); // T is int, param's type is const int&
f(rx); // T is int, param's type is const int&
```

Things would work essentially the same way if param was a pointer instead of reference.

### Case 2: ParamType is a Universal Reference. 

1. If expr is an lvalue, both T and ParamType are lvalue references. 
2. If expr is an rvalue, the case 1 rules apply.

```cpp
template<typename T>
void f(T&& param); // param is now a universal reference (usually an rvalue reference)
int x = 27; 
const int cx = x; 
const int& rx = x;
f(x); // x is lvalue, so T is int&,         param's type is also int&
f(cx); // cx is lvalue, so T is const int&, param's type is also const int&
f(rx); // rx is lvalue, so T is const int&, param's type is also const int&
f(27); // 27 is rvalue, so T is int,        param's type is therefore int&&
```

### Case 3: ParamType is Neither a Pointer nor a Reference (aka pass-by-value)

When ParamType is a copy of whatever is passed in (a completely new object) results in a few new rules

1. If expr's type is a reference, ignore the reference part
2. If expr is const, ignore that too. (or volatile)

```cpp
template<typename T>
void f(T param); // param is now passed by value
int x = 27; 
const int cx = x; 
const int& rx = x;
f(x); // T's and param's types are both int
f(cx); // T's and param's types are again both int
f(rx); // T's and param's types are still both int
```

### Things to Remember

1. During template type deduction, arguments that are references are treated as
non-references, i.e., their reference-ness is ignored.
2. When deducing types for universal reference parameters, lvalue arguments get
special treatment.
3. When deducing types for by-value parameters, const and/or volatile argu‐
ments are treated as non-const and non-volatile.
4. During template type deduction, arguments that are array or function names
decay to pointers, unless they’re used to initialize references.

## Item 2: Understand auto type deduction.

There’s a direct mapping between template type deduction and auto type deduction. There is literally an algorithmic transformation from one to the other. Auto type deduction is template type deduction.

```cpp
auto x = 27; // type specifier for x is simply auto
```

The compiler acts as if there was a template for each declaration as well as a call to that template with the corresponding initializing expression:

```cpp
template<typename T> // conceptual template for deducing x's type
void func_for_x(T param);

func_for_x(27); // conceptual call: param's deduced type is x's type
```

```cpp
const auto cx = x; // type specifier is const auto

//conceptual template and call generated by compiler:
template<typename T> //conceptual template for deducing cx's type
void func_for_cx(const T param);

func_for_cx(x); // conceptual call for deduced type is cx's type

const auto& rx = x; // type specifier is const auto&

//conceptual template and call generated by compiler:
template<typename T> //conceptual template for deducing rx's type
void func_for_rx(const T& param);

func_for_rx(x); // conceptual call for deduced type is rx's type
```

Following the rules from Item 1: 

```cpp
auto x = 27; //case 3 (x is neither pointer nor reference)
const auto cx = x; //case 3 (cx isnt either)
const auto& rx = x; //case 1 (rx is a non-universal ref)

auto&& uref1 = x; // case2: x is int and lvalue, so uref1's type is int&
auto&& uref2 = cx; // case2: cx is const int and lvalue, so uref2's type is const int&
auto&& uref3 = 27; // case2: 27 is int and rvalue, so uref3's type is int&&

auto x = { 11, 23, 9 }; // x's type is std::initializer_list<int>

template<typename T> // template with parameter
void f(T param); // declaration equivalent to x's declaration
f({ 11, 23, 9 }); // error! can't deduce type for T
```

However, if you specify in the template that param is a std::initializer_list<T>
for some unknown T, template type deduction will deduce what T is:

```cpp
template<typename T>
void f(std::initializer_list<T> initList);
f({ 11, 23, 9 }); // T deduced as int, and initList's type is std::initializer_list<int>
```

So the only real difference between auto and template type deduction is that auto
assumes that a braced initializer represents a std::initializer_list, but template
type deduction doesn’t.

### Things to Remember

1. auto type deduction is usually the same as template type deduction, but auto
type deduction assumes that a braced initializer represents a std::initial
izer_list, and template type deduction doesn’t.
2. auto in a function return type or a lambda parameter implies template type
deduction, not auto type deduction.

## Item 3: Understand decltype.

