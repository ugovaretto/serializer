# SRZ - SeRialiZation framework
Small serialization framework to serialize data to byte array.

Look at test driver for usage example.

Specializations for `POD, vector, string, map, pair` exist as welll as for types that can be 
copy constructed with no memory allocation required.

By default the `Serializer<T>` implementation is invoked in case no other suitable
implementation is found.

In general you should always create a `GetSerializer<T>` specialization for
your own type `T`.

Creating a proper `GetSerializer` implementation is a two step process:

1.  Create a serialization class
2.  Create a `GetSerializer` struct which selects the proper serialization struct when invoked as `GetSerializer<T>::Type`

The interface for the serialization class is:

```c++
template< typename T >
struct MySerialize {
    //increase size of passed byte array, store data in it then return new copy
    static ByteArray Pack(const T& d, ByteArray buf = ByteArray());
    //store data into pre-allocated memory pointed to by byte iterator and
    //return iterator incremented to end address
    static ByteIterator Pack(const T& d, ByteIterator i);
    //read data from memory pointed to by iterator and return updated
    //iterator address
    static ConstByteIterator UnPack(ConstByteIterator i, T& d);
    //compute and return the size in bytes of the serialized data; e.g.
    //in case of a vector<int> V the compute size would be (assuming array size is serialized as size_t)
    //size = sizeof(size_t) + V.size() * sizeof(int) 
    static size_t Sizeof(const T& d);
};
```
`ByteArray` can be specified to be a `char` or `unsigned char` `std::vector`.
The serialized size type for `vector` and other elements requiring a length parameter can be set to be either `size_t` or `int32_t` for cases where data needs to be exchanged with languages (such as JavaScript or Java) lacking an 8 byte unsigned type.

`#define` directive exist to select `ByteArray` and `Size` type; from the `Serializer.h`header:

```c++
#ifdef ZRF_UNSIGNED_CHAR
using Byte = unsigned char;
#else
using Byte = char;
static_assert(sizeof(char) == 1, "sizeof(char) != 1");
#endif

#ifdef ZRF_int32_size
#include <cstdint>
using Size = int32_t;
#else
using Size = size_t;
#endif

//! Serialization framework
namespace srz {
using ByteArray = std::vector< Byte >;
using ByteIterator = ByteArray::iterator;
using ConstByteIterator = ByteArray::const_iterator;
```










