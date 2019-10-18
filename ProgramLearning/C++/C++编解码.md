# string 转 wstring， char * 转 wchar*（string.c_str() = char *, w_string.c_str() = w_char*）
方法1
```C++
#include <string>
#include <locale.h> 

// 需包含locale、string头文件、使用setlocale函数。
std::wstring StringToWstring(const std::string str)
{// string转wstring
    unsigned len = str.size() * 2;// 预留字节数
    setlocale(LC_CTYPE, "");     //必须调用此函数
    wchar_t *p = new wchar_t[len];// 申请一段内存存放转换后的字符串
    mbstowcs(p,str.c_str(),len);// 转换
    std::wstring str1(p);
    delete[] p;// 释放申请的内存
    return str1;
}

std::string WstringToString(const std::wstring str)
{// wstring转string
    unsigned len = str.size() * 4;
    setlocale(LC_CTYPE, "");
    char *p = new char[len];
    wcstombs(p,str.c_str(),len);
    std::string str1(p);
    delete[] p;
    return str1;
}


int main()
{
    std::string str = "我是SunboyL。";
    std::wstring str1 = StringToWstring(str);// string转换为wstring
    
    str1 = L"我是不是SunboyL。";
    str = WstringToString(str1);// wstring转换为string
    return 0;
}
```
方法2，使用内置的转换函数 MultiByteToWideChar   
https://docs.microsoft.com/zh-cn/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar?redirectedfrom=MSDN  
```C++
CStringW buf2StrW(const char*pbuf  ,int icode)
{
 int len = MultiByteToWideChar(icode, 0, pbuf, -1, NULL, 0);  
 wchar_t *wstr = new wchar_t[len];  
 memset(wstr, 0, len*sizeof(wchar_t));  // 分配内存 
 MultiByteToWideChar(icode, 0, pbuf, -1, wstr, len);  // 转成WCHAR
 CStringW cstrDestW = wstr;  
 delete[] wstr;  // 清理内存

 return cstrDestW;  // 转成CStringW
}
```


