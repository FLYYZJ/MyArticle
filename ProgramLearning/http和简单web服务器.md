## HTTP 超文本传输协议
通信规则，规定客户端与服务端进行数据交互的内容格式。客户端发送给服务器的格式叫“请求协议”；服务器发送给客户端的格式叫“响应协议”。

特点：
1. HTTP是无连接：无连接的含义是限制每次连接只处理一个请求。服务器处理完客户的请求，并收到客户的应答后，即断开连接。采用这种方式可以节省传输时间。
2. HTTP是媒体独立的：这意味着，只要客户端和服务器知道如何处理的数据内容，任何类型的数据都可以通过HTTP发送。客户端以及服务器指定使用适合的MIME-type内容类型。
3. HTTP是无状态：HTTP协议是无状态协议。无状态是指协议对于事务处理没有记忆能力。缺少状态意味着如果后续处理需要前面的信息，则它必须重传，这样可能导致每次连接传送的数据量增大。另一方面，在服务器不需要先前信息时它的应答就较快。

请求协议的格式：
```
请求首行（请求方式，路径，协议和版本 例如 GET/index.html HTTP/1.1）
请求头（键值对形式，请求头名称，请求头内容，例如host:www.baidu.com,Accept-Encoding:gzip,deflate,sdch ）
空行 (/n/r)
内容主体（请求体）
```

请求头中几个字段的说明：
1. Referer: http://localhost:8080/hello/index.jsp 请求来自哪个页面，例如你在百度上点击链接到了这里，那么Referer:http://www.baidu.com； 如果你是在浏览器的地址栏中直接输入的地址，那么就没有Referer这个请求头了；
2. Content-Type: application/x-www-form-urlencoded：表单的数据类型，说明会使用url格式编码数据；url编码的数据都是以“%”为前缀，后面跟随两位的16进制。
3. Content-Length:13：请求体的长度，这里表示13个字节。
4. keyword=hello：请求体内容！hello是在表单中输入的数据，keyword是表单字段的名字。


请求方式（最常用2种）
1. GET：无请求体，数据量必须在1k之内，请求数据暴露在浏览器的地址栏中；
2. POST：数据不会出现在地址栏中（存放在请求体中），数据大小无上限，有请求体，中文会进行URL编码。


