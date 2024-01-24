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

This chapter helps answer the questions of: When should you use braces instead of parentheses for object creation? Why are alias declarations better than typedefs? How does constexpr differ from const? What's the relationship between const member functions and thread safety?

## Item 7: Distinguish between () and {} when creating objects. 

Using braces, specifying the initial contents of a container & default initialization values for non-static data members is easy:
```cpp
std::vector<int> v{ 1, 3, 5 }; // v's initial content is 1, 3, 5

class Widget {
 …
private:
 int x{ 0 }; // fine, x's default value is 0
 int y = 0; // also fine
 int z(0); // error!

 std::atomic<int> ai1{ 0 }; // fine
 std::atomic<int> ai2(0); // fine
 std::atomic<int> ai3 = 0; // error!
};
```

### Things to Remember

1. Braced initialization is the most widely usable initialization syntax, it prevents narrowing conversions, and it's immune to C++'s most vexing parse (Widget w2() declares a function named w2 that returns a Widget).
2. During constructor overload resolution, braced initializers are matched to std::initializer_list parameters if at all possibnle, even if other constructors offer seemingly better matches. 
3. An example of where the choice between parentheses and braces can make a significant difference is creating a std::vector<`numeric type`> with two arguments. 
4. Choosing between parentheses and braces for object creation inside templates can be challenging. 

## Item 8: Prefer nullptr to 0 and NULL

The literal 0 is an int, not a pointer. If C++ finds itself looking at 0 in a context where only a pointer can be used, it will interpret 0 as a null pointer as a fallback position. The same is true of NULL. There is some uncertainty in the details
in NULL’s case, because implementations are allowed to give NULL an integral type other than int (e.g., long). That’s not common, but it doesn’t really matter, because the issue here isn’t the exact type of NULL, it’s that neither 0 nor NULL has a pointer type.

in C++98 passing 0 or NULL to such overloads never called a pointer overload:

```cpp
void f(int); // three overloads of f
void f(bool);
void f(void*);

f(0); // calls f(int), not f(void*)
f(NULL); // might not compile, but typically calls f(int). Never calls f(void*)
```

nullptr’s advantage is that it doesn’t have an integral type. It doesn’t have a pointer type, but you can think of it as a pointer of all types. nullptr’s actual type is std::nullptr_t, and circular definition, std::nullptr_t is defined to be the type of nullptr. The type std::nullptr_t implicitly converts to all raw pointer types, and that’s what makes nullptr act as if it were a pointer of all types.

Calling the overloaded function f with nullptr calls the void* overload (i.e., the pointer overload), because nullptr can’t be viewed as anything integral:

```cpp
f(nullptr); // calls f(void*) overload
```

Using nullptr instead of 0 or NULL thus avoids overload resolution surprises, but that’s not its only advantage. It can also improve code clarity, especially when auto variables are involved.

### Things to Remember
1. Prefer nullptr to 0 and NULL.
2. Avoid overloading on integral and pointer types.

## Item 9: Prefer alias declarations to typedefs

To avoid writing types like `std::unique_ptr<std::unordered_map<std::string, std::string>>` more than once, we can use typedefs. 

```cpp
typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSS;
 ```

 typedefs are traditionally used in C++98, they work in C++11 but C++11 offers alias declarations:
 ```cpp
 using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;
```

Given that the typedef and the alias declaration do exactly the same thing, it’s reasonable to wonder whether there is a solid technical reason for preferring one over the other. But a compelling reason does exist: templates. In particular, alias declarations may be templatized (in which case they’re called alias templates), while typedefs cannot.

```cpp
template<typename T> // MyAllocList<T>
using MyAllocList = std::list<T, MyAlloc<T>>; // is synonym for std::list<T, MyAlloc<T>>
MyAllocList<Widget> lw; // client code

//with typedef:
template<typename T> // MyAllocList<T>::type
struct MyAllocList {
 typedef std::list<T, MyAlloc<T>> type; // is synonym for std::list<T, MyAlloc<T>>
};
MyAllocList<Widget>::type lw; // client code
```

If you’ve done any template metaprogramming (TMP), you’ve almost certainly bumped up against the need to take template type parameters and create revised types from them. For example, given some type T, you might want to strip off any constor reference-qualifiers that T contains, e.g., you might want to turn const std::string& into std::string. Or you might want to add const to a type or turn it into an lvalue reference, e.g., turn Widget into const Widget or into Widget&.

C++11 gives you the tools to perform these kinds of transformations in the form of type traits, an assortment of templates inside the header <type_traits>. There are dozens of type traits in that header, and not all of them perform type transformations, but the ones that do offer a predictable interface. Given a type T to which you’d like to apply a transformation, the resulting type is std::transformation<T>::type. For example:
```cpp
std::remove_const<T>::type // yields T from const T
std::remove_const_t<T> // C++14 equivalent const T → T

std::remove_reference<T>::type // yields T from T& and T&&
std::remove_reference_t<T> // C++14 equivalent T&/T&& → T

std::add_lvalue_reference<T>::type // yields T& from T
std::add_lvalue_reference_t<T> // C++14 equivalent T → T&

template <class T>
using remove_const_t = typename remove_const<T>::type;

template <class T>
using remove_reference_t = typename remove_reference<T>::type;

template <class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
```

