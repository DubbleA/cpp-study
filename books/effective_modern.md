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

Given a name or an expression, decltype tells you the name's or the expression's type. In contrast to what happens during type deduction for templates and auto in Items 1 & 2, decltype typically tells you the exact type of the name or expression given:

```cpp
const int i = 0; // decltype(i) is const int
bool f(const Widget& w); // decltype(w) is const Widget&, decltype(f) is bool(const Widget&)
struct Point {
 int x, y; // decltype(Point::x) is int, decltype(Point::y) is int
};
Widget w; // decltype(w) is Widget
if (f(w)) … // decltype(f(w)) is bool
template<typename T> // simplified version of std::vector
class vector {
public:
 …
 T& operator[](std::size_t index);
 …
};
vector<int> v; // decltype(v) is vector<int>
…
if (v[0] == 0) … // decltype(v[0]) is int&
```

In C++11 the primary use for decltype is typically declaring function templates where the function's return type depends on its pparameter types. i.e. lets say we were creating a template for a container to return a value of the type of the container after authenticating the user. 

```cpp
template<typename Container, typename Index> // C++14; not quite correct
auto authAndAccess(Container& c, Index i)
{
 authenticateUser();
 return c[i]; // return type deduced from c[i]
}

std::deque<int> d;
…
authAndAccess(d, 5) = 10; // authenticate user, return d[5], then assign 10 to it;
// this won't compile!
```

Item 1 explains that during template type deduction, the reference-ness of an initializing expression is ignored. Here 
d[5] returns an int&, but the auto return type declaration stips off the reference, thus yielding a return type of int. That int, return value is an rvalue, and the above code attempts to assign a rvalue 10 to an rvalue int (forbidden in C++)

to get authAndAccess to work we need to use decltype type deduction for its return type, i.e. specify that authAndAcess should return exactly the same type that the expression c[i] returns. therefore we can use decltype(auto): auto specifies that the type is to be deduced, and decltype says that decltype rules should be used during the deduction. 

```cpp
template<typename Container, typename Index> // C++14; works, but requires refinement
decltype(auto) authAndAccess(Container& c, Index i) 
{
 authenticateUser();
 return c[i];
}
```

