## 基本命令
```sql
连接mysql：mysql -u username -p password -p port(默认3306) -h ipaddr(指定的IP地址)
mysql帮助： mysql -h 

备份数据：mysqldump -u superuser -p dbname > path/filename.sql => 保存到path/filename.sql 中
恢复数据：mysql -u superuser -p dbname < path/filename.sql => 恢复path/filename.sql到数据库中
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


## 增删改查（基本操作）
查询
```sql
select colnames from tbname where conditions; 
```
增加数据
```sql
全列插入： insert into tbname values(...); => (...)其中数据的顺序必须和数据表的结构一致，每个列都要取到
缺省插入： insert into tbname(col1,col2,...) values (v1,v2,...); => v和col一一对应(顺序)
同时插入多条数据： insert into tbname values(...),(...),...; <=> insert into tbname(col1,col2,...) values(v1,v2,...),(v1,v2,...),...; =>在程序中用该方法可以提高插入数据的速度
```

删除数据
```sql
delete from tbname where conditions; => 注意最好加入conditions，不加conditions则会删除整张表中的数据
```

更新数据
```sql
update tbname set col1=v1,... where conditions; => 对应修改列和值
```

## 高级查询
```sql
select distinct colnames from tbname where conditions; => 查询不重复的数据 distinct,不同的colname用逗号分割

where中常用的运算符：
  条件比较符： >=， >， <=， <， =， !=(<>)
  逻辑运算符： and，or，not
  模糊查询： %condition%（（包含condition）），%condition（以condition结尾），condition%（以condition开头）,condition_（以condition开头且后面只包含一个字符，两个字符就是两个下划线）
  范围查询：in (v1,v2,v3,...) 查询字段满足在 (v1,v2,v3,...) 这个非连续范围内
           between v1 and v2 查询字段满足在 (v1,v2) 这个连续范围内
```

## 聚合


## 关联


## 参考
[面试中常见的mysql知识总结](https://blog.csdn.net/DERRANTCM/article/details/51534498)
