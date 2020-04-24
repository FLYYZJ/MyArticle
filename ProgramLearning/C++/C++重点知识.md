## C++静态 static关键字


## C++ const关键字
### 基本含义



### 参考
const注解：  
https://blog.csdn.net/wanghuiqi2008/article/details/25887029

## C++ typedef 关键字
1、typedef 在C/C++ 是用来重命名的关键字。  
2、结构体声明
```C++
typedef struct _CARATTR
{
    int m_nColor;
    int m_nBrand;
    string m_strCarNo;
}CARATTR, *pCARATTR;
CARATTR struCar1;
pCARATTR pStrCar2;
// CARATTR表示struct _CARATTR的一个对象，pStrCar2表示CARATTR的一个指针。
// 即在使用时可以用CARATTR代替 struct _CARATTR
```
3、用于回调函数与导出函数
```C++
typedef typename(*Func)(typename1, typename2, ...);
```