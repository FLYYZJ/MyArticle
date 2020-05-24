# WSGI
1、服务器程序  
接收处理客户端发来的请求

2、应用程序  
处理服务器传递的请求

WSGI：wsgi是python web开发的标准，类似于协议。它是服务器程序和应用程序的一个约定，规定了各自使用的接口和功能，以便二和互相配合。

# 服务器-中间件-应用程序的交互
### 一、WSGI应用程序规定
1. 应用程序是一个可调用的对象  
可调用的对象有三种：  
   - 一个函数  
   - 一个类，必须实现__call__()方法 
   - 一个类的实例

2. 对象接收两个参数，environ和start_response，如
   ```python
    class application:
        def __call__(self, environ, start_response):
            pass
   ```
3. 可调用对象需要返回一个可迭代的值。以可调用对象为一个类为例
   ```python
        class application:
            def __call__(self, environ, start_response):
                return [xxx] # 可迭代值
   ```

### 二、WSGI服务器程序规定
```python
def run(application):     #服务器程序调用应用程序
    environ = {}     #设定参数

    def start_response(xxx):     #设定参数
        pass
    result = application(environ, start_response)          #调用应用程序的__call__函数（这里应用程序是一个类）
    def write(data):
               pass
    def data in result:          #迭代访问
        write(data)
```
 服务器程序主要做了以下的事：
1. 设定应用程序所需要的参数
2. 调用应用程序
3. 迭代访问应用程序的返回结果，并传给客户端 


### 三、中间件作用
对于服务器程序来说，middleware就是应用程序，middleware需要伪装成应用程序，传递给服务器程序  
对于应用程序来说，middleware就是服务器程序，middleware需要伪装成服务器程序，接受并调用应用程序  
服务器获取客户端请求的url，middleware将url交给对应的函数处理。下述midware_app就是middleware
```python
def urlrouting(url_app_mapping):   
     def midware_app(environ, start_response):       #函数可调用，包含2个参数，返回可迭代的值
          url = environ['PATH_INFO']       
          app = url_app_mapping[url]       #获得对应url的应用程序
          result = app(environ, start_response)       #调用应用程序
          return result       
     return midware_app
```
一方面，midware_app函数设置了应用程序所需要的变量，并调用了应用程序。所以对于应用程序来说，它是一个服务器程序
另一方面，midware_app函数是一个可调用的对象，接收两个参数，同时可调用对象返回了一个可迭代的值。所以对于服务器程序来说，它是一个应用程序

```markdown
中间件逻辑
1. middleware需要伪装成应用程序—> WSGI应用程序的要求 —> 1. 可调用  2. 两个参数  3. 返回可迭代的值
2. middleware需要伪装成服务器程序 —> WSGI服务器程序的要求 —> 调用应用程序
```

# 参数定义
## 一、environ变量
environ通常为字典结构，带如下键值：
```python
AUTH_TYPE
  CONTENT_LENGTH        #HTTP请求中Content-Length的部分
  CONTENT_TYPE          #HTTP请求中Content-Tpye的部分
  GATEWAY_INTERFACE
  HTTP_*                    #包含一系列变量, 如HTTP_HOST,HTTP_ACCEPT等
  PATH_INFO             #URL路径除了起始部分后的剩余部分,用于找到相应的应用程序对象,如果请求的路径就是根路径,这个值为空字符串
  PATH_TRANSLATED
  QUERY_STRING          #URL路径中？后面的部分
  REMOTE_ADDR
  REMOTE_HOST
  REMOTE_IDENT
  REMOTE_USER
  REQUEST_METHOD        #HTTP 请求方法，例如 "GET", "POST"
  SCRIPT_NAME           #URL路径的起始部分对应的应用程序对象,如果应用程序对象对应服务器的根,那么这个值可以为空字符串
  SERVER_NAME
  SERVER_PORT
  SERVER_PROTOCOL       #客户端请求的协议(HTTP/1.1 HTTP/1.0)
  SERVER_SOFTWARE
```
http://localhost:5000/aaa?666 具有如下含义
```python
REQUEST_METHOD=‘GET’
SCRIPT_NAME=''
SERVER_NAME='localhost'
SERVER_PORT='5000'
PATH_INFO='/aaa'
QUERY_STRING='666'
SERVER_PROTOCOL='HTTP/1.1'
CONTENT_TYPE='text/plain'
CONTEN_LENGTH='' 

HTTP_HOST = 'localhost:5000'
HTTP_ACCEPT = 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8'
HTTP_ACCEPT_ENCODING = 'gzip,deflate,sdch'
HTTP_ACCEPT_LANGUAGE = 'en-US,en;q=0.8,zh;q=0.6,zh-CN;q=0.4,zh-TW;q=0.2'
HTTP_CONNECTION = 'keep-alive'
HTTP_USER_AGENT = 'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/32.0.1700.77 Safari/537.36'
```
## 二、start_response()函数
```python
start_response(status, response_headers, exc_info=None)
```
status 为 http 状态码，response_headers为带tuple(header_name, header_value)的list，exc_info为可选参数，处理请求出现错误时需要设置这个参数。

## 三、相关变量
```javascript
wsgi.version                                                  
值的形式为 (1, 0) 表示 WSGI 版本 1.0                             
wsgi.url_scheme                                               
表示 url 的模式，例如 "https" 还是 "http"                        
wsgi.input                                                    
输入流，HTTP请求的 body 部分可以从这里读取                         
wsgi.erros                                                    
输出流，如果出现错误，可以写往这里
wsgi.multithread
如果应用程序对象可以被同一进程中的另一线程同时调用，这个值为True
wsgi.multiprocess
如果应用程序对象可以同时被另一个进程调用，这个值为True
wsgi.run_once
如果服务器希望应用程序对象在包含它的进程中只被调用一次，那么这个值为True
```


# 参考

- [URL, URI, IRI, URN解析](https://zhangzifan.com/t/7393.html)