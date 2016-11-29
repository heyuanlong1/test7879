stl_alloc.h 定义了空间配置器
stl_construct.h 定义了用于对象构造与析构的基本函数
stl_uninitialized.h 定义了内存管理的基本函数
stl_algobase.h 定义了各种基本算法
stl_config.h 是SGI STL 的环境组态文件
stl_iterator.h 定义了iterator的各种类型
auto_ptr的源码在头文件<memory>中


vertor维护一个单向开口的连续线性空间，提供的Random Access Iterators。
list是环形双向链表，它提供的是Bidirectional iterators。
deque维护一个双向开口的连续线性空间（由一段段的定量连续空间构成），提供的Random Access Iterators。

priority_queue默认就是最大堆，可以简单改造成最小堆。







iterator_traits负责萃取迭代器的特性，判断是单向，双向或random迭代器。（stl_iterator.h）

__type_traits则负责萃取型别的特性 ,判断是内嵌类型还是自定义类型。（type_traits.h）
如果是内嵌类型，内存处理就直接memcpy之类的，不用使用constructor和destructor，比如（stl_construct.h）




# if defined(__STL_NEED_BOOL)
    typedef int bool;
#   define true 1
#   define false 0
# endif
//原来#是可以空开来的！！！！！！！！
#ifdef 和 #if defined 的区别在于，后者可以组成复杂的预编译条件，比如

#if defined (AAA) && defined (BBB)
xxxxxxxxx
#endif

#if defined (AAA) || VERSION > 12
xxxxxxxxx
#endif

而#ifdef 就不能用上面的用法，也就是说，当你要判断单个宏是否定义时
#ifdef 和 #if defined 效果是一样的，但是当你要判断复杂的条件时，只能用 #if


普通函数也可以是内联函数
inline void neilian(){}