### Things to remember

1. typedefs don't support templatization, but alias declarations do.
2. Alias templates avoid the "::type" suffix and, in templates, the "typename" prefix often required to refer to typedefs.
3. C++14 offers alias templates for all the C++11 type traits transformations.

## Item 10: Prefer scoped enums to unscoped enums

As a general rule, declaring a name inside curly braces limits the visibility of that
name to the scope defined by the braces.

This is not the case for unscoped C++98-style enums. The names of such enumerators belong to the scope containing the enum, meaining that nothing else in that scope may have the same name:

```cpp
enum Color { black, white, red }; // black, white, red are in same scope as Color
auto white = false; // error! white already declared in this scope
```

Their new C++11 counterparts, scoped enums, don’t leak names in this way:

```cpp
enum class Color { black, white, red }; // black, white, red are scoped to Color
auto white = false; // fine, no other "white" in scope
Color c = white; // error! no enumerator named "white" is in this scope
Color c = Color::white; // fine
auto c = Color::white; // also fine (and in accord with Item 5's advice)
```

Because scoped enums are declared via “enum class”, they’re sometimes referred to as enum classes. Scoped enums have a second compelling advantage: their enumerators are much more strongly typed. Enumerators for unscoped enums implicitly convert to integral types (and, from there, to floating-point types).

```cpp
enum Color { black, white, red }; // unscoped enum
std::vector<std::size_t> // func. returning
 primeFactors(std::size_t x); // prime factors of x
Color c = red;
…
if (c < 14.5) { // compare Color to double (!)
 auto factors = // compute prime factors
 primeFactors(c); // of a Color (!)
 …
}

enum class Color { black, white, red }; // enum is now scoped
Color c = Color::red; // as before, but with scope qualifier
… 
if (c < 14.5) { // error! can't compare Color and double
auto factors = // error! can't pass Color to
 primeFactors(c); // function expecting std::size_t
 …
}
```

### Things to Remember

1. C++98-style enums are now known as unscoped enums.
2. Enumerators of scoped enums are visible only within the enum. They convert to other types only with a cast.
3. Both scoped and unscoped enums support specification of the underlying type. The default underlying type for scoped enums is int. Unscoped enums have no default underlying type.
4. Scoped enums may always be forward-declared. Unscoped enums may be forward-declared only if their declaration specifies an underlying type.

## Item 11: Prefer deleted functions to private undefined ones

If you want to prevent them from calling a particular function, you generally just don’t declare the function. No function declaration, no function to call. But sometimes C++ declares functions for you, and if you want to prevent clients from calling those functions things become more complicated. 

The situation arises only for the “special member functions,” i.e., the member functions that C++ automatically generates when they’re needed. Item 17 discusses these functions in detail, but for now, we’ll worry only about the copy constructor and the copy assignment operator.

The C++98 approach to preventing use of these functions is to declare them private
and not define them:

```cpp
template <class charT, class traits = char_traits<charT> >
class basic_ios : public ios_base {
public:
 …
private:
 basic_ios(const basic_ios& ); // not defined
 basic_ios& operator=(const basic_ios&); // not defined
};
```

in C++11, there's a better way to achieve essentially the same end: "= delete" to mark the copy constructor and the copy assignment operator as deleted functions. 

```cpp
template <class charT, class traits = char_traits<charT>>
class basic_ios : public ios_base {
public:
 …
 basic_ios(const basic_ios& ) = delete;
 basic_ios& operator=(const basic_ios&) = delete;
 …
};
```

Deleted functions may not be used in any way, so even code that’s in member and friend functions will fail to compile if it tries to copy basic_ios objects. That’s an improvement over the C++98 behavior, where such improper usage wouldn’t be diagnosed until link-time. An important advantage of deleted functions is that any function may be deleted, while only member functions may be private.

One way to accomplish that is to create deleted overloads for the types we want to filter out:

```cpp
bool isLucky(int number); // original function
bool isLucky(char) = delete; // reject chars
bool isLucky(bool) = delete; // reject bools
bool isLucky(double) = delete; // reject doubles and floats
```

### Things to Remember

1. Prefer deleted functions to private undefined ones.
2. Any function may be deleted, including non-member functions and template instantiations.

## Item 12: Declare overriding functions override

The world of object-oriented programming in C++ revolves around classes, inheritance, and virtual functions. Among the most fundamental ideas in this world is that virtual function implementations in derived classes override the implementations of their base class counterparts.

```cpp
class Base {
public:
 virtual void doWork(); // base class virtual function
 …
};
class Derived: public Base {
public:
 virtual void doWork(); // overrides Base::doWork
 … // ("virtual" is optional here
}; 
std::unique_ptr<Base> upb = // create base class pointer
std::make_unique<Derived>(); // to derived class object; see Item 21 for info on std::make_unique
upb->doWork(); // call doWork through base class ptr; derived class function is invoked
```

