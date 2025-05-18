# 迭代器概念与 traits 编程技法
## 迭代器设计思维——STL关键所在
STL的中心思想在于：将数据容器(containers)和算法(algorithms)分开，彼此独立设计，最后再以一帖胶着剂讲他们撮合在一起，容器和算法的泛型化，如何设计出两者之间的良好胶着剂，才是大难题。
## 迭代器是一种 smart pointer
迭代器最重要的编程工作就是对 `operator *` 和 `operator->` 进行重载。C++标准程序库有一个 `auto_ptr`，这是一个用来包装原生指针的对象，声名狼藉的内存漏洞(memory leak)问题可藉此获得解决，`auto_ptr` 用法如下，和原生指针一模一样：

```cpp
void func() {
    auto_ptr<string> ps(new string("jjhou"));

    cout << *ps << endl;         // 输出：jjhou
    cout << ps->size() << endl;  // 输出：5
    // 离开前不需要 delete, auto_ptr 会自动释放内存
}
```