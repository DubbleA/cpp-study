# Episode 6 : Intro to Variadic Templates C++14


```cpp
#include <iostream>

//quick template to print any value to the screen
template<typename T>
void print(const T& t)
{
    std::cout << t << '\n';
}

int main()
{
    print(1);
    print("hello world");
    print(5.3);
}

```

```cpp
//what if we want to return a string representation?
template<typename T>
std::string to_string_impl(const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

//now lets say instead of just one thing we want to return many things:

//function for 0 parameter case
std::vector<std::string> to_string()
{
    return {};
}


//need to use `...` to tell compiler that we want a variadic template
//typename P1 and const P1 &p1 is there to require at least 1 parameter
template<typename P1, typename ... Param> 
std::vector<std::string> to_string(const P1 &p1, const Param& ... param)
{
    std::vector<std::string> s;
    s.emplace_back(to_string_impl(p1));

    //compiler expands this to param2, param3, param4, etc
    //calls ourselves recursively peeling off first parameter every time

    const auto remainder = to_string(param ...);

    s.insert(s.end(), remainder.begin(), remainder.end());
}

int main()
{
    const auto vec = to_string("hello", 1, 5.3, "World", 1.3f, 1, 2, 3, 4, 5, 1000, "bob");
    for(const auto &o : vec) std::cout << o << '\n';
}
```

We unnessessarily complicated this example, we DO NOT need to make recursive template instantiations. In fact we rarely need to... especially if we can get creative with initializer lists. 

```cpp
template<typename T>
std::string to_string_impl(const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template<typename ... Param> 
std::vector<std::string> to_string(const Param& ... param)
{
    //will use an initalizer list to initialize a std vector of strings and will expand all of the to_string_impl calls for each param that is passed in
    return {to_string_impl(param)...};
}

int main()
{
    const auto vec = to_string("hello", 1, 5.3, "World", 1.3f, 1, 2, 3, 4, 5, 1000, "bob");
    for(const auto &o : vec) std::cout << o << '\n';
}
```

Standard lambda function implementation:
```cpp
template<typename ... Param> 
std::vector<std::string> to_string(const Param& ... param)
{
    const auto to_string_impl = [](const auto &t){
        std::stringstream ss;
        ss << t;
        return ss.str();
    };
    return {to_string_impl(param)...};
}

int main()
{
    const auto vec = to_string("hello", 1, 5.3, "World", 1.3f, 1, 2, 3, 4, 5, 1000, "bob");
    for(const auto &o : vec) std::cout << o << '\n';
}
```

# Episode 10 : Variadic Expansion Wrap-Up

```cpp
template <typename T>
void print_impl(const T& t)
{
    std::cout << t << "\n";
}

template <typename T>
void print(const T& ... t)
{
    //need to put it in a comma delimited expression
    (void)std::initializer_list<int>{(print_impl(t), 0) ...};
}

int main()
{
    print("hello", 1, 1.5f, 'c');
}
```

`(void)std::initializer_list<int>{(print_impl(t), 0) ...};` is a little trick we can use to get it to compile. we want to take an integer after the expansion of the evaluation of `(print_impl(t), 0)` the comma expression basically tells the compiler we want to evaluate the left hand side and the right hand side and only return the result on the right hand side.  

Two important things to remember
1. Order of parameter evaluation for function calls is unspecified by the std and different compilers do it differently

For example:

```cpp
int f1()
{
    std::cout << "f1\n";
    return 1;
}

int f2()
{
    std::cout << "f2\n";
    return 2;
}

int main() {
    print(f1(), f2());
}
```

will print
```
f2
f1
1
2
```
on the g++ compiler. which means that the compiler is evaluating f2() first then f1() as it's free to rearrange these things.

but on clang it will print
```
f1
f2
1
2
```
so we cannot rely on the order it might be executed.

BUT there are two cases in which we can rely on the order that it is executed. and we have both of them here: `(void)std::initializer_list<int>{(print_impl(t), 0) ...};`
so what we have here is our initializr_list and out outermost set of braces { } is guaranteed to execute things in the order specified including the parameter expansion `...` AND things that are in a comma delimited sequence using the comma operator are also guaranteed to be executed in order. 

so for `(print_impl(t), 0)` we are guaranted that `print_impl(t)` will be executed before the 0, and then the 0 is actually returned back which goes into the initializer list (which is essentially a temporary array created on the stack which is optimized away by the compiler. and we are casting it void just to tell the compiler that we have no intention of using the result value and it all goes away cleanly)

to refactor our code a bit we can

```cpp
template <typename T>
void print(const T& ... t)
{
    (void)std::initializer_list<int>{(std::cout << t << '\n', 0) ...};
}
```

what is the most succinct way to create a vector of strings from our set of values passed?

```cpp
template <typename T>
std::vector<std::string> func(const T& ... t)
{
    std::vector<std::string> retval;
    std::stringstream ss;
    (void)std::initializer_list<int>{ (ss.str(""), ss << t, retval.emplace_back(ss.str()), 0) ...};
    return retval;
}
```

- `ss.str("")` first reset's our stringstream object
- `ss << t` use our string stream object to convert t to a string value
- `retval.emplace_back(ss.str())` push back current value
- `0` return our dummy val
- `...` expand all the params

```cpp
int main()
{
    for ( const auto&s : print("Hello", "World", 5.4, 1.1, 2.2) ) {
        std::cout << s << "\n";
    }
}
```

will print / store all the values as expected