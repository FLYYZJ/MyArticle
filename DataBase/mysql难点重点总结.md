## 原理相关
### 绘制ER图
核心三要素：实体 属性和关系。ER图中，矩形代表实体，椭圆形代表属性，菱形代表关系，形状间用线段连接。

一个简单的示例图：
![](https://github.com/undersunshine/MyArticle/blob/master/Algorithm/images/20170101154040319.png)


1. [数据库ER图绘制技巧-1](https://jingyan.baidu.com/album/624e745961c6a134e9ba5a40.html?picindex=4)
2. [数据库ER图绘制技巧-2](https://blog.csdn.net/belen_xue/article/details/52763629)
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
from tbname1
join tbname2 on relations between tb2 and tb1 join tbname3 on relations between tb3 and tb1 ...
where ...
group by ... having ...
order by ...
limit start,count
```
**sql语句的执行顺序**：
(1)from (2) on (3) join (4) where (5)group by(开始使用select中的别名，后面的语句中都可以使用)(6) avg,sum.... (7)having (8) select (9) distinct (10) order by(11) limit 

[sql语句执行顺序分析](https://blog.csdn.net/J080624/article/details/80703903)

**写sql语句的分析过程：**
确定查询内容（如查询的是姓名，成绩，平均分等信息）确定是否使用聚合函数，以及这些数据分布在哪几张表中；如果数据分布于多张表中，则需确定要根据多张表的哪些字段进行表连接（join），然后确定筛选条件（即where），最后根据前述的聚合内容确定要如何分组（用哪个字段进行group by）。

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

**自关联**：

将多张关联表合并成一张表，然后添加多个pid（parent id）等字段用于确定这个数据和合并表中的哪个字段存在相关关系。

例如有省份表（provinces）和市区表（cities），此时市区和省份是相关联的，一个省对应多个市，一个市对应一个省，此时如果分为两张表，此时做关联时需要在市区表中添加一个proid字段用于确定该市是属于哪个省份，此时proid和provinces表中的id字段关联。

而将两表合并成单表后，此时可以给每个数据行都添加一个pid字段，对应省份的数据行，这个pid可以设置为null，而对应市区的数据行，这个pid可以设置为对应省份的那个数据行的id（或其他唯一标识）从而确定了该数据行对应的省份，此时就可以实现自关联。

当然自关联的前提是合并成一张表的多张表信息具有统一的结构，例如省份表保存省份的名字，面积，市区表也保存了市区的名字和面积这两个信息，则此时可以合并，即两张表中的信息必须结构相似。

```sql
create table areas(
  aid int primary key,
  atitle varchar(20),
  pid int,
  foreign key(pid) references areas(id) => 自关联，将关联的pid指定为本表中的id

);

查询时，利用连接方式(自连接)来查询
select province.atitle as ptitle,city.atitle as ctitle from areas as city inner join areas as province on city.pid = province.id;

```

### 视图
对查询的一个封装，以维护查询(当对某一种查询多次使用时可以对视图进行封装，这样在再次调用该查询语句时可以直接调用视图，减少冗余操作)，视图定义为
```sql
create view v_stuscore as 
select students.*, scores.score from scores
inner join students on scores.stuid=students.id
```
此时i构建的视图会存放到数据库中（我们可以认为其构建了一张虚拟表），然后下次做相同的查询时，只需要从该视图中进行查询就可以了，例如查询上述sql语句(select students.\*, scores.score from scores inner join students on scores.stuid=students.id)获得的结果则可以用：
```sql
select * from v_stuscore; 
```

### 事务
当一个业务逻辑需要多个sql语句协同才能完成，如果其中某条sql语句出错，则希望整个操作都会回退。使用事务可以完成回退的功能，保证业务逻辑的正确性。
事务运用在数据表中数据被更改的时候，包括 更新，插入，删除等
事务的四大特性：ACID
```text
A：原子性（Atomicity） 事务中的全部操作在数据库中是不可分割的，要么全部完成，要么都不执行
C：一致性（Consistency）几个并行执行的事务，其执行结果必须和按某个顺序串行执行的结果一致
I：隔离性（Isolation）事务的执行不受其它事务影响，事务执行的中间结果对其它事务必须是透明的
D：持久性（Durability）对于已提交的事务，系统必须保证该事务对数据库的改变不被丢失，即使数据库出现故障
```
对于mysql表类型必须是innodb和bdb类型才可以使用事务。 => alter table tbname engine=innodb（对于原先不是innodb的数据表进行转换）

实现事务语句：
```sql
开启 begin
提交 commit
回滚 rollback

begin; => 构建一张临时表，存放中间结果
更新语句1
删除语句1
...
rollback => 回滚，放弃临时表 

commit; => 将内存中构建的临时表导入正式表中
```

### 索引
提高数据访问性能，主键和唯一索引都可以提高查询速度。建立索引提高查询速度，但是会花费一些内存方面的开销。

尽量避免使用null，使用较为简单，或占内存越小的数据类型可以较好地提高查询速度。

索引类型：

> 单列索引：一个索引只包含单个列
> 组合索引：一个索引包含多个列

[mysql中的索引分析](https://www.cnblogs.com/whgk/p/6179612.html)  
[mysql索引原理简介](https://blog.csdn.net/tongdanping/article/details/79878302)

显示/创建/删除索引
```sql
show index from tbname; => 显示表的索引
create index indexname on tbname(colname(length)); => 指定单列索引和索引长度（一般设置为指定创建该字段时的长度）
create index indexname on tbname(colname(length)，colname(length),....); => 指定多列索引和索引长度
drop index [indexname] on tbname;
```

## 参考
[面试中常见的mysql知识总结](https://blog.csdn.net/DERRANTCM/article/details/51534498)

[sql语句的执行顺序](https://blog.csdn.net/u014044812/article/details/51004754)

[mysql优化经验1](https://www.cnblogs.com/200911/p/5869123.html)

[mysql索引原理简介](https://blog.csdn.net/tongdanping/article/details/79878302)
