## CDC 和 HDC， 以及获取某个控件的HDC
- HDC是WINDOWS的一种数据类型，是设备描述句柄。而CDC是MFC里的一个类，它封装了几乎所有的关于HDC的操作。
- HDC定义的变量指向一块内存，这块内存用来描述一个设备的相关的内容，所以也可以认为HDC定义的是一个指针；而CDC类定义一个对象，这个对象拥有HDC定义的一个设备描述表，同时也包含与HDC相关的操作的函数。  
- GDI+是需要获取窗口的HDC句柄之后才能确定绘图位置

```c++
CWnd *pWin = GetDlgItem(IDC_STATIC);//获取该控件的指针，就可以对该控件直接操作了
CDC *pDc = pWin->GetDC();//获取该控件的画布
HDC hdc = pDc->GetSafeHdc(); // 获取HDC
...
// 该graphics就可以对hdc内容进行编辑
Gdiplus::Graphics graphics(hdc);
// 
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

### GDI+ 保存bitmap
参考： http://www.voidcn.com/article/p-sclqskcb-pp.html
```C++
// 转码
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
  UINT  num = 0;          // number of image encoders
  UINT  size = 0;         // size of the image encoder array in bytes

  ImageCodecInfo* pImageCodecInfo = NULL;

  Gdiplus::GetImageEncodersSize(&num, &size);
  if(size == 0)
    return -1;  // Failure

  pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
  if(pImageCodecInfo == NULL)
    return -1;  // Failure

  Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

  for(UINT j = 0; j < num; ++j)
  {
    if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
    {
      *pClsid = pImageCodecInfo[j].Clsid;
      free(pImageCodecInfo);
      return j;  // Success
    }    
  }

  free(pImageCodecInfo);
  return -1;  // Failure
}
// 保存图片
BOOL SaveHDCToFile(HDC hDC, LPRECT lpRect)
{   
  BOOL bRet = FALSE;
  int nWidth = lpRect->right - lpRect->left;
  int nHeight = lpRect->bottom - lpRect->top;

  //将目标区域贴图到内存BITMAP
  // CreateCompatibleDC 创建一个兼容hDC的DC
  HDC memDC = CreateCompatibleDC(hDC); 
  // CreateCompatibleBitmap
  HBITMAP hBmp = CreateCompatibleBitmap(hDC, nWidth, nHeight);
  // 将一个对象选择到指定的设备上下文（DC）中，且hBmp会替换掉memDC中的对象
  SelectObject(memDC, hBmp);
  // bitblt函数的首个参数是要转换后的目标hdc，而后续的hdc参数是转换前的hdc，其它参数代表目标hdc的左上角坐标和长宽（代表矩形）
  // 最后一个参数是背景色选择，可以选WHITNESS和BLACKNESS，这里是渐变色
  BitBlt(memDC, lpRect->left, lpRect->top, nWidth, nHeight,
    hDC, 0, 0, SRCCOPY);

  //保存成文件
  {
    //L"image/bmp" L"image/jpeg"  L"image/gif" L"image/tiff" L"image/png"
    CLSID pngClsid;
    GetEncoderClsid(L"image/bmp", &pngClsid);//此处以BMP为例，其它格式选择对应的类型，如JPG用L"image/jpeg" 

    Gdiplus::Bitmap *pbmSrc = Gdiplus::Bitmap::FromHBITMAP(hBmp, NULL);
    if( pbmSrc->Save(L"C:\\test.bmp", &pngClsid) == Ok)
    {
      bRet = TRUE;
    }
    delete pbmSrc;
  }

  //清理工作
  SelectObject(memDC, (HBITMAP)NULL);
  DeleteDC(memDC);  
  DeleteObject(hBmp);

  return bRet;
}
```