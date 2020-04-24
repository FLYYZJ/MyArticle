## C++静态 static关键字

1、隐藏变量（C/C++语言中）  
所有未加static前缀的全局变量和函数都具有全局可见性，其它的源文件也能访问。

2、变量内容持久化（C/C++语言中）  
存储在静态数据区的变量会在程序刚开始运行时就完成初始化，也是唯一的一次初始化。  
共有两种变量存储在静态存储区：全局变量和static变量，只不过和全局变量比起来，static可以控制变量的可见范围，说到底static还是用来隐藏的。  
作为static局部变量在函数内定义，它的生存期为整个源程序，但是其作用域仍与自动变量相同，只能在定义该变量的函数内使用该变量。**退出该函数后， 尽管该变量还继续存在，但不能使用它**。  

3、默认初始化值为0，因为static类型变量都存放在静态数据区，其内存中的字节默认值为0x00.

4、C++ 中的类成员（成员变量和成员函数）声明为static
- 静态数据成员是类的成员，而不是对象的成员
- static修饰的数据成员不在栈上分配内存而在静态数据段分配内存，static修饰的数据成员不能通过调用构造函数来进行初始化，因此static修饰的数据成员必须在类外进行初始化且只会初始化一次。
    ```c++
    #include<iostream>
    using namespace std;

    class Myclass
    {
    public:
        Myclass(int a = 0) :data1(a)
        {
            cout << "Myclass(int)" << endl;
        }
        void Show()
        {
            cout << "Show()" << endl;
            cout << data1<< " " << data2 << endl;
        }
        ~Myclass()
        {
            cout << "~Myclass()" << endl;
        }
    public:
        static int data3;
    private:
        int data1;
        static int data2;
    };

    //如果不对data2、data3初始化，编译会报错误
    //无法解析的外部符号 "public: static int Myclass::data1" (?data1@Myclass@@2HA)
    //无法解析的外部符号 "private: static int Myclass::data2" (?data2@Myclass@@0HA)
    int Myclass::data1 = 20;
    int Myclass::data2 = 30;
    //初始化方式为作用域+变量+初始值
    int main()
    {
        Myclass class1(10);
        class1.Show();
        cout << class1.data3 << endl;
        return 0;
    }
    ```
- 静态成员方法可以在类内或类外定义，但必须在类内声明；static成员方法没有this指针，所以不能直接引用非static数据成员或调用类的非static成员方法，**只能调用类的static成员数据和static成员方法**；static成员不是任何对象的组成，不依赖对象的调用所以**static成员方法不能被声明为const**，因为const只限定该类的对象；**static成员方法不能同时被声明为虚函数**。

- 在类外调用static成员方法和使用类的数据成员时只需要加上类的作用域即可。 

- 将某些和类紧密相关的全局变量和函数写到类里面，看上去像一个整体，易于理解和维护。  
想在同类的多个对象之间实现数据共享，又不要用全局变量，那么就可以使用静态成员变量。

## C++ const关键字
### 基本含义

### 1、顶层const 和 底层const
- 指针本身是否是常量以及所指向的对象是否是常量就是两个独立的问题。

顶层cosnt - 指针本身是常量
底层const - 指针指向的对象是常量

由内到外，由近到远规则
```C++
int a = 1;
int b = 2;
const int* p1 = &a;
int* const p2 = &a;
```
p1依次解读为：p1是个指针（*），指向一个int型对象（int），该对象是个常量（const）。 因此这是一个底层cosnt

p2依次解读为：p2是个常量（const），p2是个指针（*），指向一个int对象(int)。 因此这是一个顶层const


### 2、const修饰参数是为了防止函数体内可能会修改参数原始对象。
- 函数参数为指针：指针传递（pass-by-pointer）只会进行浅拷贝，拷贝一份指针给函数，而不会拷贝一份原始对象。因此，给指针参数加上顶层const可以防止指针指向被篡改，加上底层const可以防止指向对象被篡改。
- 函数参数为引用：引用传递（pass-by-reference）有一个很重要的作用，由于引用就是对象的一个别名，因此不需要拷贝对象，减小了开销。这同时也导致可以通过修改引用直接修改原始对象（毕竟引用和原始对象其实是同一个东西），因此，大多数时候，推荐函数参数设置为pass-by-reference-to-const。给引用加上底层const，既可以减小拷贝开销，又可以防止修改底层所引用的对象。

### 3、const成员函数
将const实施与成员函数上，只要是防止成员函数修改类对象的内容。  
如果const成员函数想修改成员变量值，可以用mutable修饰目标成员变量。


## C++ 拷贝构造


## C++ 指针
