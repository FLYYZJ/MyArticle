# MSDN 和 C# 图解教程方面关于类和对象的学习

## 类和结构体
类是引用类型，类对象的变量引用该对象在托管堆上的地址。如果将同一类型的第二个对象分配给第一个对象，则两个变量都引用该地址的对象。类对象使用new关键字进行初始化。
```C#
public class Person
{
    public string Name { get; set; }
    public int Age { get; set; }
    public Person(string name, int age)
    {
        Name = name;
        Age = age;
    }
    // Other properties, methods, events...
}

class Program
{
    static void Main()
    {
        Person person1 = new Person("Leopold", 6);
        Console.WriteLine("person1 Name = {0} Age = {1}", person1.Name, person1.Age);

        // Declare new person, assign person1 to it.
        Person person2 = person1;

        // Change the name of person2, and person1 also changes. 体现了对象变量引用地址的特性
        person2.Name = "Molly";
        person2.Age = 16;

        Console.WriteLine("person2 Name = {0} Age = {1}", person2.Name, person2.Age);
        Console.WriteLine("person1 Name = {0} Age = {1}", person1.Name, person1.Age);

        // Keep the console open in debug mode.
        Console.WriteLine("Press any key to exit.");
        Console.ReadKey();

    }
}
/*
    Output:
    person1 Name = Leopold Age = 6
    person2 Name = Molly Age = 16
    person1 Name = Molly Age = 16
*/
```
结构是值类型，因此结构对象的变量具有整个对象的副本。 结构的实例也可以使用 new 运算符来创建，但这不是必需的。
```C#
public struct Person
{
    public string Name;
    public int Age;
    public Person(string name, int age)
    {
        Name = name;
        Age = age;
    }
}

public class Application
{
    static void Main()
    {
        // Create  struct instance and initialize by using "new".
        // Memory is allocated on thread stack.
        Person p1 = new Person("Alex", 9);
        Console.WriteLine("p1 Name = {0} Age = {1}", p1.Name, p1.Age);

        // Create  new struct object. Note that  struct can be initialized
        // without using "new".
        Person p2 = p1;

        // Assign values to p2 members.
        p2.Name = "Spencer";
        p2.Age = 7;
        Console.WriteLine("p2 Name = {0} Age = {1}", p2.Name, p2.Age);

        // p1 values remain unchanged because p2 is  copy.
        Console.WriteLine("p1 Name = {0} Age = {1}", p1.Name, p1.Age);

        // Keep the console open in debug mode.
        Console.WriteLine("Press any key to exit.");
        Console.ReadKey();
    }
}
/*
  Output:
    p1 Name = Alex Age = 9
    p2 Name = Spencer Age = 7
    p1 Name = Alex Age = 9
*/
```
p1 和 p2 的内存在线程堆栈上进行分配。 该内存随声明它的类型或方法一起回收。 这就是在赋值时复制结构的一个原因。 相比之下，当对类实例对象的所有引用都超出范围时，为该类实例分配的内存将由公共语言运行时自动回收（垃圾回收）。
## 关于对象比较（标识还是值比较）
在比较两个对象是否相等时，首先必须明确是想知道两个变量是否表示内存中的同一对象，还是想知道这两个对象的一个或多个字段的值是否相等。 如果要对值进行比较，则必须考虑这两个对象是值类型（结构）的实例，还是引用类型（类、委托、数组）的实例。
- 若要确定两个类实例是否引用内存中的同一位置（这意味着它们具有相同的标识 ），可使用**静态 Equals 方法**。 （System.Object 是所有值类型和引用类型的隐式基类，其中包括用户定义的结构和类。）
- 若要确定两个结构实例中的实例字段是否具有相同的值，可使用 ValueType.Equals 方法。 由于**所有结构都隐式继承自 System.ValueType，因此可以直接在对象上调用该方法**，如以下示例所示：
```C#
// Person is defined in the previous example.

//public struct Person
//{
//    public string Name;
//    public int Age;
//    public Person(string name, int age)
//    {
//        Name = name;
//        Age = age;
//    }
//}

Person p1 = new Person("Wallace", 75);
Person p2;
p2.Name = "Wallace";
p2.Age = 75;

if (p2.Equals(p1))
    Console.WriteLine("p2 and p1 have the same values.");

// Output: p2 and p1 have the same values.
```

- 若要确定两个类实例中字段的值是否相等，可以使用 Equals 方法或 == 运算符。 但是，**只有类通过重写或重载提供关于那种类型对象的“相等”含义的自定义时，才能使用它们**。 类也可能实现 IEquatable<T> 接口或 IEqualityComparer<T> 接口。 这两个接口都提供可用于测试值相等性的方法。 设计自己的替代 Equals 的类后，请务必遵循操作说明：为类型定义值相等性和 Object.Equals(Object) 中介绍的准则。