The refinement needed is because the container is passed by lvalue-reference-to-non-const (i.e. int& from deque example), but this means it's not possible to pass rvalue containers to this function. Rvalues can't bind to lvalue references (unless they're lvalue-references-to-const), which is not the case here.

An rvalue container, being a temporary object, would typically be destroyed at the end of the statement containing the call to authAndAcesss, and that means that a reference to an element in that container (which is typically what authAndAccess would return) would dangle at the end of the statement that created it. Still, it could make sense to pass a temporary object to authAndAccess. A client might want simply to make a copy of an element in the temporary container.

```cpp
std::deque<std::string> makeStringDeque(); // factory function

auto s = authAndAccess(makeStringDeque(), 5); // make copy of 5th element of deque returned from makeStringDeque
```

To support this authAndAccess needs to support both lvalues and rvalues, overloading for each would mean that we would have two functions to maintain (one overload would declare an lvalue reference parameter, the other an rvalue reference parameter). A way to avoid this is to have authAndAccess use a reference parameter that can bind to lvalues and rvalues (using a universal reference).

```cpp
template<typename Container, typename Index>
decltype(auto) authAndAccess(Container&& c, Index i); // c is now a Universal Reference
{
 authenticateUser();
 return std::forward<Container>(c)[i];
}
```

In this template, we don’t know what type of container we’re operating on, and that
means we’re equally ignorant of the type of index objects it uses. However, we need to update the template’s implementation to bring it into accord with Item 25’s admonition to apply std::forward to universal references.

### Things to Remember 

1. decltype almost always yields the type of a variable or expression without
any modifications.
2. For lvalue expressions of type T other than names, decltype always reports a
type of T&.
3. C++14 supports decltype(auto), which, like auto, deduces a type from its
initializer, but it performs the type deduction using the decltype rules

## Item 4: Know how to view deduced types.

We’ll explore three possibilities: getting type deduction information as you edit your code, getting it during compilation, and getting it at runtime.

### IDE Editors

If you hover your mouse over the variable the IDE editor will likely show u a variable or functions deduced type. For simple types like int, information from IDEs is generally fine, when more complicated types are involved, the information displayed by IDEs may not be particularly helpful.

### Compiler Diagnostics

An effective way to get a compiler to show a type it has deduced is to use that type in
a way that leads to compilation problems. The error message reporting the problem
is virtually sure to mention the type that’s causing it. 

```cpp
template<typename T> // declaration only for TD;
class TD; // TD == "Type Displayer"

TD<decltype(x)> xType; // elicit errors containing
TD<decltype(y)> yType; // x's and y's types
```

### Runtime Output

The printf approach to displaying type information offers full control over the formatting of the output. 

```cpp
std::cout << typeid(x).name() << '\n'; // display types for
std::cout << typeid(y).name() << '\n'; // x and y
```

### Things to Remember

1. Deduced types can often be seen using IDE editors, compiler error messages, and the Boost TypeIndex library.
2. The results of some tools may be neither helpful nor accurate, so an under‐standing of C++’s type deduction rules remains essential.

# Chapter 2: auto

This brief chapter covers auto's ins and outs.

## Item 5: Prefer auto to explicit type declarations

auto variables have their type deduced from their initializer, so they must be initialized. That means you can avoid a host of unitalized variable problems by utilizing it. 

```cpp
int x1; //potentially uninitialized

auto x2; // error! initializer required

auto x3 = 0; //fine, x's value is well-defined
```

Because auto uses type deduction (Item 2), it can represent types known only to compilers and iterator types: 

```cpp
template<typename It> //algorithm to "do what i mean"
void dwim(It b, It e) //for al elements in range from b to e
{
    while(b != e){
        auto currValue = *b; //dereferencing iterator value type
        ...
    }
}

auto derefUPLess = [](const std::unique_ptr<Widget>& p1, const std::unique_ptr<Widget>& p2)
{ return *p1 < *p2; } // comparison func for widget smart pointers

//which can be simplified to in C++14: a comparison function for values pointed to by anything pointer-like
auto derefLess = [](const auto& p1, const auto& p2) { return *p1 < *p2; }
```

Technically, we don't really need auto to declare a variable that holds a closure, because we can use a std::function object. 

std::function is a template in the C++11 Standard Library that generalizes the idea of a function pointer (where std::function objects can refer to any callable object). 

For example, to declare a std::function object named func that could refer to any callable object acting as if it had this signature, 

```cpp
//C++11 signature for unique ptr comparison function
bool(const std::unique_ptr<Widget>&, const std::unique_ptr<Widget>&) 
```

you could write:

```cpp
std::function<bool(const std::unique_ptr<Widget>&, const std::unique_ptr<Widget>&)> func;
``` 

And since lambda expressions yield callable objects, closures can be stored in std::function objects. That means we could declare this:

```cpp
std::function<bool(const std::unique_ptr<Widget>&, 
                   const std::unique_ptr<Widget>&)> 
derefUPLess = [](const std::unique_ptr<Widget>& p1, 
                 const std::unique_ptr<Widget>& p2) 
                 { return *p1 < *p2; }
```

It is important to note that using std::function is not the same as using auto. An auto-declared variable holding a closure has the same type as the closure, and as such it only uses as much memory as the closure requires.

The type of a std::function declared variable holding a closure is an instantiation of the std::function template, and has a fixed size for any given signature. When we need more space, the std::function constructor will allocate heap memory to store the closure. 

The result is that the std::function object typically uses more memory than the auto-declared object. (also behind the scenes due to implementation details that restrict inlining and yield indirect function calls, invoking a closure via std::function is almost certain to be slower than calling it via an auto declared object). 

The moral of the story is that std::function approach is generally bigger and slower than the auto approach, and it might yield out-of-memory exceptions. 

```cpp
std::unordered_map<std::string, int> m;
…
for (const std::pair<std::string, int>& p : m)
{
 … // do something with p
}
```

Remember that the key part of a std::unordered_map is const, so the type of std::pair in the hash table (which is what a std::unordered_map is) isn’t std::pair<std::string, int>, it’s std::pair <const std::string, int>. But that’s not the type declared for the variable p in the loop above. As a result, compilers will strive to find a way to convert objects (i.e., what’s in the hash table) to the declared type for p. They’ll succeed by
creating a temporary object of the type that p wants to bind to by copying each object in m, then binding the reference p to that temporary object. At the end of each loop iteration, the temporary object will be destroyed.

To avoid unintentional type mismatches and inefficient loops we can simply:

```cpp
for (const auto& p : m){
    ...
}
```

As an added bonus: if you take p's address, you will get a pointer to an element within m. In a code not using auto, you'd get a pointer to a temporary object that would have to be destroyed at the end of the loop iteration. 

Yet auto isn't perfect. The type for each auto variable is deduced from its initalizing expression, and some initializing expressions have types that are neither anticipated nor desired. 

### Things to Remember 

1. auto variables must be initialized, are generally immune to type mismatches that can lead to portability or efficiency problems, can ease the process of refactoring, and typically require less typing than variables with explicitly specified types.
2. auto-typed variables are subject to the pitfalls described in Items 2 and 6.


## Item 6: Use the explicitly typed initializer idom when auto deduces undesired types. 

suppose I have a function that takes a Widget and returns a std::vector<bool>, where each bool indicates whether the Widget offers a particular feature:

```cpp
std::vector<bool> features(const Widget& w);

Widget w;

bool highPriority = features(w)[5]; // is w high priority?

processWidget(w, highPriority); // process w in accord with its priority

auto highPriority = features(w)[5]; // is w high priority?

processWidget(w, highPriority); // undefined behavior! The code will still compile, but the behavior is no longer predictable
``` 

Here:
1. features returns a std::vector<bool> object
2. on which operator[] is invoked
3. operator[] returns a std::vector<bool>::reference object
4. std::vector<bool>::reference has a feature to implicitly convert to bool (not bool& -> bool)

When this cast is done with auto, the result is highPriority containing a danging pointer. This is because std::vector<bool>::reference is an example of a `proxy class`: a class that exists for the purpose of emulating and augmenting the behavior of some other type. As a general rule, "invisible" proxy classes don't play well with auto. Objects of such classes are often not designed to live longer than a single statement, so creating variables of those types tends to violate fundamental library design assumptions. 

The solution is to force a different type deduction using the `the explicitly typed initializer idiom`. The explicitly typed initializer idiom involves declaring a variable with auto, but casting the initialization expression to the type you want auto to deduce. Using our example:

```cpp
auto highPriority = static_cast<bool>(features(w)[5]);\
```

### Things to Remember 

1. “Invisible” proxy types can cause auto to deduce the “wrong” type for an ini‐
tializing expression.
2. The explicitly typed initializer idiom forces auto to deduce the type you want
it to have.

# Chapter 3: Moving to Modern C++

