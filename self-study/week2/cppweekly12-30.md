## Ep 12 C++17's std::any

std::any is a typesafe container that can hold values of any type

```cpp
int main(){
    std::vector<std::any> v (5, 3.4, "Hello World");

    std::any_cast<int>(v[0]); //would print 5
    //std::any_cast<float>(v[0]); //error bad cast
    v[1].type().name(); //double
}
```

You cannot put move only objects in the container. It must be something that is copy-constructable. 

```cpp

struct S {
    //S(const S &s) = delete; //cannot put in vec
    S(const S &s) = default; //can now put in vec
    S() = default; 
}

int main(){
    std::vector<std::any> v (5, 3.4, "Hello World");

    std::any_cast<int>(v[0]); //would print 5
    //std::any_cast<float>(v[0]); //error bad cast
    v[1].type().name(); //double

    //int &i = std::any_cast<int>(v[0]); //will not work for reference

    int *i = std::any_cast<int>(v[0]); //will compile
}
```

Implementations are encouraged to avoid dynamic allocations for small objects. 

## Ep 14 - Standard Library Gems: `next` and `exchange`

std::next returns next forward iterator

```cpp
int main(){
    std::vector<int> {1,2,3,4,5,6,7};
    std::cout << std::is_sorted(std::begin(v), std::end(v)) << '\n';

    //lets say if u want to tell if everything BUT the first element is sorted 
    
    std::cout << std::is_sorted(std::next(std::begin(v)), std::end(v)) << '\n';

    //can be really helpful bc sometimes things are not random access iterators so we cannot do iterator math like begin() + 1.
}
```

std::exchange replaces the value of obj with new_value and returns the old value of obj.

```cpp
int main(){
    for(int i = 0; i < 1000; ++i){
        last = i;
        i = new_value;
        
        //we can do it all in one line:
        last = std::exchange(i, new_value);
    }
}
```

For integers doesnt save us a lot but for things that are more expensive to copy it is pretty useful. 

## Ep 15 Using `std::bind` and Ep 16 avoid using it

it takes a thing called f (function object, pointer to function, reference to function, pointer to member function, or pointer to data member) that will be bound to some arguments

basically anything that can be called as if it were a function. (it binds parameters to functions). it takes a callable object and returns a new callable one. 

```cpp
void print(const int& i)
{
    std::cout<< i << '\n';
}
int main()
{
    const auto f = std::bind(&print, 5);
    f(); //will print 5 to the screen

    int i = 5;
    const auto f = std::bind(&print, i);
    f(); //will print 5 to the screen

    int i = 6;
    f(); //will still print 5 bc bind makes copy of parameters
    
    //if we want to keep track of it we need a std ref wrapper.
    const auto f = std::bind(&print, std::ref(i));
    int i = 7;
    f(); //will print 7
}
```

takes a bit to compile.... 

```cpp
template<typename T>
void print(T i, const std::string& s)
{
    std:: cout << i << ' ' s << '\n';
}

int main()
{
    int i = 5;
    //using bind to reorder the arguments of this function.
    const auto f = std::bind(&print<int>, std::placeholders::_2, std::placeholders::_1);
    
    f("hello", i);
    i = 6;
    f("world", i);

    std::function<void (const std::string&, int)> f2(f);
    f2("test", i);
}

int main()
{
    int i = 5;
    //instead of using bind use lambda to swap arguments
    const auto f = [](auto &&arg1, auto &&arg2)
    {   
        //potentially with std::forward perfect forwarding if we wanted
        print(arg2,arg1);
    };

    f("hello", i);
    i = 6;
    f("world", i);

    std::function<void (const std::string&, int)> f2(f);
    f2("test", i);

}
```

Using the lambda is almost 20% faster. 

std::bind sometimes has both extreme compile time and runtime overhead

## Ep 17 C++17's `std::invoke`

Function that gives us a inform interface into calling callable things.

```cpp
int do_something(const int i)
{
    return 5 + i;
}

int main()
{
    std::invoke(&do_something, 5); //returns 10
}
```

By giving us a uniform interface to call anything thats callable we can do things like this:

```cpp
int do_something(const int i)
{
    return 5 + i;
}

struct S
{
    int j = 5;
    int do_something(const int i)
    {
        return j + i;
    }

    int do_something_2(const int i)
    {
        return j * i;
    }
}

int main()
{
    std::invoke(&do_something, 5); //returns 10
    S s;
    s.do_something(3); //returns 8;

    auto fp = &S::do_something; 
    int(S::*fp2)(int) = nullptr;

    if(true){
        fp2 = &S::do_something_2;
    } else {
        fp2 = &S::do_something;
    }

    (s.*fp)(2); //returns 7
    (s.*fp2)(1); //returns 5

    //instead of all this we can just
    std::invoke(&S::do_something, s, 10); 
    //returns 15

    std::invoke(&S::j, s, 10); //returns 5(member variable) from obj s

}
```

