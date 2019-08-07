# 基础模块
## Python中关于fileobj的定义
fileobj is the file object to monitor. It may either be an integer file descriptor or an object with a fileno() method.   
Python中的file object是指具有文件描述符，或者是实现了fileno方法的对象。

## selectors模块
I/O多路复用指：通过一种机制，可以监视多个描述符(socket)，一旦某个描述符就绪（一般是读就绪或者写就绪），能够通知程序进行相应的读写操作。原本为多进程或多线程来接收多个连接的消息变为单进程或单线程保存多个socket的状态后轮询处理。  
实现IO多路复用机制（提供high-level api），根据平台选出最佳IO多路复用机制，windows为select，Linux为epoll （共三种模式 select， epoll 和 poll）。  
selectors模块提供的API是基于事件的。selectors对象提供socket要处理的事件，然后让调用者等待事件的触发。注册一个事件会产生一个Selectorkey，该key包含socket信息，事件信息以及可选的应用数据。selector调用起select方法会得到事件反馈，该反馈包含一系列的key对象和一个二进制串用于指示哪些事件发生，因此使用selector的程序必须不断地调用select方法，然后处理相应的事件。


- select（windows系统使用）   
1、每次调用都要将所有的文件描述符（fd）拷贝的内核空间，导致效率下降  
2、遍历所有的文件描述符（fd）查看是否有数据访问  
3、最大链接数限额（1024）

- epoll（Linux系统使用）：涉及3个函数  
1、第一个函数是创建一个epoll句柄，将所有的描述符（fd）拷贝到内核空间，但只拷贝一次。  
2、回调函数，某一个函数或某一个动作成功完成之后会触发的函数为所有的描述符（fd）绑定一个回调函数，一旦有数据访问就触发该回调函数，回调函数将（fd）放到链表中  
3、第三个函数判断链表是否为空  
4、最大连接数没有限额

官方示例：下述示例中可以看出
```python

import selectors
import socket

sel = selectors.DefaultSelector()

def accept(sock, mask):
    conn, addr = sock.accept()  # Should be ready
    print('accepted', conn, 'from', addr)
    conn.setblocking(False)
    sel.register(conn, selectors.EVENT_READ, read) # 将这个conn文件对象注册到sel中，当触发对应的read事件时就调用read函数

def read(conn, mask):
    data = conn.recv(1000)  # Should be ready
    if data:
        print('echoing', repr(data), 'to', conn)
        conn.send(data)  # Hope it won't block
    else:
        print('closing', conn)
        sel.unregister(conn)
        conn.close()

sock = socket.socket()
sock.bind(('localhost', 1234))
sock.listen(100)
sock.setblocking(False)  # 保证不阻塞
sel.register(sock, selectors.EVENT_READ, accept) # 将这个sock文件对象注册到sel中，当触发对应的read事件时就调用accept函数

while True:
    events = sel.select()
    for key, mask in events:  
    # 一开始key对应的是sock对应的obj，随后当有连接连接到sock，则for循环会不断地去遍历socket得到的连接，实现不断轮询每个socket连接的效果。
    # 这是一个单线程的程序，利用sel保存所有的socket连接，同时不断地去轮询socket连接对象，实现对多个socket连接的处理 
        callback = key.data
        callback(key.fileobj, mask)
```

selectors的select方法：

abstractmethod select(timeout=None)
Wait until some registered file objects become ready, or the timeout expires.等待某个注册的文件对象准备好，或者超时报错。

If timeout > 0, this specifies the maximum wait time, in seconds. If timeout <= 0, the call won’t block, and will report the currently ready file objects. If timeout is None, the call will block until a monitored file object becomes ready. 超时时间设置为>0，表示最大等待时间; <= 0 表示无等待，直接返回当前已准备好的文件对象否则为空列表，None则表示等待到有文件对象准备好位置

This returns a list of (key, events) tuples, one for each ready file object. 返回值是一个(key,event)的tuple 列表，保存已经准备好的文件对象