For overriding to occur, severa requirements must be met:

- The base class function must be virtual.
- The base and derived function names must be identical (except in the case of destructors). 
- The parameter types of the base and derived functions must be identical.
- The constness of the base and derived functions must be identical.
- The return types and exception specifications of the base and derived functions must be compatible.
- The functions’ reference qualifiers must be identical. Member function reference qualifiers are one of C++11’s less-publicized features, so don’t be surprised if you’ve never heard of them. They make it possible to limit use of a member function to lvalues only or to rvalues only. Member functions need not be virtual to use them:

```cpp
class Widget {
public:
 …
 void doWork() &; // this version of doWork applies only when *this is an lvalue
 void doWork() &&; // this version of doWork applies only when *this is an rvalue
};
…
Widget makeWidget(); // factory function (returns rvalue)
Widget w; // normal object (an lvalue)
…
w.doWork(); // calls Widget::doWork for lvalues (i.e., Widget::doWork &)
makeWidget().doWork(); // calls Widget::doWork for rvalues (i.e., Widget::doWork &&)
```

```cpp

class Base {
public:
 virtual void mf1() const;
 virtual void mf2(int x);
 virtual void mf3() &;
 void mf4() const;
};
class Derived: public Base {
public:
 virtual void mf1(); //warning! mf1 is declared const in Base, but not in Derived.
 virtual void mf2(unsigned int x); //warning! mf2 takes an int in Base, but an unsigned int in Derived.
 virtual void mf3() &&; //warning! mf3 is lvalue-qualified in Base, but rvalue-qualified in Derived.
 void mf4() const; //warning! mf4 isn’t declared virtual in Base.
};
```

The correct version is:

```cpp
class Base {
public:
 virtual void mf1() const;
 virtual void mf2(int x);
 virtual void mf3() &;
 virtual void mf4() const;
};
class Derived: public Base {
public:
 virtual void mf1() const override;
 virtual void mf2(int x) override;
 virtual void mf3() & override;
 void mf4() const override; // adding "virtual" is OK, but not necessary
};
```

### Things to Remember

1. Declare overriding functions override.
2. Member function reference qualifiers make it possible to treat lvalue and rvalue objects (*this) differently

## Item 13: Prefer const_iterators to iterators

Const_iterators are the STL equivalent of pointers-to-const. They point to values that may not be modified. The standard practice of using const whenever possible dictates that you should use const_iterators any time you need an iterator, yet have no need to modify what the iterator points to.

The container member functions cbegin and cend produce const_iterators, even for non-const containers, and STL member functions that use iterators to identify positions (e.g., insert and erase) actually use const_iterators. 

```cpp
std::vector<int> values; // as before
…
auto it = std::find(values.cbegin(),values.cend(), 1983); // use cbegin and cend

values.insert(it, 1998);
``` 

About the only situation in which C++11’s support for const_iterators comes up a bit short is when you want to write maximally generic library code. Such code takes into account that some containers and container-like data structures offer begin and end (plus cbegin, cend, rbegin, etc.) as non-member functions, rather than members

For example, we could generalize the code we’ve been working with into a findAndInsert template as follows:
```cpp
template<typename C, typename V>
void findAndInsert(C& container, // in container, find
 const V& targetVal, // first occurrence
 const V& insertVal) // of targetVal, then
{ // insert insertVal
 using std::cbegin; // there
 using std::cend;
 auto it = std::find(cbegin(container), // non-member cbegin
 cend(container), // non-member cend
 targetVal);
 container.insert(it, insertVal);
}
```

This works fine in C++14, but, sadly, not in C++11. Through an oversight during standardization, C++11 added the non-member functions begin and end, but it failed to add cbegin, cend, rbegin, rend, crbegin, and crend. C++14 rectifies that oversight.

### Things to Remember

1. Prefer const_iterators to iterators.
2. In maximally generic code, prefer non-member versions of begin, end, rbegin, etc., over their member function counterparts

## Item 14: Declare functions noexcept if they won't emit exceptions
During work on C++11, a consensus emerged that the truly meaningful information
about a function’s exception-emitting behavior was whether it had any. There’s an additional incentive to apply noexcept to functions that won’t produce exceptions: it permits compilers to generate better object code.

The difference between unwinding the call stack and possibly unwinding it has a surprisingly large impact on code generation. 

In a noexcept function, optimizers dont need to keep the runtime stack in an unwindable state if an exception would propagate out of the function, nor must they ensure that objects in a noexcept function are destroyed in the inverse order of construction should an exception leave the function.

Functions with “throw()” exception specifications lack such optimization flexibility, as do functions with no exception specification at all. The situation can be summarized this way:
```cpp
RetType function(params) noexcept; // most optimizable
RetType function(params) throw(); // less optimizable
RetType function(params); // less optimizable
```
This alone is sufficient reason to declare functions noexcept whenever you know they won’t produce
exceptions.

