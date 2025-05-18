#include "mylist.h"

// Item 可以是单向链表节点或双向链表节点，此处这个迭代器特定只为链表服务，因为
// 其独特的 operator++ 之故
template <class Item>
struct ListIter {
    Item* ptr; // 保持与容器之间的一个联系 (keep a reference to Container)

    ListIter(Item* p = 0) : ptr(p) {}  // default ctor

    // 不必实现 copy ctor，因为编译器提供的缺省行为已足够
    // 不必实现 operator=，因为编译器提供的缺省行为已足够

    Item& operator*() const { return *ptr; }
    Item* operator->() const { return ptr; }

    // 以下两个 operator++ 遵循标准做法，
    // (1) preincrement operator
    ListIter& operator++() {
        ptr = ptr->next();
        return *this;
    }

    // (2) post-increment operator
    ListIter& operator++(int) {
        ListIter tmp = *this;
        ++*this;
        return tmp;
    }

    bool operator==(const ListIter& i) const {
        return ptr == i.ptr;
    }

    bool operator!=(const ListIter& i) const {
        return ptr != i.ptr;
    }
};

