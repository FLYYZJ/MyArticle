### flask唯一URL原则
唯一的的原则是一个视图页面只需要对应唯一一个URL，不需要令两个URL指向同一个视图页面（涉及SEO优化）。

一般flask的路由会设置为带斜杆的URL，针对带斜杆的URL，不会进行重定向，而针对不带斜杆的URL，会进行重定向到带斜杆的URL。

重定向原理示意图
![重定向原理](https://github.com/undersunshine/MyArticle/blob/master/Algorithm/images/%E9%87%8D%E5%AE%9A%E5%90%91%E5%8E%9F%E7%90%86.png)


### flask其它路由注册方法
当使用基于类的视图（即插视图），此时需要使用add_url_rule这种形式的添加路由的方式
```python
def hello():
  return 'Hello world'
app.add_url_rule('/hello',view_func=hello) # 指定了路由和对应执行的函数
```
其实@app.route()装饰器内部调用了app.add_url_rule方法实现添加路由，因此所有的路由都是基于app.add_url_rule函数实现的。

### flask app.run的相关参数和flask的配置文件
1. 启用debug模式，即在app.run()方法中传入debug=True这个参数即可（线上部署关闭调试模式）。
2. 指定主机，即在app.run()方法中传入host=ipaddr这个参数即可，而要接受外网的访问，则用host='0.0.0.0'
3. 指定端口，即传入 port=portnum 即可
4. 配置文件可以放在工程中的任意一个地方，在配置文件中建立配置参数，例如debug参数（是否开启）
   app.config.from_object方法可以加载自定义的配置模块
   ```python
   app.config.from_object('config') # config定义在与当前源文件相同的目录下，不是文件的具体路径或相对路径，而是模块路径
   app.run(debug=app.config['DEBUG']) # 获取在配置文件中配置好的参数信息
   ```
注意点 DEBUG 在 flask是一个默认参数，默认为False，因此在读取了config模块后，如果config中没有定义DEBUG参数，此时也会得到一个DEBUG=False的结果。而flask中，对于字母参数，必须统一都是大写形式（统一形式可以减少冗余和错误）。

### 响应对象 response
视图函数会解析返回的内容，返回状态码，content-type http headers（一般默认为content-type=text/html，解析返回内容是按照html的格式进行解析的）

视图函数会把返回的字符串封装为response对象

```python
headers={ # 指定头信息
  'content-type':'text/html' # 如果指定为text/plain的话浏览器就不会解析html标签语法
}
response = make_response('<h1>Hello world</h1>',200) # 指定返回内容和状态码
response.headers=headers # 返回头信息
return response # 返回响应对象
```



