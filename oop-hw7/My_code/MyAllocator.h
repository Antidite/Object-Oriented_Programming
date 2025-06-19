#ifndef _MyALLOCATOR_H
#define _MyALLOCATOR_H

#include<vector>
#include<iostream>
#include"MyMemoryPool.h"

//T data type,N max bytes 
template<class T,unsigned N = 256>
class allocator
{
    public:
    typedef void _Not_user_specialized;
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::true_type propagate_on_container_move_assignment;
    typedef std::true_type is_always_equal;
    unsigned MAX_BYTES = N;

    allocator() noexcept{};
    template <class U>
    allocator(const allocator<U,N>&) noexcept{};
    ~allocator() noexcept{};
    template<class U>
    struct rebind { using other = allocator<U, N>; };

    pointer address(reference _Val) const noexcept
    {
        return std::addressof(_Val);
    }
    const_pointer address(const_reference _Val) const noexcept
    {
        return std::addressof(_Val);
    }
    void deallocate(pointer _Ptr, size_type _Count)
    {       
        if(_Ptr == nullptr || _Count == 0)
            return;
        size_type bytes = _Count * sizeof(value_type);
        MemoryPool::instance().deallocate(_Ptr, bytes);
        return;
    };
    pointer allocate(size_type _Count)
    {
        size_type bytes = _Count * sizeof(value_type);
        void* p = MemoryPool::instance().allocate(bytes);
        if (p == nullptr)
        {
            throw std::bad_alloc();
        }
        return static_cast<pointer>(p);
    };
    template<class _Uty> 
    void destroy(_Uty *_Ptr)
    {
        _Ptr->~_Uty();
    };
    template<class _Objty, class... _Types>
    void construct(_Objty *_Ptr, _Types&&... _Args)
    {
        ::new (static_cast<void*>(_Ptr)) _Objty(std::forward<_Types>(_Args)...);
    };
};

//Guarantee allocator<T, N> is equivalent between different types
template <class T, class U, int N>
bool operator==(allocator<T, N> const&, allocator<U, N> const&) noexcept {
    return true;
}
template <class T, class U, int N>
bool operator!=(allocator<T, N> const& a, allocator<U, N> const& b) noexcept {
    return !(a == b);
}

template<class T>
using StudentAllocator = allocator<T,256>;

#endif