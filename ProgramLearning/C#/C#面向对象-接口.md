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