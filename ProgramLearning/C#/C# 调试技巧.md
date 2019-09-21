# 使用debugview进行调试
debugview 可以捕获程序中由TRACE(debug版本)和OutputDebugString输出的信息。支持Debug、Release模式编译的程序（即该软件捕获的是exe直接运行时，抛出的信息，而不是Visual Studio调试时的），甚至支持内核程序，而且能够定制各种过滤条件，让你只看到关心的输出信息，而且可以定制高亮显示的内容等等，非常方便。

debugview调试时只需要在需要输出debug信息的地方加入如下语句
```C#
Debug.WriteLine("DEBUG INFO"); // 输出debug信息，该模式的信息只会出现在debug模式编译后的程序中，而release模式不会出现

Trace.WriteLine("Trace INFO"); // 输出trace信息
```
这样在程序运行时打开debugview，就可以查看到对应的debug信息，调试起来很方便。