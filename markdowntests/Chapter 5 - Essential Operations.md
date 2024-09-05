
There five situations in which an object can be copied or moved. 

+ As the source of an assignment
+ As the object initializer 
+ As a function argument 
+ As a function return value
+ As an exception

If you want to be explicit about generating default implementations, you can use the `default` keyword:

```c++
class Y {
public:
	Y(sometype);
	Y(const Y&) = default
	Y(Y&&) = default;
}
```

The `=delete` prevents from your class to be copyable.
```c++
class Example {
public:
	Example() = default;
	Example(const Example&) = delete;
	Example& operator=(const Example&) = delete;
}

int main() {
	Example ex1;
	Example ex2 = ex1; // compilation error: copy constructor is deleted
	Example ex3;
	ex3 = ex1; // compilation error: copy constructor is deleted
}
```

Also it's good to use when for compiler error messages when a certain constructor is not needed. Therefore, in the example above, our copy constructor will be deleted, hence preventing our class to be copyable.

## Conversions

A constructor taking a single argument defines a conversion from its argument type.
```c++
Vector v1 = 10; // OK: has 10 elements
```

But the standard library does not allow this **int to vector** conversion.
To fix this problem we can set the constructor as **explicit**

```c++
class FuckYou {
public:
	explicit FuckYou(int s);
}

int main() {
	FuckYou fy(298); // OK
	FuckYou fi = 29; // error: no implicit conversion from int to FuckYou
}
```

Use **explicit** for constructors that take a single argument unless there is a good reason to.
## Copy and Move

The default meaning of copy is memberwise copy: copy each member.

When creating and designing a class you must think about how this class might be copied. For simple concrete types you can just use the assignment operator to copy the value from one object to the other.

```c++
Object obj1;
Object obj2;
obj1 = obj2; // copying members of obj2 into obj1
```

Copying objects when a class accesses an object via a pointer will not work with default member wise copying because it will point both objects to the same thing, leading to disaster.

To fix this problem we must define better copy semantics. 
Copying of an object of a class is defined by two members:

+ A copy constructor T(&T)
+ A copy assignment T* operator=(const T& other)

```c++
T::T(const T& other): elem{new double[other.size]}, sz{other.size} {
	for(int i = 0; i < size; ++i) {
		elem[i]	= other.elem[i];
	}
}

T& operator=(const T& other) {
	double* ptr = new double[other.size];
	for(int i = 0; i < size; ++i) {
		p[i] = other.elem[i];
	}
	delete[] elem;
	elem = p;
	size = other.size;
	return *this;
}
```

The `this` is predefined in a member function and points to the object for which the member function is called.
## Moving Containers

If you want a result/value and don't want a copy its best to *move* rather than copy.

```c++
class Vector {
	Vector(const Vector& a); // copy constructor
	Vector& operator=(const Vector& a); // copy assignment
	
	Vector(Vector&& a); // move constructor
	Vector& operator=(Vector&& a); // move assignment
}
```

```c++
Vector::Vector(Vector&& a) : elem{a.elem}, sz{a.sz} {
	a.elem = nullptr;
	a.sz = 0;
}
```

The `&&` means r-value reference. A reference in which we can bind an rvalue. 

R-value - A value that you can't assign to. Thus, an rvalue reference is a reference to something that *nobody else* can assign to, so we can safely *steal* its value.

The move constructor does not take a **const** argument. A move constructor is supposed to **remove** the value from its argument. 

After a move, the moved-from object should be in a state that allows a destructor to be run. 
```c++
Vector f() { 
Vector x(1000); 
Vector y(2000); 
Vector z(3000); 
z = x; // we get a copy (x might be used later in f()) 
y = std::move(x); // we get a move (move assignment)   
// dont use x here, x will be in a weird state
return z; // we get a move 
}
```

`std::move()` returns a reference to its argument from which we may move.
Move assignments can be critical for performance.

## Resource Management

*Smart pointers* such as **unique_ptr** are resource handles.

Memory is the not the only resource. Examples are:

+ Memory 
+ Locks
+ Sockets
+ File Handles
+ Thread Handles

Letting each resource have an owner in some scope and by default be released at the end of its owner's scope is know as *RAII (Resource Acquisition Is Initialization)* and is integrated with error handling in the form of exceptions.

Resources can be moved from scope to scope using *smart pointers*, *shared pointers, and move semantics*.

## Operator Overloading

Giving meaning to C++'s operators for user-defined types are called *operator overloading*.

```c++
class Matrix {
	Matrix& operator=(const Matrix& a);
}
```

## Container Operations

Containers in C++ all know the number of elements and the standard algorithms rely on the notion of sequences delimited by pairs of iterators.

```c++
for(auto ptr = c.begin(); ptr != c.end(); ++ptr) {
	// Access ptr like so:
	// ptr->
}
```

`c.begin()` is an iterator pointing to the first element of c and `c.end()` is an iterator that points to the one **beyond** the last element of `c`.

Iterators support `++` to **move** to the next element and `*` to access the value of the pointed-to-element.

Iterators are used to pass sequences to standard library algorithms.
```c++
sort(v.begin(), v.end());
```

### Iterators and smart pointers

Access: `*`, `->` (for a class), and `[]` (for a container)
Iteration/navigation: `++` (forward), `--` (backward), `+=`, `-=`, `+`, and `-`
Copy and/or move: `=`

### I/O Operations

For pairs of int's, `<<` means left-shift, and `>>` means right-shift.

However for **iostreams**, they are output and input operators.

`>>` input
`<<` output