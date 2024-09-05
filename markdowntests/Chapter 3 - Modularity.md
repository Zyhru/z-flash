## Introduction

The first most important step is to distinguish between the interface and its implementation. 

C++ represents interfaces by declarations. A declaration specifies all that's needed to use a function or type.

```c++
double sqrt(double);

class Vector {
public:
	Vector(int s);
	double& operator[](int i):
	int size();
private:
	double* elem;
	int sz;
}
```

## Separate Compilation

A `.cpp` file that is compiled by itself (including the header file it includes) is called a *translation unit*.

## Modules (C++20)

The language feature called **modules** is part of C++20. 

```c++
module; // will compile a defined model

export module Vector;

export class Vector {
public:
	Vector(int s);
	double& operator[](int i):
	int size();
private:
	double* elem;
	int sz;

Vector::Vector(int s): elem{new double[s], sz{s}} {}

double& Vector::operator[](int i) {
	return elem[i];
}

int Vector::size() {
	return sz;
}

export int size(const Vector& v) { return v.size(); }


// in main.cpp
import Vector;
#include <cmath>

double sqrt_sum(Vector& v) {
 double sum = 0;
 for(int i = 0; i < v.size(); ++i) {
	sum += std::sqrt(v[i]);
 }
 return sum;
}

}
```

### Differences between Headers and Modules

- A module is compiled once only
- Two modules can be imported in either order without changing their meaning
- If you import something into a module, users of your module do not implicitly gain access to. import is not transitive.

Transitive - Meaning you do not automatically import the modules that the current module depends on.

## Namespaces

Namespaces expresses that some declarations belong together and that their names shouldn't clash with other names.

```cpp
// First namespace
namespace Space1 {
    void func() {
        std::cout << "Inside Space1" << std::endl;
    }
}

// Second namespace
namespace Space2 {
    void func() {
        std::cout << "Inside Space2" << std::endl;
    }
}

int main() {
    // Calls function from Space1
    Space1::func();

    // Calls function from Space2
    Space2::func();

    return 0;
}
```

They are primarily used to organize larger components, such as libraries.

## Error Handling

### Exceptions

```c++
#include <exception>
double& Vector::operator[](int i) {
	if(i < 0 || size() <= i) {
		throw out_of_range("Vector::operator[]");
	}
	return elem[i];
}
```

The `throw` transfers control to a handler for exceptions of type `out_of_range` *directly or indirectly* called **Vector::operator[]()** 

This will happen as so: The implementation will *unwind* the function call stack as needed to get back to the context of that caller. 

A function that will never throw an exception can be declared as `noexcept`.

```c++
void user(int sz) noexcept {
	Vector v(sz);
	iota(&v[0], &v[sz], 1);
}
```

## Invariants

Whenever we define a function, we should consider what its preconditions are and consider whether to test them. 

A statement/comment of what is assumed to be true for a class is called a *class invariant, or simply an invariant*.

It is the job of the constructor to establish an invariant for its class. 

And for the member functions to make sure that the invariant holds when they exit. 


```c++
class Stack {
private:
    std::vector<int> elements;
    int maxSize;

public:
    Stack(int size) : maxSize(size) {}

    void push(int element) {
        if (elements.size() < maxSize) {
            elements.push_back(element);
        }
    }

    void pop() {
        if (!elements.empty()) {
            elements.pop_back();
        }
    }

    int top() const {
        if (!elements.empty()) {
            return elements.back();
        }
        throw std::out_of_range("Stack is empty");
    }

    bool isEmpty() const {
        return elements.empty();
    }

// Invariant: The size of 'elements' must always be less than or equal to 'maxSize'.
// This invariant ensures that the stack does not exceed its maximum capacity.
};
```

Often, a function has no way of completing its assigned task after an exception is thrown.

## Error-handling Alternatives

A function can indicate that it cannot perform its allotted task by:

- throwing an exception
- return a value indicating failure
- terminating the program with: `terminate()`, `exit()`, `abort()`

Return an error indicator (error code) when:
+ A failure is normal or expected. (It's common to request to open and fail. Maybe the file doesn't exist or file has strict permissions)
+ An immediate caller can reasonably be expected to handle the failure

We throw an exception when:

