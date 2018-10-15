### flask唯一URL原则
唯一的的原则是一个视图页面只需要对应唯一一个URL，不需要令两个URL指向同一个视图页面（涉及SEO优化）。

一般flask的路由会设置为带斜杆的URL，针对带斜杆的URL，不会进行重定向，而针对不带斜杆的URL，会进行重定向到带斜杆的URL。

重定向原理示意图
![重定向原理](https://github.com/undersunshine/MyArticle/blob/master/Algorithm/images/%E9%87%8D%E5%AE%9A%E5%90%91%E5%8E%9F%E7%90%86.png)

### flask自动重启服务器
启用debug模式，即在app.run()方法中传入debug=True这个参数即可。

### flask其它路由注册方法
当使用基于类的视图（即插视图），此时需要使用add_url_rule这种形式的添加路由的方式
```python
def hello():
  return 'Hello world'
app.add_url_rule('/hello',view_func=hello) # 指定了路由和对应执行的函数
```
其实@app.route()装饰器内部调用了app.add_url_rule方法实现添加路由，因此所有的路由都是基于app.add_url_rule函数实现的。


