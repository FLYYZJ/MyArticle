# 项目目录结构
作为一个工具包，werkzeug有着很严谨的文件结构。
```js
__init__.py
_compat.py：维护兼容性
_internal.py：提供了werkzeug中使用的一些常量和基类。
_reload.py：重新加载模块
datastructures.py：数据结构模块，用来存放整合数据的类。
exceptions.py：错误处理模块。用来存放各种错误类。
filesystem.py：文件系统模块。
formparser.py：表单解析模块。提供用于解析表单数据的工具函数。
http.py：用于处理HTTP中的数据的工具们。在wrapper.py中使用的比较多，但是这些工具函数往往单独使用作用更加显著，尤其是在没有请求和响应对象的前提下。
local.py：本地上下文对象。
posixemulation.py：为应用程序提供可移植作业系统接口（Portable Operating System Interface，POSIX）。
routing.py：路由匹配处理模块。
security.py：安全模块。
serving.py：服务运行模块。
test.py&testapp.py
url.py：处理解析URL的工具们。
useragents.py：用于处理用户代理的工具们。
utils.py：用于实现封装好的请求类和响应类的模块。对于中间件开发十分重要。
wrappers.py：封装好的各种类。包括著名的请求基类和响应基类和各种混入类。
wsgi.py：WSGI相关模块。

```



