## 基础redis教程推荐：
1.  基础redis教程：http://www.runoob.com/redis/redis-tutorial.html



## redis教程（高级功能）
redis的定义分发架构：发布者和订阅者都是Redis客户端，Channel则为Redis服务器端（队列等数据结构存放的位置），发布者将消息发送到某个的频道，订阅了这个频道的订阅者就能接收到这条消息。Redis的这种发布订阅机制与基于主题的发布订阅类似，Channel相当于主题。
![](https://github.com/undersunshine/MyArticle/blob/master/Algorithm/images/20170415153941123.jpg)

```
slaver-1发送消息
publish channelname value => 可以指定为队列或其它数据结构

slaver-2订阅频道
subscribe channnelname1,channelname2,... => 可同时订阅多个频道
psubscribe channnelname1pattern,channelname2pattern,... => 通过模式匹配同时订阅多个频道

```

## redis源码阅读



