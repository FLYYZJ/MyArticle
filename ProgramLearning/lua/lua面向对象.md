# lua面向对象
## 元表
#### 元表的作用
访问一个table中不存在的key，lua将会返回一个nil。
但一个表存在元表的时可能会发生改变。即访问不存在的key时，如果这个table存在元表，就会尝试在它的元表中寻找是否存在匹配key对应的value。  
同时，元表也可用于实现类似于C++操作符重载的操作。主要是重载每个表中的元方法，如__add, __index, __call 等。   
![](/assets/lua-metatable.png) 
元表也是实现Lua面向对象编程中的继承概念的手段。  

#### 实现元表
setmetatable和getmetatable方法  
```lua
local mytable = {}
local mymetatable = {}
setmetatable(mytable, mymetatable)  
-- 等价于 local mytable = setmetatable({}, {})
```
#### 重载元方法
```lua
mytable = setmetatable({key1 = "value1"},   --原始表
  {__index = function(self, key)            --重载函数
    if key == "key2" then
      return "metatablevalue"
    end
  end
})

print(mytable.key1,mytable.key2)  --> output：value1 metatablevalue
```


## lua面向对象的概念
lua通过表（Table，关联数组）和函数实现面向对象编程（有点像GO，GO是基于struct和function实现的）   
__index 关键字用于表明类型

## lua中的self




## 参考
lua中的self.__index == self https://www.cnblogs.com/mentalidade/p/6561418.html  
