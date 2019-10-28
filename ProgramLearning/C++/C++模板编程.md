# 类模板（class template）
```c++
template<typename T>
class complex{
    ...
    T re, im;
}
```


# 函数模板（function template）
c++标准库中的大部分算法都是基于函数模板实现
```c++
template<class T>
inline const T& min(const T&a, const T&b){
    return a < b ? a : b;  // T类型必须重载<号，否则会报错
}
```
编译器会进行实参推导，不需要指明传入参数的类型。

# 成员模板 （member template）
使构造函数更有弹性，父类可以粗略地表示子类，因为子类 is a 父类
```c++
template<class T1, class T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    template<class U1, class U2>  // 构造函数
    pair(const pari<U1, U2>& p):
     first(p.first), second(p.second) {}
};

// U1 和 U2 必须满足 为 T1 和 T2 或为它们的派生


pair<Derived1, Derived2> p;
pair<Base1 , Base2> p2(p);
```
up-cast 
```c++
template<typename _Tp>
class shared_ptr : public __shared_ptr<_Tp>{
    ...
    template<typename _Tp1>  // 实现up-cast的特性
    explicit shared_ptr(_Tpl* _p) : __shared_ptr<_Tp>(_p) {}
}

Base1* ptr = new Derived1; // up-cast， 父类在子类上头，向上转型，向父类转型
shared_ptr<Base1> sptr(new Derived1); // 模拟 up-cast
```

# 模板特化 —— 个人理解像C#上的泛型约束
## 全特化
泛化的模板下，想要针对某种特定的类型特别指定某种类型下的操作，因此会特化某种类型下的函数
```c++
template<class Key>
struct hash{}
// 如果指定了Key为char，int 或 long，则编译器会执行下述函数
template<>
struct hash<char> {  // 特化
    size_t operator() (char x) const { return x; }
}

template<>
struct hash<int> {  // 特化
    size_t operator() (int x) const { return x; }
}

template<>
struct hash<long> {
    size_t operator() (long x) const { return x; }
}

...

cout << hash(long)()(1000);
```
## 偏特化
 个数上的偏特化  
```c++
template<typename T, typename Alloc = ...>
class vector {
    ....
};

// 偏特化 T 为 bool，即当T指定为bool时，则会指定用下面执行
// 下面实现的原因是char耗费一个byte的空间，而bool只占用1个bit，则可以针对bool来实现一个vector
template<typename Alloc = ....>
class vector<bool, Alloc>{
    ...
};
```
范围上的偏特化  
```c++
template <typename T>
class C{
    ...
};
// 如果使用的是T类型的指针则使用下面的模板
template <typename T>  // template <typename U>
class C<T*> {
    ....
}
C<string> obj1; // 走第一个
C<string*> obj2; // 走第二个
```

# template template parameter 模板模板参数
