# 侯捷老师视频自学笔记
视频地址：   
https://www.bilibili.com/video/av27135524/?p=2

## 其它细节
头文件的防卫式声明
```c++
#ifndef __COMPLEX__
#define __COMPLEX__
...

#endif
```

头文件常见布局：
```C++
// 前置声明，声明自己要写的类，或定义全局变量和函数、友元函数
class ostream;
class complex;
// 类声明和实现
class complex{
    ...
};
// 类函数定义
type complex::function(...){
    ...
}
```

### 值传递和引用传递（参数传递 返回值传递）
- 参数传递
    值传递 —— 传递副本，类对象值传递会涉及到拷贝构造函数的内容  
    引用传递 —— 传递地址（效率较高，尽量使用），一般要加上一个const的约束，来确保原始对象不被修改。

- 返回值传递
  返回值传递尽量也用引用传递，但是假如返回为局部变量（对象），则不能用传递引用的方式，因为出了函数作用域，此时局部变量（对象）会被释放，返回引用会出现错误（例如访问到野指针之类的情况）。  
  传递者无需知道接收者是以reference形式接收。

### 命名空间
```c++
namespace spacename{
    ...
}
...
using namespace spacename;  // 打开了该命名空间
using spacename::fucntion;  // 使用了某个函数
...
spacename::function...      // 每次调用都说明哪个命名空间
spacename::class...
```


## 面向对象编程基础
1、class分类：数据部分（数据成员）带指针 和 不带指针（多半不用析构函数）  

2、内联（inline）函数：直接在类本体中定义（实现）的函数，或者直接在外部定义函数实现的时候加上inline关键字
```c++
inline type complex::function(...){
    ...
}
```
inline的好处是会比较快，如果函数比较复杂，则不能成为inline函数，这个是否为inline函数是由编译器确定。

3、访问级别 - private， public， protect

### 4、构造函数
- 一些基础点  
    ```c++
    complex (double r=0, double i = 0): re(r), im(i) // 函数参数列表中带了默认值
    // 初始列写法（initialization list），比较正规
    // 在初始列中是初始化，在构造函数体中进行赋值的不是初始化，而是赋值，效率低（因为编译器还是做了一次初始化）
    {

    }
    ```
- 构造函数重载
    错误写法
    ```c++
    complex (double r=0, double i = 0): re(r), im(i)
    {

    }
    complex():re(0), im(0){

    }
    ...
    complex c1;
    complex c2(); // 编译器无法确定调用哪个构造函数
    ```

### 5、常量成员函数
如果不改变数据内容的函数，一般设置为const函数。因为假如我们定义了一个const的对象，一般这种类型的对象都是只读其中的数据成员，而读取数据的成员函数不是const类型，则在调用时编译器会报错，因为编译器认为该成员函数可能修改数据成员。  
```c++
double real() const {...}

...
const complex c1(2,1);
cout<<c1.real();  // 如果real函数没有const，则该句会报错，因为存在矛盾，c1中的值不可改，但real却是可能改变值
```

### 6、操作符重载
操作符重载可以写成 成员函数 或者 非成员函数  
```c++
complex& operator += (const complex&)  // 一般以传引用的方式，且加上const修饰符以确保原始传入的对象不被修改
inline complex& complex::operator += (const complex& r){
    this.re += r.re;
    this.im += r.im;
    return this;  // this指针指向调用函数的对象本身
}
```
操作符重载需要考虑链式调用的方式，此时就得考虑返回值类型和参数类型是否匹配，因此使用引用（比指针）会是更优的做法。
```c++
c2 += c1 += c0;
```
非成员函数的形式 - 此时不能返回引用，因为返回了局部变量（临时对象）
```c++
inline complex operator + (const complex& x, const complex& y){
    return complex (real(x) + real(y), imag(x) + imag(y));
}
// 重载 —— 考虑用户的多种可能用法 
inline complex operator + (const complex& x, double y){
    return complex (real(x) + y, imag(x));
}
....

// 正号 —— 通过重载的概念和加号分开
inline operator + (const complex& x){
    return x;
}

```
流相关操作符 —— 一定要写成全局非成员函数
```c++
// 这里的os不能设为const，因为<<往os传值时都会改变ostream（状态）
//ostream&返回类型还是因为要考虑链式调用的方式 cout << c1 << conj(c2);
ostream& operator << (ostream& os, const complex& x){
    return os << '(' << real(x) << ',' << imag(x) << ')';
}
```


