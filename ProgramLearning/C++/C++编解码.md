# string 转 wstring， char * 转 wchar*（string.c_str() = char *, w_string.c_str() = w_char*）
短字节转宽字节方法。  
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
    std::string str = "我是YZJ";
    std::wstring str1 = StringToWstring(str);// string转换为wstring
    
    str1 = L"我是不是YZJ";
    str = WstringToString(str1);// wstring转换为string
    return 0;
}
```
方法2，使用内置的转换函数 MultiByteToWideChar   
https://docs.microsoft.com/zh-cn/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar?redirectedfrom=MSDN  —— 里面主要介绍了icode的几种类型，根据icode类型进行转码  
常用的转码类型是CP_ACP，这个转码类型表示使用系统默认的编码格式对字符串进行转码，如中文系统会被转码为GBK格式
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


## 编码转换

UTF-8 转 GBK
```c++
string UTF8ToGBK(const char* strUTF8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
    wchar_t* wszGBK = new wchar_t[len+1];
    memset(wszGBK, 0, len*2+2);
    MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char* szGBK = new char[len+1];
    memset(szGBK, 0, len+1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    string strTemp(szGBK);
    if(wszGBK) delete[] wszGBK;
    if(szGBK) delete[] szGBK;
    return strTemp;
}

```
GBK 转 UTF-8
```C++
string GBKToUTF8(const char* strGBK)
{
    int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len+1];
    memset(wstr, 0, len+1);
    MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len+1];
    memset(str, 0, len+1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    string strTemp = str;
    if(wstr) delete[] wstr;
    if(str) delete[] str;
    return strTemp;
}
```
