# 接口
interface关键字；接口名称以大写字母 I 开头(约定接口规范)。IEquatable<T> 的定义不为 Equals 提供实现。 该接口仅定义签名。 这样，C# 中的接口便类似于其中所有方法都是抽象方法的抽象类。 但是，类或结构可以实现多个接口，但是类只能继承单个类（抽象或不抽象）。
```C#
interface IEquatable<T>
{
    bool Equals(T obj);
}
```
当类或结构实现接口时，类或结构必须为该接口定义的所有成员提供实现。 接口本身不提供类或结构可以通过继承基类功能的方式来继承的任何功能。 但是，如果基类实现接口，则从基类派生的任何类都会继承该实现。

接口特点：
- 接口类似于只有抽象成员的抽象基类。 实现接口的任何类或结构都必须实现其所有成员。
- 接口无法直接进行实例化。 其成员由实现接口的任何类或结构来实现。
- 接口可以包含事件、索引器、方法和属性。
- 接口不包含方法的实现。
- 一个类或结构可以实现多个接口。 一个类可以继承一个基类，还可实现一个或多个接口。

- 显式接口实现 针对类中存在和接口方法一致签名的方法
如果两个接口成员不执行同一功能，则会导致其中一个接口或两个接口的实现不正确。 创建仅通过接口调用且特定于该接口的类成员，则有可能显式实现接口成员。 这可通过使用接口名称和句点命名类成员来完成。如下使用显式接口调用指定方法的实现。
```C#
public class SampleClass : IControl, ISurface
{
    void IControl.Paint()
    {
        System.Console.WriteLine("IControl.Paint");
    }
    void ISurface.Paint()
    {
        System.Console.WriteLine("ISurface.Paint");
    }
}
// Call the Paint methods from Main.

SampleClass obj = new SampleClass();
//obj.Paint();  // Compiler error.

IControl c = obj;
c.Paint();  // Calls IControl.Paint on SampleClass.

ISurface s = obj;
s.Paint(); // Calls ISurface.Paint on SampleClass.

// Output:
// IControl.Paint
// ISurface.Paint
```


# 迭代器
## C#中的可迭代对象和迭代器
1. 简单来说IEnumerable是一个声明式的接口，声明实现该接口的类就是“可迭代的enumerable”,但并没用说明如何实现迭代器(iterator).其代码实现为：
```c#
public interface IEnumerable
{
    IEnumerator GetEnumerator();
}
```
2. 而IEnumerator接口是实现式接口，它声明实现该接口的类就可以作为一个迭代器iterator.其代码实现为：
```c#
public interface IEnumerator
{
        object Current { get; }

        bool MoveNext();
        void Reset();
}
```
3. 一个collection要支持Foreach进行遍历，就必须实现IEnumerable,并一某种方式返回迭代器对象:IEnumerator.  

示例使用这两个接口实现简单迭代器
```C#
//实现IEnumerable接口

public class People :IEnumerable
{
    public Person [] pers;
    public People(Person [] ps)
    {
        this.pers = ps;
    }
    public IEnumerator GetEnumerator()
    {
        //foreach(Person p in pers) C# 2.0以上实现的更为简便的迭代器实现模式
        // {
            //  yield return p;
        // }
    return new People1(pers);
    }

}
//实现IEnumerator接口
public class People1 : IEnumerator
{
    public Person[] pers;
    public People1(Person[] per)
    {
        this.pers = per;
    }
    int position = -1;
    public bool MoveNext()
    {
        position++;
        return position < pers.Length;
    }
    public void Reset()
    {
        position=-1;
    }
    public object Current
    {
        get
        {
            try
            {
                return pers[position];
            }
            catch(IndexOutOfRangeException ex)
            {
                throw new InvalidOperationException();
            }
        }
    }
}
```