### 7、友元函数
友元函数/类可以获得类的private成员，打破封装
```c++
friend complex& __doapl(complex*, const complex&);
```
相同class的各个object互为友元
```c++
class complex{

    int func(const complex& param){
        return param.re + param.im;
    }
};
...
complex c1(2,1);
complex c2;
c2.func(c1);  // 可以，因为互为友元
```

### 8、拷贝构造函数

```c++
// 成员中带指针则必须实现下面两个方法
String(const String& str);  // 拷贝构造
String& operator=(const String& str);  // 拷贝赋值
```
默认拷贝构造函数 (**如果不自定义拷贝构造函数，则必然会带一个默认拷贝构造函数**)，针对值类型成员，会直接将值类型赋值到待拷贝的对象上，而对于指针变量等引用类型成员，此时会将指针变量直接赋值到待拷贝对象上，因此两个对象的指针成员会指向同一个地址。


#### 深拷贝
所有的成员都搞一个新的副本，包括值类型和引用类型。  
拷贝赋值时，要注意检测自我赋值
```c++
inline String& String::operator=(const String& str){
    if(this == &str){  // 检测自我赋值，如果不考虑，则下面3步会出错
        return *this;
    }
    delete[] m_data; // string 类中的一个指针成员
    // 下面三步是常规的，分别是清空自身空间，重新分配空间，赋值
    m_data = new char[strlen(str.m_data) + 1];
    strcpy(m_data, str.m_data);
    return *this;
}
```
#### 浅拷贝
当类中带有指针成员：
- A拷贝构造一个新成员B，此时新成员B中的指针成员和原对象A的指针成员会指向同一地址；
- A拷贝赋值给一个成员B，此时B中的指针成员和A中的指针成员指向同一地址，而原先B的指针成员指向的内存没有释放，存在内存泄漏风险；
- 总结起来，浅拷贝可能存在的两个问题 —— 多次析构 和 内存泄漏

#### 一个在工作中错误使用了默认拷贝构造函数导致的错误
```c++
// CEmpParams类中有一个m_pDict的指针变量
void CEmpWasatchPrint::MakeSendData(int index, CEmpParams param)
{
	……
}
```
由于param是通用传参类的类对象，此时该函数没有指定用传引用的方式进行参数传递，因此是采用值传递的方式进行参数传递，而且CEmpParams没有定义自己的拷贝构造函数，因此会调用默认拷贝构造函数，导致原始的CEmpParams对象中的m_pDict这个指针变量成员会被赋给参数param中的m_pDict，此时两者的m_pDict指向同一个地址。而在MakeSendData函数结尾，由于要调用param已经结束了生命周期要进行析构，如下析构函数所示：
```c++
CEmpParams::~CEmpParams()
{
	delete m_pDict;	m_pDict= NULL;

	delete m_pCommon;	m_pCommon =NULL;
	if(NULL != m_pDictNote)
	{
		delete m_pDictNote;
		m_pDictNote = NULL;
	}
}
```
此时会提前释放掉m_pDict中的资源。当从函数MakeSendData回到调用函数后，调用函数运行到末尾，此时也要进行一次析构，此时因为m_pDict已经被释放，因此再次调用delete函数后会导致释放错误空资源进而程序崩溃。

## 栈 和 堆
栈（stack）：存在于某个作用域（scope）的一块内存空间，在函数本体中声明的任何变量，使用的内存块都保存到stack中。在栈中的对象在作用域结束后会自动调用析构函数。  

堆（system heap），是由操作系统提供的一块global内存空间，程序可动态分配（dynamic allocated）从中获得若干区块（block），new得到的对象都在堆中分配空间，而且必须手动delete，因为C++并不会主动析构。  
```C++
class Complex{...}
...
{
    Complex *p = new Complex; // 
} // 没有delete，此时p离开作用域后不再存在，此时无法在delete p，因此内存泄漏。
```
new的实质过程是先分配内存，在调用构造函数
```c++
void *mem = operator new(sizeof(Complex));  // 内部调用malloc函数
pc = static_cast<Complex*>(mem);  // 将void指针进行转型
pc->Complex::Complex(1,2); // 构造函数
```
delete函数的实质是先调用析构函数，再释放memory
```c++
String::~String(ps); // 调用析构函数，清楚对象中的指针成员空间
operator delete(ps); // 调用free(ps)
```
VC++要求分配空间的长度必须是16的倍数，因为后续内存分配和内存回收需要一个头部（cookie）来确定位置，而最后一位为1表示内存分配，最后一位为0表示内存回收。不足16倍数的空间长度会进行填充。

