# unicode编程问题
1、使用TCHAR来定义宽字节字符
```C++
TCHAR *szContent = TEXT("我是yzj");
static TCHAR szCaption[] = TEXT("桌面系统");  // 存放到静态区
TCHAR = WCHAR = wchar_t  // Unicode方案就是宽字符，多字节方案即为普通字符
```

