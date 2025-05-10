#ifndef _JJALLOC_
#define _JJALLOC_
#include <new>  // for placement new
#include <cstddef>  // for ptrdiff_t, size_t
#include <cstdlib>  // for exit()
#include <climits>  // for UINT_MAX
#include <iostream>  // for cerr


namespace JJ {

/* ptrdiff_t 是 C/C++ 标准库中定义的一个有符号整数类型，用于表示两个指针之间的差值（即指针的偏移量）。
 * 用于存储两个指针相减的结果, 也可以用来表示内存块的大小.
 * 是有符号的（可以是负数，表示指针向前或向后的偏移）
 * 具体大小取决于平台（通常是 int、long 或 long long 的别名）
 */
template <class T>
inline T* _allocate(ptrdiff_t size, T*) {

    /* 将内存分配失败的处理函数设置为空（null），
     * 这样当内存分配失败时会抛出 bad_alloc 异常而不是调用处理函数
     */
    std::set_new_handler(0);

    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
    if (tmp == 0) {
        std::cerr << "out of memory" << std::endl;
        exit(1);
    }
    return tmp;
}

template <class T>
inline void _deallocate(T* buffer) {
    ::operator delete(buffer);
}

template <class T1, class T2>
inline void _construct(T1* p, const T2& value) {
    /* 不分配新内存，而是在 p 指向的已有内存上构造 T1 对象。
     * 相当于手动调用构造函数（T1 的构造函数会在 p 的位置初始化对象）
     */
    new(p) T1(value); // placement new. invoke ctor of T1
}

template <class T>
inline void _destroy(T* ptr) {
    ptr->~T();
}

template <class T>
class allocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    // rebind allocator of type 
    template <class U>
    struct rebind {
        typedef allocator<U> other;
    };

    // hint used for locality. 
    pointer allocate(size_type n, const void* hint=0) {
        return _allocate((difference_type)n, (pointer)0);
    }

    void deallocate(pointer p, size_type n) { _deallocate(p); }
    void construct(pointer p, const T& value) {
        _construct(p, value);
    }

    void destroy(pointer p) { _destroy(p); }

    pointer address(reference x) {return (pointer)&x; }

    const_pointer const_address(const_reference x) {
        return (const_pointer)&x;
    }

    size_type max_size() const {
        return size_type(UINT_MAX/sizeof(T));
    }
};

} // end of namespace JJ

#endif // _JJALLOC_