# Episode 6 : Intro to Variadic Templates


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