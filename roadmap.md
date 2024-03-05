TODO:

### C++ Book List
- https://stackoverflow.com/questions/388242/the-definitive-c-book-guide-and-list
- C++ Templates: The Complete Guide (A second edition covering C++11, C++14 and C++17 has been already published)
- C++ 17 - The Complete Guide Standard covering everything from the simple ones like 'Inline Variables', 'constexpr if' all the way up to 'Polymorphic Memory Resources' and 'New and Delete with over aligned Data'.
- C++ 20 - The Complete Guide
- C++ Template Metaprogramming
- C++ Concurrency In Action A second edition covering C++14 and C++17 has already been published.
- Advanced C++ Metaprogramming (pre C++11 but might be useful)

### C++ Impls
i.e. 
```cpp
struct any{
    void* data_;
    std::type_info const& (*getType_)();
    void* (*clone_)(void* otherData);
    void (*destroy_)(void* data);
    template<typename T>
    explicit any(T&& value)
        : data_{new T{std::forward<T>(value)}}
        , getType_{[]() -> std::type_info const&{ return typeid(T); }}
        , clone_([](void* otherData) -> void* { return new T(*static_cast<T*>(otherData)); })
        , destroy_([](void* data_) { delete static_cast<T*>(data_); }){}
    any(any const& other)
    : data_(other.clone_(other.data_))
    , getType_(other.getType_)
    , clone_(other.clone_)
    , destroy_(other.destroy_)
    {}
    ~any(){destroy_(data_);}
};
```
- std::tuple
- std::variant
- std::shared_ptr
- std::unique_ptr
- std::any
- https://github.com/igalakhov/compile-time-newton

### Thesis?
-  if the O2 optimization / 50% memory fragmentation optimization hurdle would be a good topic.
- do automatic memory reclamation for multithreaded code
- https://github.com/microsoft/snmalloc
- https://github.com/DanielLiamAnderson/atomic_shared_ptr
- https://dl.acm.org/doi/10.1145/3519939.3523730
- https://dl.acm.org/doi/10.1145/3558481.3591102
- https://dl.acm.org/doi/10.1145/3627535.3638491
- https://stackoverflow.com/questions/11546075/is-optimisation-level-o3-dangerous-in-g


### CUDA Study
- https://siboehm.com/articles/22/CUDA-MMM
- https://github.com/NVIDIA/cutlass/blob/main/media/docs/efficient_gemm.md
- Udacity playlist parallel programming (DO THE CHECKPOINTS) https://www.youtube.com/watch?v=F620ommtjqk&list=PLnH7E0IG44jFfiQBd_Ov7FmYHq8SZx6r0

### Operating Systems
- Skim through https://pages.cs.wisc.edu/~remzi/OSTEP/ and skip the exercises

### C++ multithreaded socket level programming
- Networking
- read https://beej.us/guide/bgnet/html/split/

### Virtual Tables, Page Tables, Shared Memory
- https://en.wikipedia.org/wiki/Virtual_method_table
- Page Table
- Virtual Memory
- Cachces (make data where its in a line for cache), (optimizing cache hits), (game dev optimizing cache lines)
- https://stackoverflow.com/questions/3928995/how-do-cache-lines-work

scribbled notes from ant_tree:
```
What a bus comp arch 

Sebastian Lague comp arch 

Virtual tables

Pages / shared memory

Every struct has a pointer to the v table

At run time u some memory that stores the vtable (usually not hot, as in not used often and might be longer to look up)

Heavily optimized to do as quickly as possible) 

U know where in the table where some method is gonna be

All a method to a struct is is this and its a pointer to a struct

9 cycles per table lookup

In performant code u never write virtual functions

Can always get around not having to use it.

Ways to check if its A B or C static cast and run it. 


physical memory stored on the ram stored on disk

Not asking the disco every time

Accessing ssd/s and disks 

Thing called a page table, (process using virtual memory)  looks at page table and finds a way to find the corresponding physical memory 

Bits of virtual physical memory

Depends but page is usually 4mb on average

Can take multiple virtual memory and map it to same page table 

Shared memory is mapping virtual memory to same physical address

Virtual memory is reasonably fast but there is fast 

std::launder 

Implement std::variant from scratch 

Template tricks 

Step 1 convince urself is possible

Write a really shitty solution 

Make it not shitty

Write compile time map 

At a compile time I wanna look up a number

Template crtp 

Talk about shared page table, shared memory is important
Signals, threads, open a pipe, 

Signals slower than shared memory

Synchronous shared memory. 

Access same memory a lot and memory close to it so its faster

CRTP

Structs public 

Classes and struct same thing besides default disability


dynamic
 cast runtime

Static cast compile time

Reason why we can’t do this->foo(); bc we dont know it at compile time, but we will know at runtime

void* is just saying here’s memory but we dont know the type of it

Void type just doesn’t have anything

static_cast<T*>(this)->foo();


At compile time c++ needs to know methods and the typename for structs and classes, but its not known for external functions

So if we want to use the type is has to be within a class / struct 

Struct reference counter

std::tuple

```

