# 空间配置器

## 空间配置器的标准接口

根据 STL 的规范，以下是 allocator 的必要接口

| 接口 | 解释 |
| --  | --  |
| `allocator::value_type` | | 
| `allocator::pointer`    | |
| `allocator::const_pointer` | | 
| `allocator::reference`  |  |
| `allocator::const_reference` | | 
| `allocator::size_type` | | 
| `allocator::difference_type` | | 
| `allocator::rebind`| 一个嵌套的 `class template class rebind<U\>` 拥有唯一成员 `other`，那是一个`typedef`, 代表 `allocator<U\>` |
| `allocator::allocator()`| 默认构造函数 |
| `allocator::allocator(const allocator&)` | 拷贝构造函数| 
| `template <class U> allocator::allocator(const allocator<U>&)`| 泛化的拷贝构造函数|
| `allocator::~allocator()`|默认析构函数|
| `pointer allocator::address(reference x) const` | 返回某个对象的地址。 `a.address(x)` 等同于 `&x`| 
| `const_pointer allocator::address(const_reference x) const`| 返回某个 `const` 对象的地址，`a.address(x)` 等同于 `&x` |
| `pointer allocator::allocate(size_type n, const void*  = 0)` | 配置空间，足以存储 `n` 个 `T` 对象。第二参数是一个提示，实现上可能会利用他来增进区域性(locality)，或完全忽略之 |
| `void allocator::deallocate(pointer p, size_type n)` | 归还先前配置的空间 |
| `size_type allocator::max_size() const` | 返回可成功配置的最大量 |
| `void allocator:construct(pointer p, const T& x)` | 等同于 `new(const void* p) T(x)`|
| `void allocator::destroy(pointer p)`| 等同于 `p->~T()`|

## 设计一个简单的空间配置器 `JJ::allocator`

详情见 `chapter2/jjalloc` 中的代码

## 具备次配置力(sub-allocation)的SGI空间配置器

虽然 SGI 也定义有一个符合部分标准、名为 allocator 的配置器，但 SGI 自己从未用过它，也不建议我们使用。主要原因是效率不佳，只把 C++ 的 `::operator new` 和 `operator::delete` 做一层薄薄的包装而已。 

### SGI 特殊的空间配置器 `std::alloc`

为了精密分工，STL allocator 将内存的配置与释放和对象内容的构造与析构区分开来。内存配置操作由 `alloc::allocate()` 负责，内存释放操作由 `alloc::deallocate()` 负责。对象构造操作由 `::construct()` 负责。对象析构操作由 `::destroy()` 负责。STL 标准规格告诉我们，配置器定义于 `<memory>` 之中， SGI `<memory>` 内含以下两个文件：

```c
#include <stl_alloc.h>      // 负责内存空间的配置与释放 
#include <stl_construct.h>  // 负责对象内容的构造与析构
```

![对象的构造与析构](/docs/assets/chapter2/2-1.png)

### 对象的构造与析构

`<stl_construct.h>` 的代码详见 `chapter2/stl_construct.h`

代码的主要流程由图 2.2 所示：

![构造和析构示意图](/docs/assets/chapter2/2-2.png)

STL 规定配置器必须有名为 `construct()` 和 `destroy` 两个成员函数，然而 SGI STL 中的 `std::alloc` 配置器并未遵守这一规则。

`destroy` 函数中，将 `[first, last)` 范围内的所有对象析构掉，如果这个范围很大，而
每个对象的析构函数都无关痛痒的话 (有 trivial destructor)，那么多次调用析构函数会对
效率是一种伤害。

### 空间的配置与释放 `std::alloc`

对象构造前的空间配置和对象析构后的空间释放，由 `<stl_alloc.h>` 负责，SGI对此的设计
哲学如下：

- 向 `system heap` 要求空间
- 考虑多线程 (multi-threads) 状态
- 考虑内存不足时的应变措施
- 考虑过多 "小型区块" 可能造成的内存碎片 (fragment) 问题。

考虑到小型区块所可能造成的内存破碎问题，SGI设计了双层级配置器，第一级配置器(配置区块超过 128bytes 时)直接使用`malloc()` 和 `free()`，第二级配置器则视情况采用不同的策略，为了降低额外负担便采用复杂的 memory pool 整理方式。

