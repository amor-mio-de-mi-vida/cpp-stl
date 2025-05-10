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

## SGI 特殊的空间配置器 `std::alloc`

为了精密分工，STL allocator 将内存的配置与释放和对象内容的构造与析构区分开来。内存配置操作由 `alloc::allocate()` 负责，内存释放操作由 `alloc::deallocate()` 负责。对象构造操作由 `::construct()` 负责。对象析构操作由 `::destroy()` 负责。STL 标准规格告诉我们，配置器定义于 `<memory>` 之中， SGI `<memory>` 内含以下两个文件：

```c
#include <stl_alloc.h>      // 负责内存空间的配置与释放 
#include <stl_construct.h>  // 负责对象内容的构造与析构
```
![对象的构造与析构](/docs/assets/2-1.png)