针对数组的new，此时分配的空间中会多一个4byte的空间用于存储数组的长度，然后再delete的时候，必须用数组delete的方式进行空间释放，否则会产生内存泄漏（泄漏点在于对象内部的指针成员没有被释放）：
```c++
String *p = new String[3];
...
delete[] p;  // 调用3次析构
```
如果不用 delete[] p， 此时会导致只调用一次析构，进而少释放剩余的空间。


静态局部对象（static object）的生命周期和整个程序一致，即当程序结束后静态局部对象才会被释放。
```c++
{
    static Complex c2(1,2);
    ...
} // 离开该scope后c2还是存在
```

全局对象（global object）

## 静态
静态数据和静态成员函数：  

- 静态数据只有1份，所有类对象共享。  
- 普通成员函数会传入一个this指针，用于确定调用的对象，这个this指针编译器会自动导入；  
    静态函数没有this指针传入，可以通过类名直接调用，也可以是由类对象进行调用
    ```c++
    class Account{
        public:
            static double m_rate;
            static void set_rate(const double& x){  // 一般用于控制静态类成员变量
                m_rate = x; // 
            }
    }
    double Account::m_rate = 8.0;
    Account::set_rate(5.0);
    Account a;
    a.set_rate(6.0);
    ```
    单例模式：
    ```c++
    class A{
        public:
            static A& getInstance{
                static A a;  // 使用才生成，且一直存在
                return a;
            }
            setup() {...}
        private:
            A();
            A(const A& rhs);
            // static A a;
    }
    ```


## OOP 和 OOD 的深入
### 继承（inherit， is a关系）
价值体现在和虚函数搭配实现功能重载或具化（多态），父类的析构函数必须是虚函数（virtual function）。
```c++
class A : public B{  // A 公有继承 B

}
```
#### 虚函数——任何函数定义前加virtual
```c++
class Shape{
public:
    virtual void draw() const = 0; // 纯虚函数，子类必须定义
    virtual void error(const std::string& message); // 父类有定义，但是子类可覆写
    int objectID() const;
}
```

基于虚函数的一种常用的设计模式——template method（模板方法）
```c++
CDocument::OnFileOpen(){
    ...
    // 父类的这个方法是虚函数方法，当子类调用了OnFileOpen到达Serialize后就会调用子类的Serialize，调用完毕后再返回父类的这个函数中继续调用下面的方法
    // 将固定部分写好，将根据业务不同而不同的函数构造成虚函数，然后留给子类去定义，子类调用父类函数时到达这一步就会调用自己定义的部分去执行
    // 当类对象调用方法时，编译器都会将表示类对象的this指针传到类方法中，此时子类调用OnFileOpen运行到Serialize方法时，实质时this->Serialize()，而this就是子类对象
    Serialize();  
    ...
}
... 
class CmyDoc: public CDocument{
    virtual Serialize(){...}
}
...
main(){
    CmyDoc myDoc;
    ...
    myDoc.OnFileOpen();   // 子类调用父类的方法
}
```


#### 构造和析构
- 构造由内到外：先构造父类的**默认构造函数**(注意当没有写明调用方式时是调用构造默认构造函数)，再调用自身的构造函数
    ```c++
        Derived::Derived(...) : Base(){...}
    ```
- 析构由外而内：先析构自己，再析构组件
    ```c++
        Derived::~Derived(...){ ... ; ~Base()}
    ```


### 组合（Composition，has a关系）
表示has a关系，表示某个类中有另一个类。如下例，此时queue中包含了deque（双端队列）类，关于queue的全部操作都有deque实现。同时这个类实现了adapter模式（适配器模式）
```c++
template<class T>
class queue{
    ...
protected:
    deque<T> c;  // 在创建类时同时创建，同时生成
public:
    bool empty const{return c.empty();}
    size_type size() const {return c.size();}
    ....
    void pop() {c.pop_front();}
}
```
#### 构造和析构
- 构造由内到外：先构造组件的**默认构造函数**(注意当没有写明调用方式时是调用构造默认构造函数)，再调用自身的构造函数
    ```c++
        Container::Container(...) : Component(){...}
    ```
- 析构由外而内：先析构自己，再析构组件
    ```c++
        Container::~Container(...){ ... ; ~Component()}
    ```