Swap functions comprise another case where noexcept is particularly desirable. swap is a key component of many STL algorithm implementations, and it’s commonly employed in copy assignment operators, too. 

For example, the declarations for the Standard Library’s swaps for arrays and std::pair are:
```cpp
template <class T, size_t N>
void swap(T (&a)[N], // see
 T (&b)[N]) noexcept(noexcept(swap(*a, *b))); // below
template <class T1, class T2>
struct pair {
 …
 void swap(pair& p) noexcept(noexcept(swap(first, p.first)) && noexcept(swap(second, p.second)));
 …
};
```
These functions are conditionally noexcept: whether they are noexcept depends on whether the expressions inside the noexcept clauses are noexcept. 

Optimization is important, but correctness is more important. noexcept is part of a function’s interface, so you should declare a function noexcept only if you are willing to commit to a noexcept implementation over the long term. If you declare a function noexcept and later regret that decision, your options are bleak. You can remove noexcept from the function’s declaration (i.e., change its interface),
thus running the risk of breaking client code. 

### Things to remember

1. noexcept is part of a function's interface, and that means that callers may depend on it. 
2. noexcept functions are more optimizable than non-noexcept functions.
3. noexcept is particularly valueable for the move operations, swap, memory deallocation functions, and destructors. 
4. Most functions are exception-neutral rather than noexcept

## Item 15: Use constexpr whenever possible

constexpr objects are const objects that have values that are known at compile time. Values known during compilation are privileged. They may be placed in read-only memory, for example, and especially for developers of embedded systems, this can be a very important. The benefit of trying to move computation from runtime to compile time can result in optimized processes, as you only have to "do the work" upfront at compile time once.

It is important to note that marking something constexpr does not guarantee that it will be evaluated at compile time, it's more of a nudge to the compiler that it can be. 

Simply put, all constexpr objects are const, but not all const objects are constexpr. If you want compilers to guarantee that a variable has a value that can be used in contexts requiring compile-time constants, the tool to reach for is constexpr, not const.

Usage scenarios for constexpr objects become more interesting when constexpr functions are involved. Such functions produce compile-time constants when they are called with compile-time constants. If they’re called with values not known until runtime, they produce runtime values. This may sound as if you don’t know what they’ll do, but that’s the wrong way to think about it. The right way to view it is this:
- constexpr functions can be used in contexts that demand compile-time constants. If the values of the arguments you pass to a constexpr function in such a context are known during compilation, the result will be computed during compilation. If any of the arguments’ values is not known during compilation, your code will be rejected.
- When a constexpr function is called with one or more values that are not known during compilation, it acts like a normal function, computing its result at runtime. This means you don’t need two functions to perform the same operation, one for compile-time constants and one for all other values. The constexpr
function does it all.

```cpp
constexpr // pow's a constexpr func
int pow(int base, int exp) noexcept // that never throws
{
 … // impl is below
}
constexpr auto numConds = 5; // # of conditions
std::array<int, pow(3, numConds)> results; // results has
 // 3^numConds
// elements

auto base = readFromDB("base"); // get these values
auto exp = readFromDB("exponent"); // at runtime
auto baseToExp = pow(base, exp); // call pow function at runtime
...

// return reflection of p with respect to the origin (C++14)
constexpr Point reflection(const Point& p) noexcept
{
 Point result; // create non-const Point
 result.setX(-p.xValue()); // set its x and y values
 result.setY(-p.yValue());
 return result; // return copy of it
}

constexpr Point p1(9.4, 27.7); // as above
constexpr Point p2(28.8, 5.3);
constexpr auto mid = midpoint(p1, p2);
constexpr auto reflectedMid = reflection(mid); // reflectedMid's value is (-19.1 -16.5) and known during compilation
```

### Things to Remember

1. constexpr objects are const and are initialized with values known during compilation
2. constexpr functions can produce compile-time results when called with arguments whose values are known during compilation.
3.  constexpr objects and functions may be used in a wider range of contexts than non-constexpr objects and functions.
4. constexpr is part of an object’s or function’s interface.

## Item 16: Make const member functions thread safe

Lets say we're working in a mathematical domain where we are developing a class to represent polynomials. Within the class we're going to need a function to compute the root(s) of a polynomial (i.e. values where the polynomial evaluates to 0). Such a function would not modify the polynomial so it would be natural to declare it const:

```cpp
class Polynomial {
public:
// data structure holding values where polynomial evals to zero (see Item 9 for info on "using")
 using RootsType = std::vector<double>; 
 … 
 RootsType roots() const;
 …
};
```

Computing the roots of a polynomial can be expensive, so we don’t want to do it if we don’t have to. And if we do have to do it, we certainly don’t want to do it more than once. We’ll thus cache the root(s) of the polynomial if we have to compute them, and we’ll implement roots to return the cached value. 

