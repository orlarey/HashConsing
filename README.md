![C/C++ CI](https://github.com/orlarey/HashConsing/actions/workflows/ubuntu.yml/badge.svg)![C/C++ CI](https://github.com/orlarey/HashConsing/actions/workflows/macos.yml/badge.svg)![C/C++ CI](https://github.com/orlarey/HashConsing/actions/workflows/windows.yml/badge.svg)


# Hash Consing in C++
## Principle
Hash consing is a technique to avoid storing duplicate contents in memory. The idea is to store a unique copy of the content in a table and then use a pointer to that content, instead of the content itself. 

For that purpose, two header files are provided: `HashConsing/HashFunction.hh` and `HashConsing/HashTable.hh`. 

The header file `HashFunction.hh` provides a rather generic hash function `HashConsing::HashFunction(const T&) -> size_t`. If `std::hash<T>` is defined, it uses it, otherwise, it provides hash functions for `std::pair`, `std::tuple`, `std::vector` as well as for flat user-defined classes and structures.


The header file `HashTable.hh` provides a generic hash table mechanism `HashConsing::HashTable<T>`, an object that transforms a content of type `T` into a pointer `T*` to that content, while keeping an internal copy of that content in a table.

Let's say we have the following fragment of code:

	#include "HashTable.hh"
	...
	HashConsing::HashTable<T> H;
	const T* p1 = H(c1);
	const T* p2 = H(c2)

Then `c1 == c2` implies `p1 == p2`, and `c1 != c2` implies `p1 != p2`.

In other words, the hash table will always return the same pointer for the same content, and two different pointers will always point to different contents. Knowing the content `c` it is possible to retrieve the pointer `p` to that content by calling `H(c)`. Having two pointers `p1` and `p2` it is very efficient to check if they point to the same content by comparing `p1 == p2`. If the pointers are different, then the contents are different.

## How to use it

Just copy the header folder `HashConsing` into your project, it has no dependencies other than the STL. Please note that it requires C++20 because it uses concepts.

Everything is encapsulated in the namespace `HashConsing`. It contains the class template `HashTable<T>` whose main operation is `operator()(const T&) -> T const*`. If the content `T` has never been seen before, an internal copy is first made. Then a pointer to the internal copy is returned. It has an additional `report()` method used to print statistics about the hash table, in particular the number of collisions. 

The namespace contains two additional classes, a default hash function  `HashConsing::HashFunction<T>`, and a default equality function `HashConsing::Equality<T>` so that the user doesn't have to provide them in many cases.

## Optional template parameters

The hash table can be parameterized with an optional hash function and an optional equality function: `HashConsing::HashTable<T[,H[,E]]>`. The default hash function is `HashConsing::HashFunction<T>` and the default equality function is `HashConsing::Equality<T>`. The default Hash function will call the standard `std::hash` function if it is defined for `T`, or provide a custom hash function otherwise. The default equality function will use the standard `operator==` if it is defined for `T`, or provide a custom equality function otherwise. The custom hash function and equality function will work for _flat_ structures that can be assimilated to a sequence of bytes such that `sizeof(T) == n*sizeof(int)`. For other use cases, the user must provide a specific hash function and equality function.

## Memory management

The hash table is a kind of memory manager per se. To always return the same pointer for the same content, the hash table keeps track of any content it has seen by storing a copy of it. The returned pointer is a const pointer to this _internalized_ copy. The pointer will remain valid until the hash table is deleted.

When the hash table is deleted, all the internalized values are also deleted and the pointers returned by the hash table are no longer valid.