can save us a lot of time not having to impl overloads. (depending on const volatile etc)

## Ep 18 C++17's constexpr if

Allows us to choose at compile time 1 or more branches to take in our code. 

```cpp
template<typename T>
auto print_type_info(const T&t)
{
    if constexpr(std::is_integral<T>::value and !std::is_same<bool, T>::value){
        std::cout << "is integral\n;"
        return t + 1;

    } else if constexpr(std::is_float<T>::value){
        return t + 0.1;

    } else {
        std::cout << "is not integral\n";
        return t;
    }
}

int main()
{
    print_type_info(5); //6
    print_type_info(2.3); //2.4
    print_type_info(true); //true
    print_type_info("hello world"); //hello world
}

```

Can really clean things up / optimize our code

## Ep 20 C++17's Fold Expressions - Introduction

```cpp
//C++14
template<typename ... T>
auto sum(T ... t)
{
    std::common_type<T...>::type result{};
    (void)std::initializer_list<int>{ (result += t, 0)... };
    return result;
}

//C++17
template<typename ... T>
auto sum(T ... t)
{
    return (t + ...);
}

template<typename ... T>
auto div(T ... t)
{
    //fold expressions can be expanded on the left, right, or middle
    
    //i.e.  return (... / t);

    return (t / ...);
}

template<typename ... T>
auto avg(T ... t)
{
    return (t + ...) / sizeof...(t);
}


int main()
{
    std::cout << sum(1, 2, 3.0, 4.5) << '\n';
    //prints 10.5

    std::cout << div(1.0, 2.0, 3.0) << '\n';
    
    std::cout << avg(1.0, 2.0, 3.0) << '\n';
}
```

## Ep 21 C++17's `if` and `switch` Init Statements

```cpp
std::vector<int> make_vec()
{
    return {1,2,3,4};
}

int main()
{
    auto vec = make_vec();
    
    const auto itr = std::find(vec.begin(), vec.end(), 2);
    if(itr != vec.end()){
        *itr = 3; //find a 2 and change it to 3
    }

    const auto itr2 = std::find(vec.rbegin(), vec.rend(), 3);
    if(itr2 != vec.rend()){
        *itr2 = 4; //find last 3 and change it to 4
    }
}

//we can just do

int main()
{
    auto vec = make_vec();
    
    {
        const auto itr = std::find(vec.rbegin(), vec.rend(), 2);
        if(itr != vec.rend())
        {
            *itr = 3;
        } else {
            vec.insert(itr, 4);
        }
    }
    if(const auto itr = std::find(vec.begin(), vec.end(), 2); 
        itr!= vec.rend())
    {
        *itr = 4;
    } else if (const auto front = vec.front(); front != 2) {
        vec.insert(itr, 4);
    }
    else {
        return front;
    }


    switch(const auto obj = get_obj(); obj.value()) {
        //
    }
}
```

## Ep 22 C++17's Nested Namespaces and `std::clamp`

```cpp
//C++14
namespace org {
    namespace group {
        namesprace project {
            namespace internal {
                struct MyStruct{};
            }
        }
    }
}

//C++17
namespace org::group::project::internal{
    struct MyStruct{};
}

int main()
{
    org::group::project::internal::MyStruct s;

    //nested ugly nested ternary
    (argc < 2) ? 2 : (argc > 5) ? 5 : argc; 

    std::clamp(argc, 2, 5); 
    //will check if between 2 and 5
}

```

## Ep 24 C++17's Structured Bindings

its just the auto [x, y] stuff.

pairs, tuples, structs etc.

## Ep 25 C++17's Aggregate Initializations

we have a way of initializing base class objects

```cpp
struct B
{
    double q;
};

struct S : B 
{
    int i;
    float f;
};

int main()
{
    S s{{54.8}, 1, 2.3};
    return s.q + s.f; //57.1
}
```

## Ep 27 - C++17 Changes To Sequence Containers

emplace_back() basically says construct a new object of this type and put it immediately where it needs to go in place in memory. theres no move no copy its as efficient as can be. 

the c++ standards committee agree that there is often a need to be able to access the data that was just emplaced_back. 

```cpp
std::vector<std::vector<int>> v;
v.emplace_back(3,1).push_back(4);

//works for emplace_front

//regular emplace still returns iterator only emplace_back and emplace_front return references to the objects that were created in the container. 
```


