## 什么是Json-RPC
json-rpc是基于json的跨语言远程调用协议。比xml-rpc、webservice等基于文本的协议数据传输格小；相对hessian、java-rpc等二进制协议便于调试、实现、扩展，是很优秀的一种远程调用协议。

## 协议简述
一次jsonRPC方法调用
```json
{ "method": "sayHello", "params": ["Hello JSON-RPC"], "id": 1}
```
- method： 调用的方法名
- params： 方法传入的參数。若无參数则传入 []
- id ： 调用标识符。用于标示一次远程调用过程

方法调用结果返回
```json
 {   
    "result":          "Hello JSON-RPC",         
    "error":                null,       
      "id":                      1
 }       
```
- result: 方法返回值。若无返回值。则返回null。若调用错误，返回null。
- error ：调用时错误，无错误返回null。
- id : 调用标识符，与调用方传入的标识符一致。


也可以直接使用GET请求调用远程方法：
http://127.0.0.1:8080/index.json?method=getSystemProperties&id=3325235235235&params=JTViJTVk

參数说明:

- method : 方法名
- params ：调用參数。json的数组格式[], 将參数需**先进行url编码，再进行base64编码**
- id : 调用标识符，随意值。

## 参考
[jsonRPC官方文档](https://www.jsonrpc.org/specification#overview)

[jsonRPC官方文档-zh](http://wiki.geekdream.com/Specification/json-rpc_2.0.html)


[json rpc 简介](https://www.cnblogs.com/clnchanpin/p/7058848.html)

