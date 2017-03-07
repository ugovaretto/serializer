//
// Created by Ugo Varetto on 7/3/17.
//
#pragma once
#include <vector>


template<typename T>
class WrapAllocator {
public:
    using ValueType = T;
    using SizeType = size_t;
    using value_type = ValueType; //only required typedef from STL
public:
    WrapAllocator(T *basePtr, size_t n) :
            bp_(basePtr), maxSize_(n) {}

    T *Allocate(size_t n) {
        return bp_;
    }

    size_t MaxSize() const { return maxSize_; }

private:
    T *bp_;
    size_t maxSize_;
};

template<typename T>
struct std::allocator_traits< WrapAllocator<T> > {
    using size_type = typename WrapAllocator<T>::SizeType;
    using difference_type = ptrdiff_t;
    using pointer = typename WrapAllocator<T>::ValueType *;
    using const_pointer = const pointer;

    template<typename...ArgsT>
    static void construct(WrapAllocator<T> &, T *, const ArgsT &...) {}

    static void destroy(WrapAllocator<T> &, T *) {}

    static pointer allocate(WrapAllocator<T> &a, size_type n) { return a.Allocate(n); }

    static pointer allocate(WrapAllocator<T> &a, size_type n, void*) { return a.Allocate(n); }

    static void deallocate(WrapAllocator<T> &a, T *, size_type) {}

    static size_type max_size(const WrapAllocator<T> &a) { return a.MaxSize(); }
};


///Wrap a buffer with an std::vector
template < typename T >
std::vector< T, WrapAllocator< T > > BufferToVector(T* buf, size_t size) {
    return std::vector< T, WrapAllocator< T > >(size, T(), WrapAllocator< T >(buf, size));
};
