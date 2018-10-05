## 基本命令
```sql
连接mysql：mysql -u username -p password -p port(默认3306) -h ipaddr(指定的IP地址)
mysql帮助： mysql -h 
```
## 建表/删表 
基本建库方式
```sql
create database dbname charset=charsetname;(一般为utf-8)
show databases; => 显示所有的数据库
select database(); => 查看当前使用的数据库
use dbname; => 使用某数据库

```
基本建表/修改表方式
```sql
create table tbname(column type);
例子：
列及其类型的： 列名，类型，列的一些属性:
  auto_increment（自动增长，一般是主键） 
  primary key（主键）
  not null（不能为空）
  default 1 设置默认值

create table students(
id int auto_increment primary key not null, 
sname varchar(10) not null,
gender bit default 1 ,
); => 此时底层默认使用InnerDB ENGINE = InnoDB，默认编码是utf8 
show create table tbname; => 显示建表时的语句

desc tbname; => 显示表结构
show tables; => 显示当前使用的数据库中所有的表

alter table tbname add|change|drop colname type; => 增加列，修改列，删除列
alter table students add isDelete bit default 0; => 增加一个isDelete列

drop table tbname; => 删除表格

rename table orignalname to newname; => 修改表名
```

基本删表方式
```sql
drop database dbname
```


## 增删改查
查询
```sql

```


## 聚合


## 关联


## 参考
[面试中常见的mysql知识总结](https://blog.csdn.net/DERRANTCM/article/details/51534498)
