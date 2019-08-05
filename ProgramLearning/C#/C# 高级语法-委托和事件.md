# 委托
## 何为委托
```
函数签名：
一个函数由这么几部分组成，函数名、参数个数、参数类型、返回值，函数签名由参数个数与其类型组成。
```
委托是包含相同前面和返回值类型的方法列表（包含单个方法的委托和C++的函数指针相似）。委托在我的个人理解里面就像是某种函数模板，可以用委托变量来代表同样签名的函数，从而可以将该类型的函数作为函数的参数或者作为函数的返回值使用。  
![](images/delegate-1.png)

## 委托声明和创建
```C# 
delegate void MyDel(int x); // 无返回值，带int参数
```
![](images/delegate-2.png)

```C#
MyDel delVar; // 声明
delVar = new MyDel(myInstObj.MyM1); // 利用实例方法进行初始化
MyDel delVar; // 声明
delVar = new MyDel(SClass.MyM1); // 利用静态方法进行初始化
/* 上述等价于 */
delVar = myInstObj.MyM1;
delVar = SClass.MyM1;
```
上述的初始化所得的delVar和dVar的实质是一个引用，分别指向内存中的委托对象。此时除了为委托分配内存，同时也会将第一个方法放入委托的调用列表中。
![](images/delegate-3.png)
委托变量可随意更换引用
```C#
MyDel delVar;
delVar = SClass.MyM1;
...
delVar = myInstObj.MyM1; // 更换了引用的委托
```

## 多播委托（组合委托）
使用 + 和 - 进行委托的组合，如：
```C#
MyDel delVar1 = SClass.MyM1;
MyDel delVar2 = myInstObj.MyM1;
MyDel delVar3 = delVar1 + delVar2;  // 
```
![](images/delegate-4.png)

增加委托方法
```C#
MyDel delVar1 = SClass.MyM1;
delVar += SC1.m3;
delVar += X.Act;
```
减少委托方法
```C#
MyDel delVar1 = SClass.MyM1;
delVar += SC1.m3;
delVar += X.Act;

delVar -= X.Act;
```
![](images/delegate-5.png)
