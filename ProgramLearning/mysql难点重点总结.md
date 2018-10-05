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

查询条件判断中的运算符：
```sql
select distinct colnames from tbname where conditions; => 查询不重复的数据 distinct,不同的colname用逗号分割

where中常用的运算符：
  条件比较符： >=， >， <=， <， =， !=(<>)
  逻辑运算符： and，or，not
  
  模糊查询： %condition%（（包含condition）），%condition（以condition结尾），condition%（以condition开头）,condition_（以condition开头且后面只包含一个字符，两个字符就是两个下划线）
  范围查询：in (v1,v2,v3,...) 查询字段满足在 (v1,v2,v3,...) 这个非连续范围内
           between v1 and v2 查询字段满足在 (v1,v2) 这个连续范围内
  
  空判断：is null 不能用 = null; is not null 不能用 != null;
```
**优先级 小括号 ＞ not ＞ 比较运算符 ＞ 逻辑运算符**

## 较为高级的应用
### 聚合(聚合函数)
统计现有的多行数据，并输出统计结果。常用的5个聚合函数如下
```sql
count(*) => 统计符合条件的列有多少行
max(colname) => 找出符合条件的数据中的最大值
min(colname) => 找出符合条件的数据中的最小值
sum(colname) => 求符合条件的数据的和（数字类型的求和）
avg(colname) => 求符合条件的数据的平均值（数字类型的平均值）
```
示例：执行顺序是先执行where条件判断找到符合条件的数据，再作聚合
```
select count(*) from students where id>3; 统计students表中 id>3 总共有多少个学生，执行顺序是先找出符合条件的数据，然后再做统计
select * from students where age=(select min(age) from students); 选择年龄最小的那个学生的信息
```
### 分组
假设有如下students表格：

|id| name        | age           | gender  |
|--|:-----------:|:-------------:| -------:|
|1| jack         | 14            |M    |
|2| jane         | 13            |F    |
|3| tim          | 12            |M    |
|4| zoey         | 15            |F    |
|4| joyce         | 15           |F    |

```sql
select col1,col2,聚合 ... from tbname group by col1,col2,col3,....
group by 将列中相同的值聚成一组

查询男女的总数：
select gender as sex,count(*) from students group by gender; => 按照执行顺序，先作聚合，即按照gender分成M 和 F两组，然后再做聚合和筛选数据
```
分组后的数据筛选： having，和where的区别在于having是在数据分组后再进行筛选，而where是在分组前进行数据的筛选
```sql
select col1,col2,聚合 ... from tbname group by col1,col2,col3,.... having col1,col2,...,聚合...
select gender as sex,count(*) from students group by gender having gender='F'; 聚合结果后选出gender为F的那些数据的统计结果
select gender as sex,count(*) from students group by gender having count(*)> 2; 聚合结果后选出分组的行数大于2的那些数据的结果
```

### 排序
order by，按照列或多列进行升序ASC 或 降序DESC 排序，默认为ASC；位于where句之后
```sql
select * from tbname where conditions order by col1 desc|asc, col2 desc|asc,... 
```

### 分页： 有利于降低服务器的压力
limit关键字: start是指从指定的start id开始（start id默认从0开始，这个id可以是0,1,2,3...这些数值，不间断，和数据表中的id列无关），count表示取其中的count行数据出来
```sql
select * from tbname where conditions limit start,count
select * from students where gender = 'F' limit (n-1)*m,m; n为页码，此时从1开始计算，m为每页要显示的数据条数
```

### 连接
join（inner join），left join，right join
示例：学生和成绩有直接关系，科目和成绩有直接关系，而学生和科目无直接关系，因此from取scroes表
```sql
select students.name,subjects.title,scores.score from scores join subjects on subjects.id = scores.subid inner join students on  students.id = scores.stuid;
```
**实际的三种连接方式的参考**：

[inner join](http://www.w3school.com.cn/sql/sql_join_inner.asp)

[left join](http://www.w3school.com.cn/sql/sql_join_left.asp)

[right join](http://www.w3school.com.cn/sql/sql_join_right.asp)

完整的查询语句
```sql
select distinct *
from tbname
where ...
group by ... having ...
order by ...
limit start,count
```
**sql语句的执行顺序**：
(1)from (2) on (3) join (4) where (5)group by(开始使用select中的别名，后面的语句中都可以使用)(6) avg,sum.... (7)having (8) select (9) distinct (10) order by(11) limit 




## 3个重要的知识点
关系，事务和索引
### 关联，关系(relations)
一对一，一对多，多对多 三种关系。关系也是一种数据，需要被保存。

**外键约束**：建立关系字段后为**保证关系的正确性**而设定的约束。构建了外键约束，对于关联表中不存在的关联数据（如id），如果此时数据表的对应字段插入了这些不存在的关联数据会报错（即该约束规范了表的关联关系，使得在插入数据的过程中不会引入错误的关联数据）。
```sql
create table scores(
  id int primary key auto_increment,
  stuid int,
  subid int,
  score decimal(5,2),
  foreign key(stuid) references students(id), => 对当前表的stuid构建外键，对应的是students表的id
  foreign key(subid) references subject(id)
);
```

外键的级联操作： 删除掉某表中的数据时，如果有其它表格和该表格存在外键约束的时候，其它表格中的数据也要做相应操作，mysql设定了4种操作：
```sql
restrict： 抛出异常
cascade： 关联数据也会被删除
set null： 将外键设置为null
no action： 不做任何操作
```
为了不误删除数据，推荐解决方案是使用逻辑删除而非物理删除的方案，即设置关联表中的数据为逻辑删除（根据要删除数据的外键），然后将外键约束设定为set null 或 no action，然后物理删除要删除的数据即可。

## 参考
[面试中常见的mysql知识总结](https://blog.csdn.net/DERRANTCM/article/details/51534498)

[sql语句的执行顺序](https://blog.csdn.net/u014044812/article/details/51004754)
