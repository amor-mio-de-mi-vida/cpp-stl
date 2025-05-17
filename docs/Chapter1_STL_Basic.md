# STL 概论

## STL 的价值

就低层次而言，STL带给我们一套极具实用价值的零部件，以及一个整合的组织。除此之外，STL还带给我们一个高层次的、以泛型思维为基础的、系统化的、条理分明的“软件组件分类学“。从这个角度来看，STL是一个抽象概念库。

| Concept     | Description |
| ----------- | ----------- |
| Assignable      | 可被赋值 |
| Default Constructible   | 不需任何参数就可构造 |
| Equality Comparable | 可判断是否等同 |
| LessThan Comparable | 可比较大小|
| Regular | 正规 |
| Input Iterator | 具输入功能的迭代器 |
| Output Iterator | 具输出功能的迭代器 |
| Forward Iterator | 单向迭代器 |
| Bidirectional Iterator | 双向迭代器 |
| Random ACcess Iterator | 随机存取迭代器 |
| Unary Function | 一元函数 |
| Binary Function | 二元函数 |
| Predicate | 传回真假值的一元判断式 |
| Binary Predicate | 传回真假值的二元判断式 |
| sequence container | 序列式容器 |
| associative container | 关联式容器 |

STL的创新价值便在于具体叙述了上述这些抽象概念，并加以系统化。C++ classes 允许我们自行定义类，C++ templates允许我们将类参数化，藉由两者结合并透过 traits 编程技法，形成了 STL 的绝佳温床。

## STL 六大组件 功能与运用

STL提供六大组件，彼此可以组合套用

| 组件 | 解释 |
| --- | --- |
| 容器(containers) | 各种数据结构，如 `vector`, `list`, `deque`, `set`, `map` 用来存放数据。从实现的角度来看，STL容器是一种 class template, 就体积而言，这一部分很像冰山在海面下的比率。 |
| 算法(algorithms) | 各种常用算法比如 `sort`, `search`, `copy`, `erase`, ... ,从实现的角度来看，STL算法是一种 function template |
| 迭代题(iterators)| 扮演容器与算法之间的胶合剂，是所谓的"泛型指针", 共有五种类型，以及其他衍生变化。从实现的角度来看，迭代器是一种将 `operator*`, `operator->`, `operator++`, `operator--` 等指针相关操作予以重载的 class template。所有 STL 容器都附带有自己专属的迭代器。原生指针(native pointer)也是一种迭代器。|
|仿函数(functors)| 行为类似函数，可作为算法的某种策略(policy), 从实现的角度来看，仿函数是一种重载了 `operator()` 的 class 或 class template。一般函数指针可视为侠义的仿函数。|
| 配接器(adapters)| 一种用来修饰容器(containers) 或仿函数(functors)或迭代器(iterators)接口的东西，改变 `container` 接口者，称为 `container adapter`; 改变 `iterator` 接口者，称为 `iteratoradapter`。|
| 配置器(allocators) | 负责空间配置与管理，从实现的角度来看，配置器是一个实现了动态空间配置、空间管理、空间释放的 class template。|

![1-1](/docs/assets/chapter1/1-1.png)
