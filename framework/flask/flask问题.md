# flask使用中的一些问题
## 问题1 使用flask-sqlalchemy时，在create_app()函数中用db进行create_all()操作，但实际只是建了数据库，没有建表。
db的定义如下
```python
from flask_sqlalchemy import SQLAlchemy as _SQLAlchemy, BaseQuery
from contextlib import contextmanager

class Query(BaseQuery):
    def filter_by(self, **kwargs):
        return super(Query, self).filter_by(**kwargs)


class SQLAlchemy(_SQLAlchemy):
    @contextmanager
    def auto_commit(self):
        try:
            yield  # 起到中断的作用
            # （事务）在commit之前的操作都没有提交到数据库中，直到commit后才真正提交到数据库中
            self.session.commit()
        except Exception as e:
            self.session.rollback()  # 必须执行回滚操作，避免出现数据误操作
            raise e

db = SQLAlchemy(BaseQuery)
```
假如有一个数据表book是继承于db的，但是和db不处于同一个文件中，则此时在开始flask服务时根本就不会建book表，因为db在create_all()的时候是会检查其上下文，如果book在任何引用db的地方的上下文都不存在，则不会建表，可以在任意位置（需要的位置）处做如下声明，则会建book表
```python
from ... import db
from ... import book
```