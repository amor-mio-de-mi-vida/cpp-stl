#include <new>
#include <type_traits> // use std::false_type instead of __false_type

template <class T1, class T2>
inline void construct(T1* p, const T2& value) {
    new(p) T1(value); // placement new 
}

// 以下是 destroy() 第一个版本，接受一个指针
template <class T>
inline void destroy(T* pointer) {
    pointer->~T();  // 调用 dtor ~T();
}

// 以下是 destro() 第二个版本，接受两个迭代器，此函数设法找出元素的 value type，
// 进而利用 __type_traits<> 求取最适当措施
template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
    __destroy(first, last, value_type(first));
}

// 判断元素的 value type 是否有 trivial destructor
template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

// 如果元素的 value type 有 non-trivial destructor ...
template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, std::false_type) {
    for (; first < last; ++first)
        destroy(&*first);
}

// 如果元素的 value type 有 trivial destructor ...
template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, std::true_type) {}


// 以下为 destroy() 第二版本针对迭代器为 char* 和 wchar_t* 的特化版
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}

