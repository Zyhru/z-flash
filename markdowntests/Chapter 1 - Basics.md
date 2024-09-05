C++ is a compiled language. Meaning source text will be processed by a compiler, producing object files, which are **combined** by a **linker** yielding an executable program. 

Source file -> compile -> object file -> link -> executable file
Executable programs are NOT portable. (e.g Windows -> Mac (vice-versa))

C++ is a statically typed language, meaning every entity must have a type. Must be known to the compiler at its point of use. 
## << Operator

The `<<` operator means to ("put to the output stream"). 
The `>>` operator is used for input. **cin** is the standard output input stream.

```c++
// "Hello World\n" is written to the standard output stream
std::cout << "Hello World\n" << std::endl;
```

## Functions

- If two alternative functions could be called, but neither is better than the other, the call is deemed ambiguous and the compiler gives an error.

```c++
void print(int, double);
void print(double, int);

void user2() {
	print(0,0); // error: ambigious (could not find function delcaration)
}
```

Function overloading is one of the essential parts of generic programming.

## Types, Variables, and Arithmetic

*A declaration* is a statement that introduces an entity into the program. 

It specifies a type for the entity: 
• *A type* defines a set of possible values and a set of operations (for an object). 
• *An object* is some memory that holds a value of some type. 
• *A value* is a set of bits interpreted according to a type. 
• *A variable* is a named object. C++ offers a small zoo of fundamental

*unsigned* : non-negative integer (use for bitwise logical operations)

Floating-point numbers are recognized by a decimal point (2.13) or by an exponent (3e-2)

Integer literals are decimal by default. **0b** prefix indicates a binary (base 2) integer literal. (e.g 0b10101010)

### Logical Operators

x&y // bitwise and
x|y // bitwise or
x^y // bitwise exclusive or
''xy // bitwise complement
x&&y // logical and
x||y // logical or
!x   // logical not

Bitwise logical operator returns a result of the operand type for which the operation has been performed on each bit.

## Initialization

C++ offers a variety of notations for expressing initialization, such as:

```c++
double d1 = 2.3;
double d2 {2.3};
double d3 = {2.3};
complex<double> z = 1;
complex<double> z2 {d1, d2};
complex<double> z3 = {d1, d2};
vector<int> v {1,2,3,4,5,6};
```

The **{}** - list form saves you from conversions that lose information.

```c++
int x = 7.8 // becomes 7
int x {7.8} // error: floating-point to integer conversion
```

Conversions that lose information is called *narrowing conversions*, such as:

+ double to int
+ int to char

are allowed and implicitly applied when you use = but not {}. 
A constant **cannot** be left uninitialized. 

User defined types such as (string, vector, Matrix, etc) can be defined to be implicitly initialized using the keyword **auto**.

```c++
auto b = true;
auto ch = 'x';
auto i = 123;
auto d = 1.2;
auto z = sqrt(y);
auto bb {true}
```

Use auto when you have specific reasons including:

+ The definition is in a large scope where we want to make the type clearly visible to readers of our code.
+ We want to be explicit about a variable's range or precision (e.g double rather than float)

## Scope and Lifetime