```c++
/*
- An error is so rare that a programmer is likely to forget to check for it. 
  For example, when did you last check the return value of printf()?
  
- An error cannot be handled by an immediate caller. Instead, the error has to 
  percolate back to an ultimate caller. For example, it is infeasible to have 
  every function in an application reliably handle every allocation failure or network outage.
  
- New kinds of errors can be added in lower-modules of an application so that 
  higher-level modules are not written to cope with such errors. For example, 
  when a previously single-threaded application is modified to use multiple threads 
  or resources are placed remotely to be accessed over a network.
  
- No suitable return path for error codes is available. For example, a constructor 
  does not have a return value for a ‘‘caller’’ to check. In particular, constructors 
  may be invoked for several local variables or in a partially constructed complex object 
  so that clean-up based on error codes would be quite complicated.
  
- The return path of a function is made more complicated or expensive by a need 
  to pass both a value and an error indicator back (e.g., a pair), possibly leading 
  to the use of out-parameters, non-local error-status indicators, or other workarounds.
  
- The error has to be transmitted up a call chain to an ‘‘ultimate caller.’’ 
  Repeatedly checking an error code would be tedious, expensive, and error-prone.
  
- The recovery from errors depends on the results of several function calls, 
  leading to the need to maintain local state between calls and complicated control structures.
  
- The function that found the error was a callback (a function argument), 
  so the immediate caller may not even know what function was called.
  
- An error implies that some ‘‘undo action’’ is needed.
*/

```

The ensure termination add `noexcept` to a function so that `throw` from anywhere in the functions implementation will turn into a terminate(). 

Do not believe the myth that exception handling is slow.

## Contracts

No general and standard way of writing optional run-time tests of invariants, preconditions, etc. 

A contract mechanism is proposed in C++20. 

For now, we can use ad hoc mechanisms. 
```c++
double& Vector::operator[](int i) {
	if(RANGE_CHECK && i < 0 || size() <= i) {
		throw out_of_range{"Vector::operator[]"};
	}
	return elem[i];
}
```

Standard library offers a debug macro `assert()`. To assert a condition must hold at runtime.
```c++
void f(const char* p) {
	assert(p != nullptr);
	// ..
}
```

If the `assert()` fails in *debug mode*, the program terminates. If we are not in debug mode, the `assert()` is not checked.

## Static Assertions

Exception report errors found at runtime. If an error can be found at compile, it is usually best to do so. 

To perform simple checks at compile time we can simply use the `static_assert()`.

```c++
static_assert(4 <= sizeof(int), "Integers are too small"); // Checks int size
```

If an `int` on this system does not have least 4 bytes then perform this compile time check.

The `static_assert` mechanism can be used for anything that can be expressed in terms of constant expressions.

## Function Arguments and Return Values

The primary way of passing information from one part of a program to another is through a function call. 

Information needed to perform a task is passed as **arguments** to a function.

The results that are produced are passed back as **values**.
### Argument Passing

We usually pass small values **by-value** and larger ones **by-reference**. 

Small refers to *something that is cheap to copy* (Size of two or three pointer or less is a good rule of thumb)

A function argument can have a default value (A value that is considered preferred or most common).
```c++
void print(int val, int base = 10);
print(x,16); // hexadecimal
print(x,60); // Sexagesimal
print(x); // using default base
```

This is a nice alternative to overloading:
```c++
void print(int val, int base);

void print(int val) {
	print(value, 10);
}
```
## Value Return

A local variable disappears when the function returns, so we should **not** return a pointer or a reference to it.
```c++
int& bad() {
	int x;
	return x; // returns a reference to the local variable x
	// undefined behavior
}
```
## Structured Binding

```c++
struct Entry {
	std::string name;
	int value;
}

Entry read_entry(std::istream& is) {
	std::string s;
	int i;
	is >> s >> i;
	return {s,i};
}
```

In the example above `{s,i}` is used to construct the return value. We can *unpack* an **Entry's** members into local variables like so:
```c++
auto [n,v] = read_entry(is); // Structured binding
std::cout << "{" << n << v;
```

## Advice

- Distinguish between declaration (used as interfaces) and definitions. (used as implementations)
- Use namespaces to express logical structure.
- Don't put the `using` directive in header files.
- Exceptions for error handling only.
- Develop an error-handling strategy early in design 
- Prefer RAII over try-blocks
- Constructor establishes invariant
- Design your error-handling around invariants
- Pass small values by value and large values by references
- Don't overuse structured binding.