key is the SelectorKey instance corresponding to a ready file object. events is a bitmask of events ready on this file object.

## threading模块（event部分）

通过threading.Event()可以创建一个事件管理标志，该标志（event）默认为False，event对象主要有四种方法可以调用：

- event.wait(timeout=None)：调用该方法的线程会被阻塞，如果设置了timeout参数，超时后，线程会停止阻塞继续执行；如果标志为True将立即返回，否则阻塞线程至等待阻塞状态，等待其他线程调用set()。
- event.set()：将event的标志设置为True，调用wait方法的所有线程将被唤醒；
- event.clear()：将event的标志设置为False，调用wait方法的所有线程将被阻塞；
- event.isSet()：判断event的标志是否为True。

## socket模块
#### 1、SO_REUSEADDR选项：
通常一个服务器进程终止后，操作系统会保留几分钟它的端口，从而防止其他进程在超时之前使用这个端口。如果设置SO_REUSEADDR为true，操作系统就会在服务器socket被关闭或服务器终止后马上释放该服务器的端口。

#### 2、setsockopt(level,optname,value) 和 getsockopt(level,optname,buflen)
value参数的内容是由level和optname决定的。level定义了哪个选项将被使用。通常情况下是SOL_SOCKET。意思是正在使用socket选项。还可以通过设置一个特殊协议号码来设置协议选项。SOL_SOCKET的常用选项如下

![](images/socketserver-3.png)

  


# 源码解析
![](images/socketserver-1.png)
![](images/socketserver-2.png)


  
整个代码的框架大致如下：socket_server中包含了几个Server的定义，以及Handler 还有 Mixin。

一般我们使用定义对应的Handler，然后选择合适的Server并对Server加入合适的Mixin来实现多线程并发Socket服务。
```
+------------+
| BaseServer |
+------------+
        |
        v
+-----------+        +------------------+
| TCPServer |------->| UnixStreamServer |
+-----------+        +------------------+
        |
        v
+-----------+        +--------------------+
| UDPServer |------->| UnixDatagramServer |
+-----------+        +--------------------+
```

### BaseServer
全部Server的基类，抽象了各个TCPServer和UDPServer都会使用的操作方法，正式使用中并不会调用到这个类，而是调用其子类（观察代码可以发现其中缺失一些变量，而这些变量都在子类中声明）

使用SocketServer类的一般写法
```python
import socketserver

ip_port = ("127.0.0.1", 8000)


class MyServerHandler(socketserver.BaseRequestHandler):
    def Handle(self):
        print("conn is :", self.request)  # conn
        print("addr is :", self.client_address)  # addr

        while True:
            try:
                # 收消息
                data = self.request.recv(1024)
                if not data: break
                print("收到客户端的消息是", data.decode("utf-8"))
                # 发消息
                self.request.sendall(data.upper())
            except Exception as e:
                print(e)
                break


if __name__ == "__main__":
    s = socketserver.ThreadingTCPServer(ip_port, MyServerHandler)
    s.serve_forever()
```

