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
```