```cpp
#ifdef__USE_MALLOC
...
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc; // 令 alloc 为第一级配置器
# else 
...
// 令 alloc 为第二级配置器
typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> alloc;
# endif /* ! __USE_MALLOC */
// alloc 并不接受任何 template 类型参数!!!!
```

SGI 会为配置器再包装一个接口，使得其接口能够符合 STL 规格

```cpp
template <class T, class Alloc>
class simple_alloc{
public:
    static T *allocate(size_t n) 
        {return 0 == n? - : (T*) Alloc::allocate(n * sizeof(T)); }
    static T *allocate(void)
        {return (T*) Alloc:allocate(sizeof(T)); }
    static void deallocate(T *p, size_t n) 
        { if (0 != n) Alloc::deallocate(p, n * sizeof(T)); }
    static void deallocate(T* p)
        { Alloc::deallocate(p, sizeof(T)); }
};

```

SGI STL 容器全部使用这个 `simple_alloc` 接口，例如

```cpp
template <class T, class Alloc = alloc> // 缺省使用 alloc 为配置器
class vector {
protected:
    // 专属之空间配置器，每次配置一个元素的大小
    typedef simple_alloc<value_type, Alloc> data_allocator;

    void deallocate() {
        if (...)
            data_allocator::deallocate(start, end_of_storage - start);
    }
    ...
};
```

![第一级配置器和第二级配置器](/docs/assets/chapter2/2-3.png)

![第一级配置器和第二级配置器的包装接口和运用方式](/docs/assets/chapter2/2-4.png)

具体代码详见 `/chapter2/stl_alloc.h`

第一级配置器实现出类似 `c++ new-handler` 的机制，他不能直接运用 c++ `new-handler`机制，因为它并非使用 `::operator new` 来配置内存。这个原因一部分是历史因素，另一个就是 c++ 并未提供相应于 `realloc()` 的内存配置操作。因此 SGI 不能直接使用 C++ 的 `set_new_handler()`, 必须仿真一个类似的 `set_malloc_handler()`。

SGI 第二级配置器的做法是：如果区块超过 128 bytes 时，就移交第一级配置器处理。当区块小于 128 bytes 时，则以内存池管理（又称为sub-allocation）：每次配置一大块内存，并维护对应的自由链表(free-list)。下次若再有相同大小的内存需求，就直接从 free-list 中取出。如果客端释放小额区块，就由配置器回收到free-list中。为了方便管理，SGI第二级配置器会主动将小额区块的内存需求量上调至8的倍数，并维护16个free-lists，各自管理大小分别为 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128 bytes的小额区块，free-list 的节点结构如下：

```cpp
union obj {
    union obj* free_list_link;
    char client_data[1];  /* The clinet sees this. */
};
```

使用 `union`可以避免为了维护链表所必需的指针而造成内存的一种浪费。

![自由链表的实现技巧](/docs/assets/chapter2/2-5.png)

区块自 free list 调出的操作如图所示

![freelist 调出操作](/docs/assets/chapter2/2-6.png)

区块回收纳入 freelist 操作如下图所示

![freelist 回收操作](/docs/assets/chapter2/2-7.png)

内存池的操作流程：

![内存池的操作流程](/docs/assets/chapter2/2-8.png)

## 内存基本处理工具

STL 定义有五个全局函数，作用于未初始化空间上。这样的功能对于容器的实现很有帮助。

| 全局函数 | 对应高层次函数|
| - | - |
| `construct()` | - |
|`destroy()`| - |
|`uninitialized_copy()`|`copy()`|
|`uninitialized_fill()`|`fill()`|
|`uninitialized_fill_n()`|`fill_n()`|

POD 意指 Plain Old Data，也就是标量型别(scalar types)或传统的 C struct 型别。POD 型别必然有`trivial ctor/dtor/copy/assignment` 函数，因此，我们可以对 POD 型别采用最有效率的复制手法，而对 non-POD 型别采取最保险安全的做法。

以图形显示本节三个函数对效率的特殊考虑:

![三个内存基本函数的泛型版本与特化版本](/docs/assets/chapter2/2-9.png)