### 委托（delegation， Composition by reference）
在类中存在指向其他类对象的指针，这个指针就是委托，对象可以通过调用指针实现一些指针指向的对象所具备的功能。
```c++
class StringRep;
class String{
public:
    String();
    String(const char* s);
    String(const String& s);
    String &operator = (const String& s);
    ~String();
...
private:
    //pimpl（Handle 处理句柄）,这个就是委托，或者是通过引用的组合（指针实际也可类比为一种引用）
    // 在需要使用的时候才取new出来，不同步
    // 此时该string的具体实现可借由rep来实现，而rep可以是多种多样的，也就是说其实string提供了对外的接口，而真正的实现都委托给了rep进行实现
    // 实现引用计数的基础，默认拷贝构造时会将该指针赋给不同的对象中的rep，从而有多个对象都指向rep中所指向的字符数组，实现共享概念。
    // copy on write技术，在修改共享内容且不想影响其它对象时会制造一个副本给对象，从而该对象不再共享这个字符串
    StringRep* rep; 
}

class StringRep{
    friend class String;
    StringRep(const char* s);
    ~StringRep();
    int count;
    char* rep;
};
```


### 转换函数 —— 代理模式相关
转换函数（某种语法糖）： 无返回类型（其实就是函数名），operator开头，可以是任意的type（自定义类型或者其它的内置类型）
```c++
class Fraction{
public:
    Fraction(int num, int den = 1):m_numerator(num), m_denominator(den) { }
    operator double() const{
        return (double)(m_numerator * 1.0 / m_denominator);
    }
private:
    int m_numerator;  // 分子
    int m_denominator;  // 分母
}

...
Fraction f(3, 5);
double d = 4 + f;  // 转成double
```

非显式单参数构造函数（non-explicit-one-argument ctor）  
one-argument：单实参（可以有默认值的参数，默认值参数不计在其中）  
```c++
class Fraction{
public:
    Fraction(int num, int den = 1):m_numerator(num), m_denominator(den) { }
    Fraction operator + (const Fraction& f){
        return Fraction(....);
    }
private:
    int m_numerator;  // 分子
    int m_denominator;  // 分母
}

...
Fraction f(3, 5);
// 编译器找到 + 号函数
// + 号后面的4会被编译器作为Fraction构造函数中的首个参数num，因为是den有默认值，这个就是非显式单参数构造函数的一个作用（将4转为Fraction）
Fraction d = f + 4;  
```
注意有时候 非显式单参数构造函数 和 转换函数 是不能共用的，会导致编译器无法知道调用哪个函数（**因为两种转换都是可行的-其它类型转成本类型 或 本类型转成其它类型**）。

显式单参数构造函数（explicit-one-argument ctor）
explicit 一般用于构造函数之前  
```c++
class Fraction{
public:
    explict Fraction(int num, int den = 1):m_numerator(num), m_denominator(den) { }
    operator double() {
        return (double)(m_numerator * 1.0 / m_denominator);
    }
    Fraction operator + (const Fraction &f){
        return Fraction(....);
    }
private:
    int m_numerator;  // 分子
    int m_denominator;  // 分母
}

...
Fraction f(3, 5);
// explict 表明构造函数不再支持隐式的转换，即不可再将double或int等类型转为Fraction
// 此时4无法转为Fraction，故下句报错
Fraction d = f + 4;  
```


### pointer-like classes （智能指针）
指针所允许的动作，pointer-like class都支持，并多了一些操作
```c++
template<class T>
class shared_ptr{
public:
    // 模拟了指针的动作
    T& operator*() const {
        return *px;
    }
    T* operator->() const {
        return px;
    }

    shared_ptr(T *p) : px(p) { }
private:
    T* px;  // 真正的指针
    long* pn;
    //.....

}

struct Foo {
    ...
    void method(void){  ...  }
}

shared_ptr<Foo> sp(new Foo);
// Foo f(*sp);  
sp->method(); // == px -> method 
```

迭代器（看课件）


### function-like classes 仿函数
函数的小括号部分其实是一个运算符（function call operator），只要设计的类或结构体重载了小括号，则该类或结构体也是一个函数。  
```c++
template<class T>
struct identity : public unary_function<T, T> {
    const T& operator() (const T& x) const {
        return x;
    }
}

template<class Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type> {  // 选择Pair的第一个值
    const typename Pair::first_type& operator() (const Pair& x) const {
        return x.first;
    }
}

template<class Pair>
struct select2nd  : public unary_function<Pair, typename Pair::second_type> {  // 选择Pair的第二个值
    const typename Pair::second_type& operator() (const Pair& x) const {
        return x.second;
    }
}

template<class T1, class T2>
struct pair{
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()){}
}
...
template<class Arg, class Result>
struct unary_function {
    typedef Arg argument_type;
    typedef Result result_type;
}
```