From Tanz:
```
VTables (or virtual tables) are arrays of pointers to virtual functions.
They allow C++ to support dynamic dispatch (or run-time method binding).
Virtual functions are member functions of a C++ class that can be redefined in a child class.
A pointer to a vtable will exist as a datamember of all classes with virtual functions.
When I call a virtual method, I lookup the object's v-table and call the appropriate derived class method.

Essentially allows the below behavior:
```

```cpp
#include <iostream>

struct Base{
    virtual int method(){
        return 1;
    }
};

struct Derived: Base{
    int method() override{
        return 2;
    }
};

int main(){
    Base* a = new Derived();
    // output is 2
    std::cout << a->method() << std::endl;
    // The original can still be called with Base::method
}
```

So what does this look like under the hood? This is all compiler dependant but let's say I have the below code:

```cpp
struct Base{
    virtual int method();
    virtual int method2();
    virtual int method3();
};
```
The compiler builds a table containing 3 function-pointers. This is a relatively small space overhead. Every object of type base will have a pointer to this object.

Now lets say we have a Derived class.
```cpp
struct Derived: Base {
    int method() override;
};
```
The compiler will build and store another static table in memory.
Since we don't add any more virtual functions the table is still of size 3.
Now, we can just use the same function pointers for method1 and method2, but we need to replace function pointer for method and any other things we might override.
The pointer for derived class objects will be to this new vtable.

When we try to call a virtual function we dereference the pointer and use the function pointer to call the function.
This way we are able to figure out which function to use dynamically.
A good explanation of what goes on under the hood and in the hardware can be found here (http://www.dietmar-kuehl.de/mirror/c++-faq/virtual-functions.html#faq-20.4).
Essentially there are three calls that go on in the hardware to fetch the location and load the function.


### Youtube
- HFT Ultra Low latency https://www.youtube.com/watch?v=_0aU8S-hFQI
- Convex Optimization https://www.youtube.com/watch?v=McLq1hEq3UY&list=PL3940DD956CDF0622
- Optiver trading C++ https://www.youtube.com/watch?v=8uAW5FQtcvE
- Watch carl cook shit
- Atomics https://youtu.be/ZQFzMfHIxng?si=qNP17-tu15LA_u8Y
- Ranges, Views, Pipelines, and Currying https://youtu.be/h1aRKYs-FqA?si=ZxpOLOLR1RbtLpRb
- std::linear algebra https://youtu.be/-UXHMlAMXNk?si=690-HzlvRClVxaI9
- atomic shared pointers https://www.youtube.com/watch?v=lNPZV9Iqo3U
- how does an OS boot https://www.youtube.com/watch?v=KkenLT8S9Hs
- waterloo convex optimization stuff https://ucalendar.uwaterloo.ca/2324/COURSE/course-CO.html
- waterloo optimization class https://wwkong.github.io/files/notes/co255_CouseNotes_W14.pdf
- ipad list 

### Random:
- Compiler course at waterloo https://student.cs.uwaterloo.ca/~cs444/
- Algorithms class at cornell https://www.cs.cornell.edu/courses/cs6820/2023fa/lectures.html
- perf engineering class MIT playlist https://youtube.com/playlist?list=PLUl4u3cNGP63VIBQVWguXxZZi0566y7Wf&si=hnk-ZJmcOX37PNXI
- competitive programming handbook - https://cses.fi/book/book.pdf
- kleinberg for algos https://www.cs.princeton.edu/~wayne/kleinberg-tardos/
- Options, Futures, and Other Derivatives - John C. Hull
- Trivia: just sort stackoverflow by cpp20 and cpp17 tagged qs
- Cornell phd compiler self guided course https://www.cs.cornell.edu/courses/cs6120/2023fa/self-guided/
- oop notes https://github.com/clayton-halim/cs246e-notes/blob/master/Notes/problem_13.md