取TCPServer代码进行分析：
```Python
class TCPServer(BaseServer):

    """Base class for various socket-based server classes.

    Defaults to synchronous IP stream (i.e., TCP).

    Methods for the caller:

    - __init__(server_address, RequestHandlerClass, bind_and_activate=True)
    - serve_forever(poll_interval=0.5)  # 这个函数继承自BaseServer，调用了_handle_request_noblock函数，无阻塞处理request
    - shutdown()
    - handle_request()  # if you don't use serve_forever()
    - fileno() -> int   # for selector

    Methods that may be overridden:

    - server_bind()
    - server_activate()
    - get_request() -> request, client_address
    - handle_timeout()
    - verify_request(request, client_address)
    - process_request(request, client_address)
    - shutdown_request(request)
    - close_request(request)
    - handle_error()

    Methods for derived classes:

    - finish_request(request, client_address)

    Class variables that may be overridden by derived classes or
    instances:

    - timeout
    - address_family
    - socket_type
    - request_queue_size (only for stream sockets)
    - allow_reuse_address

    Instance variables:

    - server_address
    - RequestHandlerClass
    - socket

    """

    address_family = socket.AF_INET  # TCP

    socket_type = socket.SOCK_STREAM # TCP

    request_queue_size = 5  # listen函数后的数量，表示同时处理5个请求

    allow_reuse_address = False  # 重用标志，默认不立刻关闭当前的连接

    def __init__(self, server_address, RequestHandlerClass, bind_and_activate=True):
        """Constructor.  May be extended, do not override."""
        BaseServer.__init__(self, server_address, RequestHandlerClass)
        self.socket = socket.socket(self.address_family,
                                    self.socket_type) # BaseServer中没有的socket变量
        if bind_and_activate:  # 下面的操作都是常规的socket操作 - 绑定IP地址，然后进行监听
            try:
                self.server_bind()
                self.server_activate()
            except:
                self.server_close()  # 实质就是关闭socket
                raise

    def server_bind(self):
        """Called by constructor to bind the socket.

        May be overridden.

        """
        if self.allow_reuse_address:
            self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  # 当连接断开后可立刻重用端口
        self.socket.bind(self.server_address)
        self.server_address = self.socket.getsockname()

    def server_activate(self):
        """Called by constructor to activate the server.

        May be overridden.

        """
        self.socket.listen(self.request_queue_size)  # 其实就是开启listen监听

    def server_close(self):
        """Called to clean-up the server.

        May be overridden.

        """
        self.socket.close()

    def fileno(self):
        """Return socket file number.

        Interface required by selector.

        """
        return self.socket.fileno()

    def get_request(self):  # 在BaseServer中有调用该函数
        """Get the request and client address from the socket.

        May be overridden.

        """
        return self.socket.accept()

    def shutdown_request(self, request):
        """Called to shutdown and close an individual request."""
        try:
            #explicitly shutdown.  socket.close() merely releases
            #the socket and waits for GC to perform the actual close.
            # 显式关闭，其实就是立刻关闭，使用socket.close的实质是释放了socket对象，但实质关闭时间是GC回收的时间
            request.shutdown(socket.SHUT_WR)
        except OSError:
            pass #some platforms may raise ENOTCONN here
        self.close_request(request)

    def close_request(self, request):
        """Called to clean up an individual request."""
        request.close()  # 中断和客户端的连接
```
其中Handle_Request()方法用于处理单次请求，这种情况是用户不想使用server_forever()方法的情况。而Server_forever()方法调用的是_handle_request_noblock方法，调用了self.get_request方法，该方法体现在TCP_Server中就是调用了socket的accept方法获得连接对象和连接地址。  
随后调用process_request方法，该方法的调用了RequestHandlerClass对获得的请求进行处理。


```python
'''
BaseServer中的_handle_request_noblock函数
'''
    def _handle_request_noblock(self):
        """Handle one request, without blocking. 不阻塞地处理一个request

        I assume that selector.select() has returned that the socket is
        readable before this function was called, so there should be no risk of
        blocking in get_request().
        """
        try:
            request, client_address = self.get_request()  # TCP服务器中得到socket的client和对应的地址信息
        except OSError:
            return
        if self.verify_request(request, client_address):
            try:
                self.process_request(request, client_address)
            except Exception:
                self.handle_error(request, client_address)
                self.shutdown_request(request)  # 在TCP server中是关闭TCP连接
            except:
                self.shutdown_request(request)
                raise
        else:
            self.shutdown_request(request)
```
此处调用了process_request方法，其实质是调用如下函数
```python
    def finish_request(self, request, client_address):
        """Finish one request by instantiating RequestHandlerClass."""
        self.RequestHandlerClass(request, client_address, self)  # 调用了Handle类进行处理
```
所以具体怎么用，就是取到数据后怎么使用，就是有用户自定义的RequestHandler决定。回顾上面的一般用法中的MyServerHandler类，其中定义的Handle方法，其中有data = self.request.recv(1024)，明显可以看出这个request就是client,addr = sock.accpet()中的client，和普通socket的用法是一致的。

