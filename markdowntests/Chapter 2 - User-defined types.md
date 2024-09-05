## Introduction

Types that are built out of other types are called *user-defined types.* (Referred to *classes and enumerations*)

User-defined types are:

+ easier to use
+ less error prone
+ efficient for what they do as direct use of built-in types
## Classes

A constructor is guaranteed to be used to initialize objects of its class. (Eliminates the problem of uninitialized variables.)

A struct is simply a class with members **public** by default. You can define constructors and other member functions for a struct.
## Unions

A union is a struct in which all members are allocated at the same address so that the union occupies only as much space as its largest member.

A union holds a value for only one member at a time.

```c++
enum Type {ptr, num};

struct Entry {
	string name;
	Type t;
	Node* p;
	int i;
}

void f(Entry* pe) {
	if(pe->t == num) {
		cout << pe->i;	
	} else if(pe->t == ptr) {
		// 	
	}
}
```

The example above indicates that the members `p` and `i` are never used at the same time, so space is wasted. 

To recover, simply use a union:
```c++
union Value {
	Node* p;
	int i;
}

struct Entry {
	string name;
	Type t;
	Value v;
}
```

The correspondence between a *type field* (here, t) and the type held in a union is *error-prone*. 

To avoid errors:

- Enforce the correspondence by encapsulating the union and the type field in a class and offer access only through member function that use the union correctly.

The standard library type **variant**, can be used to eliminate most direct uses of unions. 

A **variant** stores a value of one of a set of alternative types.

```c++
variant<Node*, int> v; // either hold Node* or int
```

```c++
struct Entry {
	string name;
	variant<Node*, int> v;
}

void f(Entry* pe) {
	if(holds_alternative<int>(pe->v)) {
		cout << get<int>(pe->v);
	}
}
```

Variant is simpler and safer to use than a union.
## Enumerations

User-defined type to enumerate values:

```c++
enum class Color {red, violet, blue};
enum class Game {VALORANT, COD, ROCKLEAGUE};

Color col = Color::red;
Game gam = Game::COD;
```

Enumerations are used to represent a small set of integer values.
It makes code more readable and less error-prone than it would have been had the *symbolic (and mnemonic)* enumerator names not been used.

The **class** after the **enum** specifies that an enumeration is strongly typed and that its enumerators are scoped. 

Enum classes help prevent accidental misuses of constants. 

Enum class has only assignment, initialization, and comparisons defined. Since enums are user-defined types, we can define operators for it:

```c++
Traffic_light& operator++(Traffic_light& t) {
	switch(t) {
	case Traffic_light::green:  return t = Traffic_light::yellow;
	case Traffic_light::yellow: return t = Traffic_light::red;
	case Traffic_light::red:    return t = Traffic_light::green;
	}

	Traffic_light next = ++light;
	return next;
}
```

If you don't want to explicitly call the enum name to access its value you can simply declare an enum like so:

```c++
enum Color {red, green blue};
enum class ColorE {red, green blue};
Color c = red; 
ColorE ce = ColorE::red; // enum class has to be called by its enum name
```

## Keywords/Questions that I don't understand/have
- augment -> improve/ add more value to (make something greater)
- how are user-defined types less error prone if they are written by humans which often make errors? (it provides structure, abstraction, enforce invariants, etc)
- tagged unions
## Sentences that caught my eye

- C++ augments the built-in types and operations with a sophisticated set of *abstraction mechanisms*.


## Advice

+ Prefer class enums over plain enums to minimize surprises