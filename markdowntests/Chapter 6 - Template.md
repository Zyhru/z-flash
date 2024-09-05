A template is a **class** or a **function** that we parameterize with a of values or types.
## parameterized types

`template<typename T>` is C++'s version of the mathematical "for all T", or "for all types T". 

`<typename T>` means that you can pass in any data type.
*Templates are a **compile-time** mechanism.*

```c++
template<class T>
class Vector {
private:
	T* elem;
	int sz;
public:
	explicit Vector(int s);
	~Vector(){delete[] elem;}

	T& operator[](int i);
	const T& operator[](int i) const;
	int size() const { return sz; }
};
```

A template + a set of template arguments is called an *instantiation*  or a *specialization*. 

Late in the compilation process, at *instantiation time*, code is generated for each instantiation used in a program.



Concepts are a feature introduced in *C++ 20*. 

It allows to specify *constraints* on **template parameters**. Essentially saying "**This template parameter MUST satisfy certain behaviors or properties**".

To use something like: `template<Element T>` you must first define a concept.

```c++
#include <concept> 
template<typename T>
concept Element = requires(T t) {
	{ t.slideCancel() } -> std::same_as<void>;
};

/* Applying the concept */
template<Element T> 
class Vector {
// ..
}
```

**Concepts** are helpful because it allows you specify constraints on template parameters.

If will be a compile-time error if you don't meet the constraints of a template concept parameter.
```c++
Vector<int> x; // GOOD: int is copyable.
Vector<thread> y; // BAD: standard thread isn't copyable.
```


## Value Template Arguments

A template can take **value arguments** in addition to **type arguments**.
```c++
template<typename T, int N>
struct Buffer {
	constexpr int size() { return N; }
	T elem[N];
};
```

String literals cannot yet be a template value arguments, but you can pass an array that is holding characters of a string.
```c++
template<char* s>
void f() {
	std::cout << s;
}

char array[] = "skill issue";
void use() {
	f()<"hello">();
	f()<arr>();
}
```


## Template Argument Deduction

Template arg deduction is a feature that allows the compiler to automatically determine the template arguments based on the function arguments you pass in. 