比较重要的一点是当多个server同时开启时，怎么一一处理这些server的请求，这个就是利用了selector模块，socketserver模块定义了全局的_ServerSelector，所有的server都会注册到这个队列中，每隔poll_interval会去扫描这些server，如果有server被客户端连接，就会被取出来，并逐个进行客户端请求处理。

当且仅当__shutdown_request被置为true时，这个监听线程就会跳出循环。而客户端和server的连接是作为长连接还是短连接处理，则取决于用户自己设计的handlerclass。  
```python
    def serve_forever(self, poll_interval=0.5):
        """Handle one request at a time until shutdown.

        Polls for shutdown every poll_interval seconds. Ignores
        self.timeout. If you need to do periodic tasks, do them in
        another thread. 
        """
        self.__is_shut_down.clear()  # 首先获得将所有wait线程阻塞
        try:
            # XXX: Consider using another file descriptor or connecting to the
            # socket to wake this up instead of polling. Polling reduces our
            # responsiveness to a shutdown request and wastes cpu at all other
            # times.
            with _ServerSelector() as selector:  # 这里的_ServerSelector是全局的，当开启多个socketserver的时候，都会被注册到这个selector对列中
                selector.register(self, selectors.EVENT_READ)  # 将当前对象（因为实现了fileio接口，因此是fileobj）注册进selector队列中

                while not self.__shutdown_request:
                    ready = selector.select(poll_interval)  # 在队列中取出被连接（触发read事件）的对象，组成list，代表所有被连接的server
                    # bpo-35017: shutdown() called during select(), exit immediately.
                    if self.__shutdown_request:
                        break
                    if ready:  # 如果已经有socket准备好，即已经有服务端获得连接
                        self._handle_request_noblock()

                    self.service_actions()
        finally:  # 省略了 except: pass
            self.__shutdown_request = False
            self.__is_shut_down.set()  # 唤醒其它的wait进程
```

多线程socketserver服务器，通过Mixin的方式将多线程处理功能移植入TCPServer中
```python
class ThreadingUDPServer(ThreadingMixIn, UDPServer): pass
class ThreadingTCPServer(ThreadingMixIn, TCPServer): pass
```
Python的Mixin可以视为其它语言中接口的概念
```python
class ThreadingMixIn:
    """Mix-in class to handle each request in a new thread."""

    # Decides how threads will act upon termination of the
    # main process
    daemon_threads = False
    # If true, server_close() waits until all non-daemonic threads terminate.
    block_on_close = True  # 服务关闭后会等待所有非守护进程结束再结束
    # For non-daemonic threads, list of threading.Threading objects
    # used by server_close() to wait for all threads completion.
    _threads = None  # 线程列表，存放处理请求的线程

    def process_request_thread(self, request, client_address):
        """Same as in BaseServer but as a thread.

        In addition, exception handling is done here.

        """
        try:
            self.finish_request(request, client_address)
        except Exception:
            self.handle_error(request, client_address)
        finally:
            self.shutdown_request(request)

    def process_request(self, request, client_address):
        """Start a new thread to process the request."""
        t = threading.Thread(target = self.process_request_thread,
                             args = (request, client_address))  # 启动线程
        t.daemon = self.daemon_threads  # 是不是配置为守护进程
        if not t.daemon and self.block_on_close:
            if self._threads is None:
                self._threads = []
            self._threads.append(t)  #  将新建的线程放入list中
        t.start()

    def server_close(self):
        super().server_close()  # 调用父类的关闭线程
        if self.block_on_close:  # block_on_close=True需要线程进行等待，等待其它非守护线程到达后才退出
            threads = self._threads
            self._threads = None
            if threads:
                for thread in threads:
                    thread.join()
```