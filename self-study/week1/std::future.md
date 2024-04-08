# C++ Weekly Episode 9: std::future Quick- Start

Lets say we want to play around with the C++ random number generators to see what kind of distributions we would get. 

```cpp
std::set<int> make_sorted_random(const size_t num_elems)
{
    std::set<int> retval;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, num_elems-1);

    std::generate_n(std::inserter(retval, retval.end()), num_elems, [&](){ return dis(gen); });
    return retval;
}

int main()
{
    std::cout << make_sorted_random(1000000).size() << ' ' << make_sorted_random(1000000).size() << '\n';
}
```
For each call to the function it takes double the time? What if we use std::future and its multithreading capabilities?

How do we create a future for something that can run in the background?

```cpp
int main()
{
    std::cout << std::async(make_sorted_random, 1000000).get().size() << ' ' << std::async(make_sorted_random, 1000000).get().size() << '\n';
}
```

`auto f1 = std::async(make_sorted_random, 1000000)` tells c++ that we want to call make_sorted_random() asynchronously (done using std::async) and it returns a future. A future holds a value that is promised to exist at some point in the future. and when we call `.get()` on it, it returns the value that is stored in the future (by blocking until it is available). 

so we know the return value is a `std::set<int>`. the problem of async is that it can choose if it runs deferred or asynchronously. 

```cpp
int main()
{
    auto f1 = std::async(std::launch::async, make_sorted_random, 1000000);
    auto f2 = std::async(std::launch::async, make_sorted_random, 1000000);

    std::cout << f1.get().size() << ' ' << f2.get.size() << '\n';
}
```

Futures can also store and forward exceptions. 

If we keep it simple by keeping all parameters passed to asynchronously launched function as copies and return something that is a copy/internal value and never try to access any global or shared data you can get really good parallelization results without worrying about manual locking. 

```cpp
std::set<int> make_sorted_random(const size_t num_elems)
{
    std::set<int> retval;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, num_elems-1);

    std::generate_n(std::inserter(retval, retval.end()), num_elems, [&](){ return dis(gen); });
    return retval;
}

int main()
{
    auto f1 = std::async(std::launch::async, make_sorted_random, 1000000);
    auto f2 = std::async(std::launch::async, make_sorted_random, 1000000);

    std::cout << f1.get().size() << ' ' << f2.get.size() << '\n';
}
```