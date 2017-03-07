//
// Created by Ugo Varetto on 7/3/17.
//
#pragma once
#include <vector>

#ifndef SRZ_USE_ALLOCATOR_TRAITS

template<typename T>
class WrapAllocator {
public:
    using value_type = T;
public:
    WrapAllocator(T *basePtr, size_t n) :
            bp_(basePtr), maxSize_(n) {}

    T *allocate(size_t n, T* hint = 0) {
        return bp_;
    }
    void deallocate(T* p, size_t) {}
    size_t max_size() const { return maxSize_; }
    template < typename...ArgsT >
    void construct(void*, ArgsT...) {} //::new((void *)p) U(std::forward<Args>(args)...)
    void destroy(void*) {}
private:
    T *bp_;
    size_t maxSize_;
};
#else
template<typename T>
struct std::allocator_traits< WrapAllocator<T> > {
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = typename WrapAllocator<T>::value_type *;
    using const_pointer = const pointer;

    template<typename...ArgsT>
    static void construct(WrapAllocator<T> &, T *p, const ArgsT &...args) { a.construct(p, std::forward(args)...);}

    static void destroy(WrapAllocator<T>& a, T *p) { a.destroy(p); }

    static pointer allocate(WrapAllocator<T> &a, size_type n) { return a.allocate(n); }

    static pointer allocate(WrapAllocator<T> &a, size_type n, void*) { return a.allocate(n); }

    static void deallocate(WrapAllocator<T> &a, T *p, size_type n) { a.deallocate(p, n); }

    static size_type max_size(const WrapAllocator<T> &a) { return a.max_size(); }
};
#endif

///Wrap a buffer with an std::vector
template < typename T >
std::vector< T, WrapAllocator< T > > BufferToVector(T* buf, size_t size) {
    return std::vector< T, WrapAllocator< T > >(size, T(), WrapAllocator< T >(buf, size));
};
