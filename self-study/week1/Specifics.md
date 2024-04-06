# Episode 2 : Cost of Using Statics

```cpp
struct C {
    static const std::string &magic_static()
    {
        static const std::string s = "bob";
        return s;
    }
    const std::string &s = magic_static();

    const std::string &magic_static_ref()
    {
        return s;
    }
}
```
`static const std::string s = "bob"` is not constructed until the first time this `&magic_static()` function has been called.

since C++11 initialization of static variables happen in a thread-safe fashion. 

`const std::string &s = magic_static();` is a local reference created using class initialization. So when the object of type C is constructed the member variable `s` is assigned a `const reference` of the value that is returned by the `magic_static()` function call. 

`&magic_static_ref()` that are using to get a reference back to the member variable `s`. This method is not declared static, so it is returning a reference to the member variable `s` and not a reference to the `static const std::string s = "bob";` in the first function.

All three functions are returning pointers to the same value. to the same object in the first function: `static const std::string s = "bob";`

What we are trying to do is identify how much overhead there is in accessing a static variable. 

Intuitively, the fact that a static variable is guaranteed to be constructed in a thread safe manner requires that the compiler must have some way of determining if the variable has already been constructed. If not, performing a lock and constructing the variable for us. 

```cpp
auto main() -> int
{
    C::magic_static().size();
    C::magic_static().size();
    C::magic_static().size();
    return C::magic_static().size();
}
```

Compiler has to make a call to the size function every time

Since it is static, there is a cmpb with a guard variable to check if it has been set. This compare happens every time. So we end up with 4 distinct function calls that have a thread safe comparison to see if the variable has been initialized or not.

```cpp
auto main() -> int
{
    C c;
    c.magic_static_ref().size();
    c.magic_static_ref().size();
    c.magic_static_ref().size();
    return c.magic_static_ref().size();
}
```

our `magic_static_ref()` fucntion returns a cached reference to this variable s. Here the compiler has completely eliminated all of these calls to magic_static_ref().size() because it knows there are no side affects to those calls. so we do not have to pay to access the variable created by our static function. 

What do we do with this? ehhh just something to be aware of if working with static variables that we access a lot. 


# Episode 7: Stop Using std::endl

```cpp
int main()
{
    std::cout << "Hello World" << std::endl;
    //std::endl is the same as 
    std::cout << "Hello World" << '\n' << std::flush;

    //we dont actually need the std::flush every time
    //its faster to do 
    std::cout << "Hello World" << '\n';
}
```

instead of a string `"\n"` we should be sending a single character `'\n'` bc in theory it should be more efficient. if you send an ostream operator a string its going to have to loop over all the characters of the string looking for null and then stop when its done. if its just a single character it knows it has only one thing to do. 

there is no real point in repeatedly using `std::endl` because if you wanted your file to be flushed you could just do `std::flush` once at the end. 

# Episode 8: C++ Name Demangling