```cpp
class Polynomial {
public:
 using RootsType = std::vector<double>;
 RootsType roots() const
 {
 if (!rootsAreValid) { // if cache not valid
 … // compute roots,
 // store them in rootVals
 rootsAreValid = true;
 }
 return rootVals;
 }
private:
 mutable bool rootsAreValid{ false }; // see Item 7 for info
 mutable RootsType rootVals{}; // on initializers
};
```

Conceptually, roots doesn’t change the Polynomial object on which it operates, but, as part of its caching activity, it may need to modify rootVals and rootsAreValid. That’s a classic use case for mutable, and that’s why it’s part of the declarations for these data members. 

The problem arises when two threads simulataneously call `roots` on a Polynomial object. Even though the function is marked const, one or both of these threads might try to modify the data members rootsAreValid and rootVals. Meaning that this code could have different threads reading and writing the same memory without synchronization. (Thats the definition of a data race. This code has undefined behavior)

The easiest way to address the issue is the usual one: employ a mutex:
```cpp
class Polynomial {
public:
 using RootsType = std::vector<double>;
 RootsType roots() const
 {
 std::lock_guard<std::mutex> g(m); // lock mutex
 if (!rootsAreValid) { // if cache not valid
 … // compute/store roots
 rootsAreValid = true;
 }
 return rootVals;
 } // unlock mutex
private:
 mutable std::mutex m;
 mutable bool rootsAreValid{ false };
 mutable RootsType rootVals{};
};
```

It’s worth noting that because std::mutex is a move-only type (i.e., a type that can be moved, but not copied), a side effect of adding m to Polynomial is that Polynomial loses the ability to be copied. It can still be moved, however. 

In some situations, a mutex is overkill. For example, if all you’re doing is counting
how many times a member function is called, a std::atomic counter (i.e, one where
other threads are guaranteed to see its operations occur indivisibly—see Item 40) will
often be a less expensive way to go. 

### Things to Remember

1. Make const member functions thread safe unless you’re certain they’ll never be used in a concurrent context.
2. Use of std::atomic variables may offer better performance than a mutex, but they’re suited for manipulation of only a single variable or memory location.

## Item 17: Understand special member function generation

The special member functions are the ones that C++ is willing to generate on its own. C++98 has four such functions: the default constructor, the destructor, the copy constructor, and the copy assignment operator. 

These functions are generated only if they’re needed, i.e., if some code uses them without their being expressly declared in the class. A default constructor is generated only if the class declares no constructors at all. 

As of C++11, the special member functions club has two more inductees: the move constructor and the move assignment operator. Their signatures are:

```cpp
class Widget {
public:
 …
 Widget(Widget&& rhs); // move constructor
 Widget& operator=(Widget&& rhs); // move assignment operator
 …
};
```

Move operations are generated for classses (when needed) only if these three things are true:
- No copy operations are declared in the class.
- No move operations are declared in the class. 
- No destructor is declared in the class. 

At some point, analogous rules may be extended to the copy operations, because
C++11 deprecates the automatic generation of copy operations for classes declaring
copy operations or a destructor. 

Provided the behavior of the compiler-generated functions is correct (i.e, if memberwise copying of the class’s non-static data members is what you want), your job is easy, because C++11’s “= default” lets you say that explicitly:

```cpp
class Widget {
public:
 …
 ~Widget(); // user-declared dtor
 … // default copy ctor
 Widget(const Widget&) = default; // behavior is OK
 Widget& // default copy assign
    operator=(const Widget&) = default; // behavior is OK
 …
};
```
This approach is often useful in polymorphic base classes, i.e., classes defining interfaces through which derived class objects are manipulated. Declaring the move operations disables the copy operations, so if copyability is also desired, one more round of “= default” does the job:

```cpp
class Base {
public:
 virtual ~Base() = default; // make dtor virtual
 Base(Base&&) = default; // support moving
 Base& operator=(Base&&) = default;
 Base(const Base&) = default; // support copying
 Base& operator=(const Base&) = default;
 …
};
```

The C++11 rules governing the special member functions are thus:
- Default constructor: Same rules as C++98. Generated only if the class contains no user-declared constructors.
- Destructor: Essentially same rules as C++98; sole difference is that destructors are noexcept by default (see Item 14). As in C++98, virtual only if a base class destructor is virtual.
- Copy constructor: Same runtime behavior as C++98: memberwise copy construction of non-static data members. Generated only if the class lacks a userdeclared copy constructor. Deleted if the class declares a move operation. Generation of this function in a class with a user-declared copy assignment operator or destructor is deprecated.
- Copy assignment operator: Same runtime behavior as C++98: memberwise copy assignment of non-static data members. Generated only if the class lacks a user-declared copy assignment operator. Deleted if the class declares a move operation. Generation of this function in a class with a user-declared copy constructor or destructor is deprecated.
- Move constructor and move assignment operator: Each performs memberwise moving of non-static data members. Generated only if the class contains no userdeclared copy operations, move operations, or destructor. Note that there’s nothing in the rules about the existence of a member function template preventing compilers from generating the special member functions. That means that if Widget looks like this:

