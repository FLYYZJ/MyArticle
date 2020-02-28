# MongoDB 简介
数据模型面向文档，文档类似于JSON结构（BSON)
- 概念
    1. 数据库
    2. 集合（collection），集合中可存放文档，文档集合
    3. 文档，数据库中的最小单元，操作文档
# MongoDB操作

## 一、基本操作
show dbs / show databases - 显示当前的数据库  
use dbname - 使用数据库dbname，如果dbname不存在，则在第一次插入数据时就会自动创建数据库，集合也一样  
db - 显示当前所处的数据库
show collections - 显示数据库中去全部集合

###  插入
```c
db.collection.insert(doc)  // 向集合collection中插入文档doc，doc为json格式，键不需要加双引号

db.collection.insert([doc1, doc2, doc3]) // 插入多个doc

db.collection.insertOne(doc) // 插入一个文档对象

db.collection.insertMany([doc1, doc2, doc3, ...]) // 必须插入多个文档
```
mongodb在没有指定_id时，会自动添加ID，作为文档的唯一标识，而生成的方式是使用ObjectId()进行生成（基于时间戳，不会重复）  

### 删除
```c
db.collection.Remove({删除条件})
db.collection.Remove({删除条件}, true)  // 给定了true条件就只删除一个

db.collection.deleteMany({删除条件})

db.collection.deleteOne({删除条件})

db.drop() // 清空整个数据库
```


###  修改
```c
db.collection.update({查询条件}, {新对象})  //  update会默认使用新对象替换旧对象，就对象将完全被替换

db.collection.update({条件}, $set: {替换值}) // 此时本来不存在的属性也会被添加

db.collection.update({条件}, $unset: {删除值}) // 删除指定的属性，删除值可以赋任何值

db.collection.update({name: xiaoming}, {age: 18})

updateMany（同时修改多个符合条件的记录） 和 updateOne（只修改第一个符合条件的记录，此时和update一致）

db.collection.update({条件}, {
    $set: {
        ...
    }
},
{附加条件，如 {multi: true }即表示基于上述修改多个记录 })
```
内嵌文档（json中的值为object的元素）查询，则属性名必须加引号    
db.users.update({username:"xiaoming"}, {$push:{"hobby.movies":"Interstellar"}})，注意此处的引号，其中push，当值为数组时，可用该关键字向数组中添加元素  
此外还有addToSet之类的方法，此时如果数组中存在对应的元素，就不会再添加

###  查询
```c
db.collection.find()  // 查询集合中的全部文档 

db.collection.find({...})  // 按对象作为条件进行查询，例如db.collection.find({age:15, name:"xiaoming"})，返回数组，可以加索引[]

db.collection.find({...}).count() // 获得查询结果的数量

db.collection.findOne({}) // 获得符合条件的第一个文档，类似对象，可以使用.获取特定的字段
```
db.users.find({'hobby.movies': "hero"}) 内嵌文档（json中的值为object的元素）查询，则属性名必须加引号
，注意此处的引号。  

db.users.find({age: {$lt: 30}}) 查询小于30岁的user，此外还有其他比较符，详见文档

db.users.find({age:{$lt: 30}}).limit(10) 限制查询的大小  
db.users.find({age:{$lt: 30}}).skip(10).limit(10) 分页查询，查询11-20条记录，skip和limit的位置可对调  

## 二、高级操作

## 流程控制
for 循环
```javascript
// 速度较慢
for(var i = 1; i<=20000; i++) {
    // 往numbers文档中添加20000带num的元素
    db.numbers.insert({num: i})
}
// 速度较快 —— 少进行数据库操作，数据库操作比较耗时
var arr = []
for(var i = 1; i<=20000; i++) {
    arr.push({num: i})
}
db.numbers.insert(arr)
```

### 文档关联
文档间关系：
- One to One  
    通过内嵌文档来体现1对1关系  
    ```json
    {  
        name:"wife",  
        husband: {  
            name: "husband"  
        }  
    }  
    ```
- One to Many
    用户为users, 订单为orders
    ```json
    users: {  
        {id: 1, username: "ubk"}, {id: 2, username: "abc"}  
    }  
    orders: {  
        {  
            list:["good1", "good2"],  
            userId: 1 - 对应ubk  
        }  
    }  
    ```
- Many to Many
    ```json
    {
        tearchers: {
            {"name":"Mr.wang", "id": 1},
            {"name":"Mr.liu", "id": 2},
            {"name":"Mr.zhang", "id": 3}
        },
        students: {
            {"name":"xiaoming", "techids": ["1", "2", "3"] }
        }
    }
    ```
- Many to One


## json 导入 MongoDB


## 学习视频/文档
https://www.bilibili.com/video/av49923533?p=10  
https://docs.mongodb.com/ 