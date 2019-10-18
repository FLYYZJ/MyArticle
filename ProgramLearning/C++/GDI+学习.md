## CDC 和 HDC， 以及获取某个控件的HDC
- HDC是WINDOWS的一种数据类型，是设备描述句柄。而CDC是MFC里的一个类，它封装了几乎所有的关于HDC的操作。
- HDC定义的变量指向一块内存，这块内存用来描述一个设备的相关的内容，所以也可以认为HDC定义的是一个指针；而CDC类定义一个对象，这个对象拥有HDC定义的一个设备描述表，同时也包含与HDC相关的操作的函数。  
- GDI+是需要获取窗口的HDC句柄之后才能确定绘图位置

```c++
CWnd *pWin = GetDlgItem(IDC_STATIC);//获取该控件的指针，就可以对该控件直接操作了
CDC *pDc = pWin->GetDC();//获取该控件的画布
HDC hdc = pDc->GetSafeHdc(); // 获取HDC
...
Gdiplus::Graphics graphics(hdc);
```


## 加载GDI+动态库和释放
在MFC的窗口程序中加载GDI+动态库，我们可以把加载部分放到stdafx.h文件中，具体为
```C++
// gdi+ 头文件
#include <GdiPlus.h> 
using namespace Gdiplus;
#pragma comment(lib, "GdiPlus.lib") 
```
而且需要在继承于WinApp的App类的InitInstance()函数中进行资源的初始化，然后在ExitInstance()函数中关掉GDI+的资源，如下：
```C++
// InitInstance()
GdiplusStartupInput gdiplusStartupInput;
GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
CWinApp::InitInstance();
// ExitInstance()函数
GdiplusShutdown(m_gdiplusToken);  // 关掉GDI+资源
```