A declaration introduces its name into a scope:
+ *Local scope:* A name declared in a function is called a *local name.* Its scope extends from its point of declaration to the end of the block in which its declaration occurs.
+ *Class scope:* A name is a called a *member name (class member name)* if it is defined in a class, outside any function, lambda or **enum class** . It starts from the opening { to the end of the declaration.
+ Namespace scope: A name is a called a *namespace member name* if it is defined in a namespace outside any function, lambda, class, or enum class. 

A name not declared not inside any other construct is called a *global name* and is said to be in the *global namespace.*

- An object created by **new** lives until destroyed by **delete**.
- Static variables live until the program terminates.
- Namespace object the point of destruction is the end of the program.

## Constants

C++ supports two notions of immutability (unable to change)

const: meaning roughly "I promise not to change this value." Mainly used so when passing functions using pointers and references without fear of it being modified. 

The value of const can be calculated at run-time. (It has the possibility to be computed at run time.)

constexpr: meaning roughly "to be evaluated at compile time". Mainly used to specify constants, and allow placement of data in read-only memory (where it is unlikely to be corrupted) and for performance. 

The value of constexpr must be calculated by the compiler. constexpr = "constant expression" (probability of compile-time)
```c++
constexpr double square(double x) { return x*x; }
constexpr double max1 = 1.4*square(17); // OK, 17 is a constanst expression
constexpr double max2 = 1.4*square(var); // error: var is not a const expression
constexpr double max3 = 1.4*square(var); // OK, may be eval at runtime
```

In some places, constant expressions are required by language rules.

## Pointers, Arrays, and References

The size of an array must be a constant expression.
A pointer is a variable that holds a memory address.
```c++
int x = 10;
int* ptr = &x; // holds the address of x (pointing to x)
int value_of_x = *ptr; // dereferencing ptr to access the value of x


// Another example 
// Allocating memory for a 1D array
int* array = (int*)malloc(sizeof(int) * 28);
if(!array) {
	printf("failed to allocate memory for array\n");
	return -1; // exit(), terminate()
}
for(int i = 0; i < 28; ++i) {
	array[i] = i;
	printf("%d\n", array[i]);
}

free(array);
```

Postfix operators can be slower than prefix operators since with postfix it has to copy the value first and then increment/decrement that said value

unary operators are a single expression such as:
+ `*` means contents of and the prefix unary `&` means address of
+ `&` unary suffix means "reference to". A reference cannot be made to refer to a different object after its initialization.

```c++
void sort(vector<double>& v); // sort v (v is a vector of doubles)
```

We ensure that we do not copy `my_vec` and that it really is `my_vec` that is sorted and not a copy of it.

When we don't want to modify an argument but still won't want the *cost* of copying, we use a const reference.

```c++
double sum(const vector<double>&);
```

When used in declarations, operators (such as `&`, `*`, and `[]`) are called *declarator operators*.

T a[n]
T* p
T&
T f(A)

There is no null reference. A reference must refer to a valid object.
## Null Pointer

We try to ensure that a pointer always points to an object so that dereferencing is valid. When there is no object to point to or if we need to represent the notion of "no object available" (e.g at the end of a list), we give the pointer the value of **nullptr**.

```c++
double* pd = nullptr;
Link<Record>* list = nullptr;
int x = nullptr;
```

It's good to check that pointer argument actually points to something:

```c++ 
int count_x(const char* p, char x) {
	if(p == nullptr)
		   return 0;
	int count = 0;
	for(; *p!=0; ++p) 
		if(*p == x) 
			++count;
	return count;
}
```

**nullptr** eliminates the confusion between integers (such as 0 or NULL)

## Tests

A **switch**-statement tests a value against a set of constants. Those constants, called **case labels**, must be distinct, and if the value tested **does not** match any of them, the **default** is chosen. 

If the value doesn't match any case-label and there isn't any default declared, no action is taken. 

```c++
void action() {

	while(true) {
		std::cout << "Enter action:\n";
		std::string act;
		std::cin >> act;
		Point delta {0,0};
		for(char ch : act) {
			switch(ch) {
				case 'u':
				case 'n':
					++delta.y;
					break;
				case 'r':
				case 'e':
					++delta.x;
					break;
				default:
					std::cout << "I freeze!\n";
			}
			move(current+delta*scale);
			update_display();
		}
	
	}
}
```

An if statement can introduce a variable and test it.
```c++
void f(std::vector<int>& v) {
	if(auto n = v.size(); n != 0) {
		// we get here IF n does not equal to 0	
		// n is in scope within this if statement
	}
}
```

The purpose of declaring a name in the condition of an if statement is to keep the scope of the variable limited to improve readability and minimize errors.

Common Case: (Testing a variable against 0 or nullptr)
```c++
void f(vector<int>& v) {
	if(auto n = v.size()) {
	
	}
}
```

## Mapping to Hardware

C++ offers a direct mapping to hardware. When you use on the fundamental operations, the implementation is what the hardware offers, typically a single machine operation.

Example:

Adding two *ints, x+y* executes an integer add machine instruction.

A pointer is represented in memory as a machine address. The basic machine model of C and C++ is based on computer hardware, rather than some form of mathematics.

## Assignment

An assignment of a built-in type is a simple machine copy operation.
```c++
int x = 2;
int y = 4;
x = y; // x becomes 3
```

Both objects are independent. If we change the value of y without affecting the value of x. Example: if we change `x=99` it will not change the value of y.

If we want different objects to refer to the same (shared) value, we MUST say so. (this give you the ability to pick and choose. freedom)

```c++
int x = 9;
int y = 29;
int *p = &x;
int *q = &y;
p = q; // p is now pointing to the address of q
```


Assigning a reference to a reference just copies the value from the object referred to.
```c++
int x = 9;
int y = 29;
int& r = x;
int& r1 = y;
r = r1; // is assigned the value of to what y is storing/pointing to:

// So any change to r1 wont change r
```

But you assign different references to the same memory location. (binding)

When accessing a value from a pointer you must use the declarator operator `*`. In a reference it is implicitly accessing a value.
```c++
int x = 9;
int* ptr = &x;
int value = *ptr;

int& r = x; // print out the value (implicitly deferences x)

```

## Initialization

Initialization differs from assignment. For an assignment to work correctly, the assigned-to object MUST have a value.

```c++
int x = 3;
int f = 9;
int z;
f = x; // OK: x has a value (FYI: f is equal to the value of x, not the address. We never declared that we wanted that type of control)
x = z; // ERROR: z has no value
```

The task of initialization is to make an uninitialized piece of memory into a valid object. 

For almost all types, the effect of reading from or writing to an uninitialized variable is undefined.

```c++
int x = 7;
int& r {x};
r = 7

int& r2;
r2 = 99;
```

Assigning a reference to a value, could lead to some unspecified memory location, it can result into bad results or a crash.

## Key Words I don't know

- thread context switching
- generic programming
## Sentences that caught my eye

- Having your code to be comprehensible, is the first step on the way to maintainability. 
- The number of errors in code correlates strongly with the amount of code and the complexity of the code.