```cpp
class Widget {
 …
 template<typename T> // construct Widget
 Widget(const T& rhs); // from anything
 template<typename T> // assign Widget
 Widget& operator=(const T& rhs); // from anything
 …
};
```

### Things to Remember

1. The special member functions are those compilers may generate on their own: default constructor, destructor, copy operations, and move operations.
2. Move operations are generated only for classes lacking explicitly declared move operations, copy operations, and a destructor
3. The copy constructor is generated only for classes lacking an explicitly declared copy constructor, and it’s deleted if a move operation is declared. The copy assignment operator is generated only for classes lacking an explicitly declared copy assignment operator, and it’s deleted if a move operation is declared. Generation of the copy operations in classes with an explicitly declared destructor is deprecated.
4. Member function templates never suppress generation of special member functions.

# Chapter 4: Smart Pointers

Flaws with raw pointers:

1. Its declaration doesn't indicate whether it points to a single object or to an array.
2. Its declaration reveals nothing about whether you should destroy what it points to when you're done using it.
3. If you determine that you should destroy what the pointer points to, there's no way to tell how. (delete, or diff destruction mechanism??? i.e. dedicated destruction function)
4. If we know we are using delete, should we use "delete" or "delete[]"? Using the wrong form results in undefined behavior.
5. If we know that the pointer owns what it points to and you discover how to destroy it, it’s difficult to ensure that you perform the destruction exactly once along every path in your code (including those due to exceptions). Missing a path leads to resource leaks, and doing the destruction more than once leads to undefined behavior.
6. There’s typically no way to tell if the pointer dangles, i.e., points to memory that no longer holds the object the pointer is supposed to point to. Dangling pointers arise when objects are destroyed while pointers still point to them.

Smart pointers are wrappers around raw pointers that act much like the raw pointers they wrap, but that avoid many of their pitfalls. Smart pointers can do virtually everything raw pointers can, but with far fewer opportunities for error.

There are four smart pointers in C++11: std::auto_ptr, std::unique_ptr, std::shared_ptr, and std::weak_ptr.

std::auto_ptr is a deprecated leftover from C++98. It was an attempt to standardize what later became C++11’s std::unique_ptr.

std::unique_ptr does everything std::auto_ptr does, plus more. It does it as efficiently, and it does it without warping what it means to copy an object. It’s better than std::auto_ptr in every way. The only legitimate use case for std::auto_ptr is a need to compile code with C++98 compilers. Unless you have that constraint, you should replace std::auto_ptr with std::unique_ptr.

The smart pointer APIs are remarkably varied. About the only functionality common
to all is default construction. Comprehensive references for these APIs are
widely available, 

## Item 18: Use std::unique_ptr for exclusive-ownership resource management

When you reach for a smart pointer, std::unique_ptr should generally be the one closest at hand. It’s reasonable to assume that, by default, std::unique_ptrs are the same size as raw pointers, and for most operations (including dereferencing), they execute exactly the same instructions. This means you can use them even in situations where memory and cycles are tight. If a raw pointer is small enough and fast enough for you, a std::unique_ptr almost certainly is, too.

std::unique_ptr embodies exclusive ownership semantics. A non-null std::unique_ptr always owns what it points to. Moving a std::unique_ptr transfers ownership from the source pointer to the destination pointer. (The source pointer is set to null.) Copying a std::unique_ptr isn’t allowed, because if you could copy a std::unique_ptr, you’d end up with two std::unique_ptrs to the same resource, each thinking it owned (and should therefore destroy) that resource. std::unique_ptr is thus a move-only type. Upon destruction, a non-null std::unique_ptr destroys its resource. By default, resource destruction is accomplished by applying delete to the raw pointer inside the std::unique_ptr.

By default, destruction would take place via delete, but, during construction, std::unique_ptr objects can be configured to use custom deleters:

```cpp
auto delInvmt = [](Investment* pInvestment) { // custom deleter
 makeLogEntry(pInvestment); // (a lambda expression)
 delete pInvestment; 
};
template<typename... Ts> // revised
std::unique_ptr<Investment, decltype(delInvmt)> // return type
makeInvestment(Ts&&... params)
{
 std::unique_ptr<Investment, decltype(delInvmt)> // ptr to be
 pInv(nullptr, delInvmt); // returned

 if ( /* a Stock object should be created */ )
 {
 pInv.reset(new Stock(std::forward<Ts>(params)...));
 }
 else if ( /* a Bond object should be created */ )
 {
 pInv.reset(new Bond(std::forward<Ts>(params)...));
 }
 else if ( /* a RealEstate object should be created */ )
 {
 pInv.reset(new RealEstate(std::forward<Ts>(params)...));
 }
 return pInv;
}
```

