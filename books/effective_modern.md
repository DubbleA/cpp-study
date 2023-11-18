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
 


```cpp

```