[关于IEnumerable和IEnumerator的区别](https://www.cnblogs.com/shaosks/archive/2011/09/27/2193270.html)

## 迭代器模式
在.NET中，迭代器模式被IEnumerator和IEnumerable及其对应的泛型接口所封装。如果一个类实现了IEnumerable接口，那么就能够被迭代；调用GetEnumerator方法将返回IEnumerator接口的实现，它就是迭代器本身。迭代器类似数据库中的游标，他是数据序列中的一个位置记录。迭代器只能向前移动，同一数据序列中可以有多个迭代器同时对数据进行操作。  
简单迭代器示例：
```c#
class IterationSampleEnumerator : IEnumerator
{
    IterationSample parent;//迭代的对象  #1
    Int32 position;//当前游标的位置 #2
    internal IterationSampleEnumerator(IterationSample parent)
    {
        this.parent = parent;
        position = -1;// 数组元素下标从0开始，初始时默认当前游标设置为 -1，即在第一个元素之前， #3
    }

    public bool MoveNext()
    {
        if (position != parent.values.Length) //判断当前位置是否为最后一个，如果不是游标自增 #4
        {
            position++;
        }
        return position < parent.values.Length;
    }

    public object Current
    {
        get
        {
            if (position == -1 || position == parent.values.Length)//第一个之前和最后一个自后的访问非法 #5
            {
                throw new InvalidOperationException();
            }
            Int32 index = position + parent.startingPoint;//考虑自定义开始位置的情况  #6
            index = index % parent.values.Length;
            return parent.values[index];
        }
    }

    public void Reset()
    {
        position = -1;//将游标重置为-1  #7
    }
    // 使用时调用以下方法
    
}

public class A : IEnumerable
{

    public IEnumerator GetEnumerator()
    {
        return new IterationSampleEnumerator(this);
    }
} 

```

c# 2.0后基于yield语句可实现迭代器。下面用GetEnumerator方法实现上述类中的迭代器中的MoveNext，Current和Reset方法。
```c#
public class A : IEnumerable
{
    public IEnumerator GetEnumerator()
    {
        for (int index = 0; index < this.values.Length; index++)
        {
            yield return values[(index + startingPoint) % values.Length];
        }
    }
}
```
yield return指示编译器该方法非普通方法，且该方法会返回IEnumerator对象。当然也可以指定GetEnumertor的返回类型，使其能够使用迭代块来执行迭代方法并返回一个IEnumerable需要实现的类型，IEnumerator或者对应的泛型。**如果实现的是非泛型接口，则yield return的类型为object，否则返回IEnumeratble<T>中T所指定的类型**  
迭代器的执行顺序一般为：
- 它需要一些初始的状态
- 当MoveNext被调用时，他需要执行GetEnumerator方法中的代码来准备下一个待返回的数据。
- 当调用Current属性是，需要返回yielded的值。
- 需要知道什么时候迭代结束是，MoveNext会返回false

yield break：可以提前中止迭代器的迭代过程，示例：
```c#
static IEnumerable<Int32> CountWithTimeLimit(DateTime limit)
{
    try
    {
        for (int i = 1; i <= 100; i++)
        {
            if (DateTime.Now >= limit)
            {
                yield break;
            }
            yield return i;
        }
    }
    finally  // 该语句块仅会在 yield break 后执行
    {
        Console.WriteLine("停止迭代！"); Console.ReadKey();
    }
}
static void Main(string[] args)
{
    DateTime stop = DateTime.Now.AddSeconds(2);
    foreach (Int32 i in CountWithTimeLimit(stop))
    {
        Console.WriteLine("返回 {0}", i);
        Thread.Sleep(300);
    }
}
```
尽量使用foreach语句去迭代迭代器中的内容。如下示例，提前中止迭代器迭代流程，调用了return后，foreach语句会调用GetEnumerator返回的迭代器的Dispose方法。在结束迭代之前调用包含迭代块的迭代器的Dispose方法时，状态机会执行在迭代器范围内处于暂停状态下的代码范围内的所有finally块。  
```c#
DateTime stop = DateTime.Now.AddSeconds(2);
foreach (Int32 i in CountWithTimeLimit(stop))
{
    if (i > 3)
    {
        Console.WriteLine("返回中^");
        return;
    }
    Thread.Sleep(300);
}
/*
返回 1
返回 2
返回 3
返回 4
返回中 ......
停止迭代
*/
```

[迭代器详解](https://www.cnblogs.com/yangecnu/archive/2012/03/17/2402432.html)

## 泛型接口中的协变和抗变
https://segmentfault.com/a/1190000007005115

C# 中对于对象（即对象引用），仅存在一种隐式类型转换，即 子类型的对象引用到父类型的对象引用的转换。这里的变化指的就是这种 子->父 的类型转换。
```c#
object o = "hello";
```
以下实例
```c#
string s = "abc";
object o = Method(s);
```
其实质是发生了如下两次转换
1. string Method(object o) 可变换为 string Method(string o)
2. string Method(string o) 可变换为 object Method(string o)

函数的 输入类型（函数参数） 可由 object 变换为 string，父->子
在函数输出时，函数的 输出类型（返回值） 可由string变换为object，子->父

#### 接口仅用于输出（返回值）的情况，协变
```c#
interface IDemo<out T>
{
    //仅将类型 T 用于输出
    T Method(object value);
}

public class Demo : IDemo<string>
{
    //实现接口
    public string Method (object value)
    {
        //别忘了类型转换！
        return value.ToString();
    }
}    
.....
// 可将 string Method (object value) 转换为 object Method (object value)
IDemo<string> demoStr = new Demo();
IDemo<object> demoObj = demoStr;
```
#### 接口仅用于输入（函数参数）的情况，逆变
```c#
interface IDemo<in T>
{
    //仅将类型 T 用于输入
    string Method(T value);
}

public class Demo : IDemo<object>
{
    //实现接口
    public string Method (object value)
    {
        return value.ToString();
    }
}    
......
// 可将 IDemo<object> 类型转换为 IDemo<string> 类型
IDemo<object> demoObj = new Demo();
IDemo<string> demoStr = demoObj;
```