- delInvmt is the custom deleter for the object returned from makeInvestment. All custom deletion functions accept a raw pointer to the object to be destroyed, then do what is necessary to destroy that object. In this case, the action is to call makeLogEntry and then apply delete. Using a lambda expression to create delInvmt is convenient, but, as we’ll see shortly, it’s also more efficient than writing a conventional function.
- When a custom deleter is to be used, its type must be specified as the second type argument to std::unique_ptr. In this case, that’s the type of delInvmt, and that’s why the return type of makeInvestment is std::unique_ptr<Investment, decltype(delInvmt)>. (For information about decltype, see Item 3.)
- The basic strategy of makeInvestment is to create a null std::unique_ptr, make it point to an object of the appropriate type, and then return it. To associate the custom deleter delInvmt with pInv, we pass that as its second constructor argument.
- Attempting to assign a raw pointer (e.g., from new) to a std::unique_ptr won’t compile, because it would constitute an implicit conversion from a raw to a smart pointer. Such implicit conversions can be problematic, so C++11’s smartpointers prohibit them. That’s why reset is used to have pInv assume ownership of the object created via new.
- With each use of new, we use std::forward to perfect-forward the arguments passed to makeInvestment (see Item 25). This makes all the information provided by callers available to the constructors of the objects being created.
- The custom deleter takes a parameter of type Investment*. Regardless of the actual type of object created inside makeInvestment (i.e., Stock, Bond, or Real Estate), it will ultimately be deleted inside the lambda expression as an Investment* object. This means we’ll be deleting a derived class object via a base class pointer. For that to work, the base class—Investment—must have a virtual destructor:
```cpp
class Investment {
public:
 … // essential
 virtual ~Investment(); // design
 … // component!
};```

In C++14, the existence of function return type deduction (see Item 3) means that makeInvestment could be implemented in this simpler and more encapsulated fashion:

```cpp
template<typename... Ts>
auto makeInvestment(Ts&&... params) // C++14
{
 auto delInvmt = [](Investment* pInvestment) // this is now
 { // this is now inside
 makeLogEntry(pInvestment); // make-Investment
 delete pInvestment;
 };
 std::unique_ptr<Investment, decltype(delInvmt)> // as
 pInv(nullptr, delInvmt); // before
 if ( … ) // as before
 {
 pInv.reset(new Stock(std::forward<Ts>(params)...));
 }
 else if ( … ) // as before
 {
 pInv.reset(new Bond(std::forward<Ts>(params)...));
 }
 else if ( … ) // as before
 {
 pInv.reset(new RealEstate(std::forward<Ts>(params)...));
 }
 return pInv; // as before
}
```

When using the default deleter (i.e., delete), you can reasonably assume that std::unique_ptr objects are the same size as raw pointers. When custom deleters enter the picture, this may no longer be the case. Deleters that are function pointers generally cause the size of a std::unique_ptr to grow from one word to two. For deleters that are function objects, the change in size depends on how much state is stored in the function object. Stateless function objects (e.g., from lambda expressions with no captures) incur no size penalty, and this means that when a custom deleter can be implemented as either a function or a captureless lambda expression, the lambda is preferable

std::unique_ptr comes in two forms, one for individual objects (std::unique_ptr<T>) and one for arrays (std::unique_ptr<T[]>). As a result, there’s never any ambiguity about what kind of entity a std::unique_ptr points to. The std::unique_ptr API is designed to match the form you’re using. For example, there’s no indexing operator (operator[]) for the single-object form, while the array form lacks dereferencing operators (operator* and operator->).

std::unique_ptr is the C++11 way to express exclusive ownership, but one of its most attractive features is that it easily and efficiently converts to a std::shared_ptr:
```cpp
std::shared_ptr<Investment> sp = // converts std::unique_ptr
 makeInvestment( arguments ); // to std::shared_ptr
 ```
This is a key part of why std::unique_ptr is so well suited as a factory function return type.

### Things to Remember:
- std::unique_ptr is a small, fast, move-only smart pointer for managing resources with exclusive-ownership semantics.
- By default, resource destruction takes place via delete, but custom deleters can be specified. Stateful deleters and function pointers as deleters increase the size of std::unique_ptr objects.
- Converting a std::unique_ptr to a std::shared_ptr is easy.

## Item 19: Use std::shared_ptr for shared-ownership resource management. 

An object accessed via std::shared_ptrs has its lifetime managed by those pointers through shared ownership. No specific std::shared_ptr owns the object. Instead, all std::shared_ptrs pointing to it collaborate to ensure its destruction at the point where it’s no longer needed. When the last std::shared_ptr pointing to an object stops pointing there (e.g., because the std::shared_ptr is destroyed or made to point to a different object), that std::shared_ptr destroys the object it points to. As with garbage collection, clients need not concern themselves with managing the lifetime of pointed-to objects, but as with destructors, the timing of the objects’ destruc‐
tion is deterministic. 

A std::shared_ptr can tell whether it’s the last one pointing to a resource by consulting the resource’s `reference count`, a value associated with the resource that keeps track of how many std::shared_ptrs point to it. std::shared_ptr constructors increment this count (usually), std::shared_ptr destructors decrement it, and copy assignment operators do both.