响应协议：
```
响应首行
响应头信息
空行
响应体
```
响应头： 协议 服务器版本 响应体编码 响应体长度 cookie信息 响应时间
状态码： 200（访问成功） 301（重定向） 400（无效页面） 500（服务器内部错误）
304（缓存相关）
```
当用户第一次请求index.html时，服务器会添加一个名为Last-Modified响应头，这个头说明了index.html的最后修改时间，浏览器会把index.html内容，以及最后响
应时间缓存下来。当用户第二次请求index.html时，在请求中包含一个名为If-Modified-Since请求头，它的值就是第一次请求时服务器通过Last-Modified响应头发送
给浏览器的值，即index.html最后的修改时间，If-Modified-Since请求头就是在告诉服务器，我这里浏览器缓存的index.html最后修改时间是这个,您看看现在的
index.html最后修改时间是不是这个，如果还是，那么您就不用再响应这个index.html内容了，我会把缓存的内容直接显示出来。而服务器端会获取If-Modified-Since值，
与index.html的当前最后修改时间比对，如果相同，服务器会发响应码304，表示index.html与浏览器上次缓存的相同，无需再次发送，浏览器可以显示自己的缓存页
面，如果比对不同，那么说明index.html已经做了修改，服务器会响应200。
```
![](https://images2015.cnblogs.com/blog/877318/201610/877318-20161026162455218-1166783413.png)


## web框架概念
特指为解决一个开放性问题而设计的具有一定约束性的支撑结构，使用框架可以帮你快速开发特定的系统(减少重复开发，减少冗余)。

统一接口用于实现 接受HTTP请求、解析HTTP请求、发送HTTP响应（对请求进行解析和封装，基于nginx、apache或WSGI server），WSGI（Web Server Gateway Interface）接口，不用重复编写socket编程代码。

### 简单的实现：运行后在浏览器中访问 127.0.0.1:8080实现访问。
```python
from wsgiref.simple_server import make_server
def application(environ,start_response):
    '''
    :param environ:
    :param start_response:
    :return:
    '''
    print(environ) # 请求头中的内容，一个字典
    start_response('200 ok',[('Content-Type','text/html'),('accept-encoding','gzip,dflate')]) # 设置请求头
    return [b'<h1>Hello web!</h1>']
httpd = make_server('',8080,application) # 监听到HTTP请求就会执行application函数
print('Serving HTTP on port 8080...')

httpd.serve_forever() # 持续监听HTTP请求
```
### 加入了路径判断的代码：
```python
from wsgiref.simple_server import make_server
def application(environ,start_response):
    '''
    :param environ:
    :param start_response:
    :return:
    '''
    print('path',environ['PATH_INFO'])
    start_response('200 ok',[('Content-Type','text/html'),('accept-encoding','gzip,dflate')]) # 设置请求头
    path = environ['PATH_INFO']
    if path == '/alex':
        return [b'<h1>Hello alex!</h1>']
    elif path == '/yuan':
        return [b'<h1>Hello yuan!</h1>']
    else:
        return [b'<h1>404 not found</h1>'] # 可以将return的内容修改成一个html文件，然后反馈这个html文件的内容就可以了（动态形式）
httpd = make_server('',8080,application) # 监听到HTTP请求就会执行application函数
print('Serving HTTP on port 8080...')
# 持续监听HTTP请求
httpd.serve_forever()
```
### 减少if-else块（解析路径）
```
from wsgiref.simple_server import make_server

def login():
    return [b'<h1>please login</h1>']
def register():
    return [b'<h1>please register</h1>']
def foo1():
    return [b'<h1>hello alex</h1>']
def foo2():
    return [b'<h1>hello yuan</h1>']

def router(): # 引入router，通过遍历来匹配路由
    url_patterns = [
        ('/login',login),
        ('/register',register),
        ('/yuan',foo2),
        ('/alex',foo1),
    ]
    return url_patterns

def application(environ,start_response):
    '''

    :param environ:
    :param start_response:
    :return:
    '''
    print('path',environ['PATH_INFO'])
    start_response('200 ok',[('Content-Type','text/html'),('accept-encoding','gzip,dflate')]) # 设置请求头
    path = environ['PATH_INFO']
    url_patterns = router()
    for item in url_patterns:
        if item[0] == path:
            return item[1]()
    return [b'<h1>404 not found</h1>']



httpd = make_server('',8080,application) # 监听到HTTP请求就会执行application函数

print('Serving HTTP on port 8080...')

# 持续监听HTTP请求
httpd.serve_forever()

```
### 较为完整的架构：
```python
from wsgiref.simple_server import make_server
def login(req = None):
    print(req['QUERY_STRING'])
    return [b'<h1>please login</h1>']
def register(req = None):
    return [b'<h1>please register</h1>']
def foo1(req = None):
    return [b'<h1>hello alex</h1>']
def foo2(req = None):
    return [b'<h1>hello yuan</h1>']
def router():
    url_patterns = [
        ('/login',login),
        ('/register',register),
        ('/yuan',foo2),
        ('/alex',foo1),
    ]
    return url_patterns

def application(environ,start_response):
    '''
    :param environ:
    :param start_response:
    :return:
    '''
    print('path',environ['PATH_INFO'])
    start_response('200 ok',[('Content-Type','text/html'),('accept-encoding','gzip,dflate')]) # 设置请求头
    path = environ['PATH_INFO']
    url_patterns = router()
    for item in url_patterns:
        if item[0] == path:
            return item[1](environ)
    return [b'<h1>404 not found</h1>']
httpd = make_server('',8080,application) # 监听到HTTP请求就会执行application函数
print('Serving HTTP on port 8080...')

# 持续监听HTTP请求
httpd.serve_forever()
```
login.html文件为：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>

<form action="http://127.0.0.1:8080/login" method="get">
    <p>user name<input type="text" name="user"></p>
    <p>user pwd<input type="text" name="pwd"></p>
    <p><input type="submit">submit</p>
</form>

</body>
</html>
```
### 加入了模板渲染
```python
from wsgiref.simple_server import make_server
import time
def login(req = None):
    print(req['QUERY_STRING'])
    return [b'<h1>please login</h1>']
def register(req = None):
    return [b'<h1>please register</h1>']
def foo1(req = None):
    return [b'<h1>hello alex</h1>']
def foo2(req = None):
    return [b'<h1>hello yuan</h1>']
def show_time(req = None): # 运用到了简单的模板渲染
    times = time.ctime()
    with open('showtime.html','r') as f:
        data = f.read()
        data = data.replace('{{time}}',str(times))
        return [data.encode('utf8')]
def router():
    url_patterns = [
        ('/login',login),
        ('/register',register),
        ('/yuan',foo2),
        ('/alex',foo1),
        ('/show_time',show_time),
    ]
    return url_patterns
def application(environ,start_response):
    '''
    :param environ:
    :param start_response:
    :return:
    '''
    print('path',environ['PATH_INFO'])
    start_response('200 ok',[('Content-Type','text/html'),('accept-encoding','gzip,dflate')]) # 设置请求头
    path = environ['PATH_INFO']
    url_patterns = router()
    for item in url_patterns:
        if item[0] == path:
            return item[1](environ)
    return [b'<h1>404 not found</h1>']
httpd = make_server('',8080,application) # 监听到HTTP请求就会执行application函数
print('Serving HTTP on port 8080...')
# 持续监听HTTP请求
httpd.serve_forever()
```
对应的showtime.html
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>

<h1>time:{{time}}</h1>

</body>
</html>
```


## 参考
[http概念理解](http://www.cnblogs.com/yuanchenqi/articles/6000358.html)

[web框架概念](http://www.cnblogs.com/yuanchenqi/articles/6083427.html)
