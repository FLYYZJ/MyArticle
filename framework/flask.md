### 设计上的一些原则
1. 保证视图函数中的代码是简洁易懂的，尽量将所需的功能封装成外部模块中的一个函数，以供调用
2. 尽量将容易被证伪（if...else...）的操作放在前面，这样可以减少内部的操作时间
3. 阅读源码的次序是先了解功能，架构，随后再到需要仔细阅读的函数中阅读细节代码

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
4. 配置文件可以放在工程中的任意一个地方，在配置文件中建立配置参数，例如debug参数（是否开启）。app.config.from_object方法可以加载自定义的配置模块，在app所在的文件中引入下列语句，config文件中只需要写入所有的常量即可，很简单
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
提供数据的api接口（例如json格式的数据）
```python
headers={
  'content-type':'applications/json,
  ....
}
response = make_response(jsondata,statuscode) => 传入json数据和状态码即可
```
其实可以不需要使用response对象也可以返回数据
```python
return content,statuscode,headers and other info => 简易的响应内容返回方式，flask会在后台将其封装成response对象
```

### url参数传递
此时q和page会作为url中传递到视图函数的参数
```python
@app.route('/book/search/<q>/<page>')
def function(q,page):
  ...
```

### flask 路由深入理解
flask在url和视图函数中间加入endpoint概念，为实现反向路由提供方便。
flask的url要顺利访问到视图函数，需要urlmap对象和viewfunctions对象中分别存有url和对应的视图函数，这样才能实现索引。
![](https://github.com/undersunshine/MyArticle/blob/master/Algorithm/images/%E5%BE%AE%E4%BF%A1%E5%9B%BE%E7%89%87_20181015171931.png)

url参数传递，对于不是使用?param1=val1&param2=val2这种形式，可以直接放入装饰器的参数列表中，如下
```python
@web.route('/book/search/<q>/<page>') # 视图函数插入蓝图中
def search(q,page):
    '''
        q：普通关键字 isbn
        page：
        :return:
    '''
    isbn_or_key = is_isbn_or_key(q)
    if isbn_or_key == 'isbn':
        res = YuShuBook.search_by_isbn(q)
    else:
        res = YuShuBook.search_by_keyword(q)
    # res不是字符串而是字典，因此要访问需转换为字符串（序列化）
    # return json.dumps(res),200,{'content-type':"application/json"}
    return jsonify(res) # 等价于上面的一串语句
```
反之，需要使用Request对象(包含全部请求信息，是flask后台构建的)来接收?后面所带的参数
```python
@web.route('/book/search') # 视图函数插入蓝图中
def search_args():
    '''
        q：普通关键字 isbn
        page：
        :return:
    '''
    q = request.args['q'] # 获取参数,q至少一个字符，长度限制
    page = request.args['page'] # 正整数，且最大值限制
    isbn_or_key = is_isbn_or_key(q)
    if isbn_or_key == 'isbn':
        res = YuShuBook.search_by_isbn(q)
    else:
        res = YuShuBook.search_by_keyword(q)
    # res不是字符串而是字典，因此要访问需转换为字符串（序列化）
    # return json.dumps(res),200,{'content-type':"application/json"}
    return jsonify(res) # 等价于上面的一串语句
```


### 蓝图 blueprint
蓝图是夹在app和视图函数之间的一层，可以解决flask中视图函数分文件的问题
![](https://github.com/undersunshine/MyArticle/blob/master/Algorithm/images/20181015175413.png)

### wtform进行参数校验
```python
class SearchForm(Form):
    q = StringField(DataRequired(),validators=[Length(min=1,max=30)]) # 指定q为字符串类型，且长度有限制，必须有数据（不能为空格）
    page = IntegerField(validators=[NumberRange(min=1,max=99)],default=1)
------------------------------------------
form = SearchForm(request.args) # 表单对象
if form.validate(): # 表单得到验证
  q = form.q.data.strip()
  page = form.page.data # 可以得到默认值
```


### 数据库相关 模型类
三种创建数据库的方式- model first，Database First，Code First
1. Database First：直接在数据库中建表同时传入数据
2. model first：利用navicat提供的ER图工具创建相应的关系图，然后自动生成对应的数据库
3. code first：模型层，和ORM不同。code first只关注数据怎么创建。

业务逻辑最好写在模型层，因为数据库的构建和各表间的关系和业务相关度高，因此最好构建在模型层。

基于sqlalchemy进行模型层的封装操作
```python
from sqlalchemy import Column, Integer, String
from flask_sqlalchemy import SQLAlchemy
db = SQLAlchemy() # 实例化sqlalchemy对象
class Book(db.Model): # 定义模型
    id = Column(Integer,primary_key=True,autoincrement=True)
    title = Column(String(50),nullable=False)
    author = Column(String(30),nullable=False,default='未名')
    binding = Column(String(20))
    publisher = Column(String(50))
    price = Column(String(20))
    isbn = Column(String(15),nullable=False,unique=True)
    pages = Column(Integer)
    pubdate = Column(String(20))
    summary = Column(String(1000))
    image = Column(String(50))

```
同时需要注册和绑定app
```python
db.init_app(app)
db.create_all(app=app)
```
如果再存储数据时有某个字段不存在，但是实际在模型类中有定义，则此时可以将对应的表去除，然后再重新启动程序生成表，这样可以补充那个字段，或者手动补充缺失的字段。

### flask开启多线程
对象是保存状态的地方。

线程（是由webserver进行调度，不由flask进行调度）

flask在app.run方法中设置threaded参数为True实现flask开启多线程，在app.run方法中设置process参数为1,2,3,...实现flask设置进程数。

**线程隔离的意义在于 使当前线程可以正确引用到其创建的对象，而不会是引用到其它线程所创建的对象。**

flask中的线程隔离：利用字典结构和线程id号（线程唯一标识）实现请求对象获取线程信息，即请求对象可以知道当前访问服务器对应的线程是哪个以及其相关信息。
```python
request = {t_key1:Request1,t_key2:Request2,...}
```

flask中的线程隔离对象Local，werkzeug local Local，本质是一个字典
```python
myobj = Local()
myobj.b = 1

def worker():
  myobj,b = 2
  print(myobj.b) # 输出2

newt=threading.Thread(target=worker,name='t1')
newt.start()
time.sleep(1)
print(myobj.b) # 输出1
```

线程隔离栈 LocalStack()：push，pop和top方法，有线程隔离的特性。

### flask常见误区
错误： working outside application context
本地代理-LocalProxy：代理模式——设计模式内容

flask中的上下文（对象，对一系列flask上下文进行封装）： 应用上下文 Flask封装，请求上下文 Request封装

Flask核心对象存储在AppContext（封装Flask核心对象，因为Flask保存一些核心操作，而同时也有也有一些操作需要核心对象之外的操作和数据，因此AppContext进行封装）中,Request对象存储在RequestContext中。


当用户发来一个请求后，会检查_app_ctx_stack（这个栈可以用app.app_context()方法得到，有push和pop两个方法）是否为空，如果为空则推入一个app对象，随后再往_request_ctx_stack推入request对象（基于用户的请求构造而成）。当请求结束后，app对象和request对象会被弹出。因此一切操作都是基于用户发来的请求进行的，包括入栈和出栈操作。
current_app 和 request 对象都是指向这两个栈顶的元素。LocalProxy not bind 出现的情况是因为两个栈同时为空的情况。

实现上下文协议的对象可以使用with语句，一个类如果定义了__enter__ 和 __exit__ 函数则构成一个上下文管理器，此时可以使用with语句。

![](https://github.com/undersunshine/MyArticle/blob/master/Algorithm/images/20181016210402.png)

### 模型层（view层）
从数据库或者API中获取的数据是原始数据，直接传给视图函数处理；而用户获取的数据展示都是体现在页面上，而页面的结构不同，可能所需的数据也不同（不需要所有数据，需要改变数据的结构或者需要多种数据）

在原始数据到页面间加入viewmodel层可以对传入页面的数据做一定的修饰和改变，从而适配页面的需求。

![](https://github.com/undersunshine/MyArticle/blob/master/Algorithm/images/20181017114259.png)

模板：render方法进行渲染，可以有多个varname，随意取名，htmlname是放在templates目录下的（app = Flask(__name__)该句对应的目录为根目录，templates目录也在该根目录下）对应的html文件，可以在注册app或蓝图的地方定义模板文件存放的位置（即指定路径，蓝图的根目录是app目录，而app目录是根目录，传入的路径是相对路径）
```python
@app.route('/test')
def test():
  r = {'t1':'hello', 't2':'world'}
  return render_template(htmlname,varname = r) 
```

模板中解析数据，flask是基于jinja2模板引擎实现的。在模板（HTML文件中）中使用方式和Django是一致的。jinja2常用语法示例：
```html
{{data.age}} == {{data['age']}} data是由视图函数中传入的字典或对象

for循环
{% for foo in [1,2,3] %} 这个列表也可以从视图函数传入
  {{foo}} 或 标签语言 输出1，2，3
{% endfor %}

{% for k,v in data.items() %} 针对字典数据，该字典是由视图函数传入
  {{k}}:{{v}} 
{% endfor %}

if条件控制(可使用and 和 or 等条件符)
{% if data.age == 18 %}
  {{data.name}} 或 标签语言
{% elif data.age < 18 %}
  {{data.name}} 或 标签语言
{% else %}
  {{data.name}} 或 标签语言
{% endif %}
```
#### 模板继承
和django一致，需要在base.html中写入block，即{% block blockname %} {% endblock %}，引入模板是{% extends 'base.html' %}。在继承模板的文件中，使用{% block blockname(在base.html中定义的某个block) %} {% super() %}（如果要加入父模板中的内容就要该方法） 写入自定义内容 {% endblock %}

#### 模板中的过滤器和管道
```html
{{data.name | default('未名')}} 当name属性不存在时会输出未名，竖线可以理解为Linux的管道命令
{{data.name | default('未名',true)}} 当name属性不存在或name属性存在但为空值时会输出未名
{{ data | length() }} ： 获取data的长度
```

#### 反向URL：
url_for:通过endpoint使得可以反向得到视图函数的url
```html
{{url_for('static',filename="test.css")}} 前面的static是相对路径（相对于app 根路径），filename是要传入的静态文件（css，图片，js）
```

#### 消息闪现
在视图函数中使用flash函数，例如flash('2123 231 23'),当然flash可以有多个，同时可以设置这些flash消息的分类。在模板文件中使用 {% set messages = get_flashed_messages(category_filter=category_name) %} 获取闪现数据，当然其中的category_filter参数可以不用传递。

此时需要在配置文件中写入SECRET_KEY，才能使用flash数据
```python
SECRET_KEY = 任意对一无二的字符串
```
```html
{% set messages = get_flashed_messages(category_filter=category_name) %}  得到的message仅在当前block中有效
{% with messages = get_flashed_messages(category_filter=category_name) %} {{messages}} {% endwith %} 此时message仅在with块中有效，with限定了变量的作用域
```

## flask中的method —— GET和POST方法
```python
@web.route('/register', methods=['GET', 'POST']) # 在后面代入methods方法，列表，传入方法有GET 和 POST 两种
request.form # 获得post方式提交的表单数据
```

### MVC的业务逻辑应该写在Model层中，而不是写在Controller层中，模型层和业务相关