The existence of the reference count has performance implications:
- `std::shared_ptrs are twice the size of a raw pointer`, because they internally contain a raw pointer to the resource as well as a raw pointer to the resource's reference count. 
- `Memory for the reference count must be dynamically allocated.`  Item 21 explains that the cost of the dynamic allocation is avoided when the std::shared_ptr is created by std::make_shared, but there are situations where std::make_shared can’t be used. Either way, the reference count is stored as dynamically allocated data.
- `Increments and decrements of the reference count must be atomic`, because there can be simultaneaous readers and writers in different threads. Atomic operations are typically slower than non-atomic operations, so even though reference counts are usually only a word in size, you should assume that reading and writing them is comparatively costly.

a std::shared_ptr object contains a pointer to the reference count for the object it points to. That’s true, but it’s a bit misleading, because the reference count is part of a larger data structure known as the control block. There’s a control block for each object managed by std::shared_ptrs. The control block contains, in addition to the reference count, a copy of the custom deleter, if one has been specified. If a custom allocator was specified, the control block contains a copy of that, too. The control block may also contain additional data, including, as Item 21 explains, a secondary reference count known as the weak count, but we’ll ignore such data in this Item.

An object’s control block is set up by the function creating the first std::shared_ptr to the object. At least that’s what’s supposed to happen. In general, it’s impossible for a function creating a std::shared_ptr to an object to know whether some other std::shared_ptr already points to that object, so the following rules for control block creation are used:

- `std::make_shared (see Item 21) always creates a control block`. It manufac‐
tures a new object to point to, so there is certainly no control block for that object at the time std::make_shared is called.
-  `A control block is created when a std::shared_ptr is constructed from a
unique-ownership pointer (i.e., a std::unique_ptr or std::auto_ptr).` Unique-ownership pointers don’t use control blocks, so there should be no con‐
trol block for the pointed-to object. 
- `When a std::shared_ptr constructor is called with a raw pointer, it creates acontrol block` If you wanted to create a std::shared_ptr from an object that already had a control block, you’d presumably pass a std::shared_ptr or a std::weak_ptr (see Item 20) as a constructor argument, not a raw pointer

A consequence of these rules is that constructing more than one std::shared_ptr from a single raw pointer gives you a complimentary ride on the particle accelerator of undefined behavior, because the pointed-to object will have multiple control blocks.

### Things to Remember
- std::shared_ptrs offer convenience approaching that of garbage collection
for the shared lifetime management of arbitrary resources.
- Compared to std::unique_ptr, std::shared_ptr objects are typically
twice as big, incur overhead for control blocks, and require atomic reference count manipulations.
- Default resource destruction is via delete, but custom deleters are supported. The type of the deleter has no effect on the type of the std::shared_ptr.
- Avoid creating std::shared_ptrs from variables of raw pointer type.

## Item 20: Use std::weak_ptr for std::shared_ptrlike pointers that can dangle.

You may be wondering how a std::weak_ptr could be useful. You’ll probably wonder even more when you examine the std::weak_ptr API. It looks anything but
smart. std::weak_ptrs can’t be dereferenced, nor can they be tested for nullness. That’s because std::weak_ptr isn’t a standalone smart pointer. It’s an augmentation of std::shared_ptr. The relationship begins at birth. std::weak_ptrs are typically created from std::shared_ptrs. They point to the same place as the std::shared_ptrs initializing them, but they don’t affect the reference count of the object they point to.

As a final example of std::weak_ptr’s utility, consider a data structure with objects A, B, and C in it, where A and C share ownership of B and therefore hold
std::shared_ptrs to it:

A (shared_ptr) -> B (shared_ptr) -> C

Suppose it’d be useful to also have a pointer from B back to A. What kind of pointer should this be?

There are three choices:
- A raw pointer. With this approach, if A is destroyed, but C continues to point to B, B will contain a pointer to A that will dangle. B won’t be able to detect that, so B may inadvertently dereference the dangling pointer. That would yield undefined behavior.
- A std::shared_ptr. In this design, A and B contain std::shared_ptrs to each other. The resulting std::shared_ptr cycle (A points to B and B points to A) will prevent both A and B from being destroyed. 
- A std::weak_ptr. This avoids both problems above. If A is destroyed, B’s pointer back to it will dangle, but B will be able to detect that. Furthermore,
though A and B will point to one another, B’s pointer won’t affect A’s reference count, hence can’t keep A from being destroyed when std::shared_ptrs no
longer point to it.

However, it’s worth noting that the need to employ std::weak_ptrs to break prospective cycles of std::shared_ptrs is not terribly common. In strictly hierarchal data structures such as trees, child nodes are typically owned only by their parents. When a parent node is destroyed, its child nodes should be destroyed, too. Links from parents to children are thus generally best represented by std::unique_ptrs.

### Things To Remember 
- Use std::weak_ptr for std::shared_ptr-like pointers that can dangle.
- Potential use cases for std::weak_ptr include caching, observer lists, and the prevention of std::shared_ptr cycles.