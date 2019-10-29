# 基础部分理解
## 关于相等
有时需要比较两个值是否相等。 在某些情况下，测试的是“值相等性” ，也称为“等效性” ，这意味着两个变量包含的值相等。  
在其他情况下，必须确定两个变量是否引用内存中的同一基础对象。 此类型的相等性称为“引用相等性” 或“标识” 。

### 1、引用相等
引用相等性指两个对象引用均引用同一基础对象。使用静态的Object.ReferenceEquals方法就可以确定两个引用是否引用同一对象。
```C#
public static bool ReferenceEquals (object objA, object objB);

using System;
class MyClass {

   static void Main() {
      object o = null;
      object p = null;
      object q = new Object();

      Console.WriteLine(Object.ReferenceEquals(o, p));
      p = q;
      Console.WriteLine(Object.ReferenceEquals(p, q));
      Console.WriteLine(Object.ReferenceEquals(o, p));
   }
}
/*
This code produces the following output.
True
True
False
*/
```
### 2、值相等性
值相等性指两个对象包含相同的一个或多个值。对于基元类型（int或bool）直接使用 == 进行比较。而对于其它大部分类型而言，值相等性通常定义为所有字段或属性具有相同的值。  
可以基于类型中所有字段和属性的比较结果来定义值相等性，也可以基于子集进行定义。 但在任何一种情况下，类和结构中的实现均应遵循 5 个等效性保证条件：
1. x.Equals(x) 返回 true。 这称为自反属性。
2. x.Equals(y) 返回与 y.Equals(x) 相同的值。 这称为对称属性。
3. 如果 (x.Equals(y) && y.Equals(z)) 返回 true，则 x.Equals(z) 返回 true。 这称为可传递属性。
4. 只要未修改 x 和 y 引用的对象，x.Equals(y) 的连续调用将返回相同的值。
5. x.Equals(null) 返回 false。 但是，null.Equals(null) 会引发异常；它未遵循上面的第二条规则。

为类对象定义值相等性比较方法：  
在类（引用类型）上，两种 Object.Equals(Object) 方法的默认实现均执行引用相等性比较（ReferenceEqual方法），而不是值相等性检查。 实施者替代虚方法时，目的是为其指定值相等性语义。  
即使类不重载 == 和 != 运算符，也可将这些运算符与类一起使用。 但是，默认行为是执行引用相等性检查（默认已经将 == 和 != 重载为执行引用相等性比较）。 在类中，如果重载 Equals 方法，则应重载 == 和 != 运算符，但这并不是必需的。
```C# 
namespace ValueEquality
{
    using System;
    class TwoDPoint : IEquatable<TwoDPoint>  // 要实现IEquatable接口，覆写里面的Equal方法才能做值相等性比较
    {
        // Readonly auto-implemented properties.
        public int X { get; private set; }
        public int Y { get; private set; }

        // Set the properties in the constructor.
        public TwoDPoint(int x, int y)
        {
            if ((x < 1) || (x > 2000) || (y < 1) || (y > 2000))
            {
                throw new System.ArgumentException("Point must be in range 1 - 2000");
            }
            this.X = x;
            this.Y = y;
        }

        public override bool Equals(object obj)
        {
            return this.Equals(obj as TwoDPoint);
        }

        public bool Equals(TwoDPoint p)
        {
            // If parameter is null, return false.
            if (Object.ReferenceEquals(p, null))
            {
                return false;
            }

            // Optimization for a common success case.
            if (Object.ReferenceEquals(this, p))
            {
                return true;
            }

            // If run-time types are not exactly the same, return false.
            if (this.GetType() != p.GetType())
            {
                return false;
            }

            // Return true if the fields match.
            // Note that the base class is not invoked because it is
            // System.Object, which defines Equals as reference equality.
            return (X == p.X) && (Y == p.Y);
        }

        public override int GetHashCode()
        {
            return X * 0x00010000 + Y;
        }

        public static bool operator ==(TwoDPoint lhs, TwoDPoint rhs)
        {
            // Check for null on left side.
            if (Object.ReferenceEquals(lhs, null))
            {
                if (Object.ReferenceEquals(rhs, null))
                {
                    // null == null = true.
                    return true;
                }

                // Only the left side is null.
                return false;
            }
            // Equals handles case of null on right side.
            return lhs.Equals(rhs);
        }

        public static bool operator !=(TwoDPoint lhs, TwoDPoint rhs)
        {
            return !(lhs == rhs);
        }
    }

    // For the sake of simplicity, assume a ThreeDPoint IS a TwoDPoint.
    class ThreeDPoint : TwoDPoint, IEquatable<ThreeDPoint>
    {
        public int Z { get; private set; }

        public ThreeDPoint(int x, int y, int z)
            : base(x, y)
        {
            if ((z < 1) || (z > 2000))
            {
                throw new System.ArgumentException("Point must be in range 1 - 2000");
            }    
            this.Z = z;
        }

        public override bool Equals(object obj)
        {
            return this.Equals(obj as ThreeDPoint);
        }

        public bool Equals(ThreeDPoint p)
        {
            // If parameter is null, return false.
            if (Object.ReferenceEquals(p, null))
            {
                return false;
            }

            // Optimization for a common success case.
            if (Object.ReferenceEquals(this, p))
            {
                return true;
            }

            // Check properties that this class declares.
            if (Z == p.Z)
            {
                // Let base class check its own fields 
                // and do the run-time type comparison.
                return base.Equals((TwoDPoint)p);
            }
            else
            {
                return false;
            }    
        }

        public override int GetHashCode()
        {
            return (X * 0x100000) + (Y * 0x1000) + Z;
        }

        public static bool operator ==(ThreeDPoint lhs, ThreeDPoint rhs)
        {
            // Check for null.
            if (Object.ReferenceEquals(lhs, null))
            {
                if (Object.ReferenceEquals(rhs, null))
                {
                    // null == null = true.
                    return true;
                }

                // Only the left side is null.
                return false;
            }
            // Equals handles the case of null on right side.
            return lhs.Equals(rhs);
        }

        public static bool operator !=(ThreeDPoint lhs, ThreeDPoint rhs)
        {
            return !(lhs == rhs);
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            ThreeDPoint pointA = new ThreeDPoint(3, 4, 5);
            ThreeDPoint pointB = new ThreeDPoint(3, 4, 5);
            ThreeDPoint pointC = null;
            int i = 5;

            Console.WriteLine("pointA.Equals(pointB) = {0}", pointA.Equals(pointB));
            Console.WriteLine("pointA == pointB = {0}", pointA == pointB);
            Console.WriteLine("null comparison = {0}", pointA.Equals(pointC));
            Console.WriteLine("Compare to some other type = {0}", pointA.Equals(i));

            TwoDPoint pointD = null;
            TwoDPoint pointE = null;



            Console.WriteLine("Two null TwoDPoints are equal: {0}", pointD == pointE);

            pointE = new TwoDPoint(3, 4);
            Console.WriteLine("(pointE == pointA) = {0}", pointE == pointA);
            Console.WriteLine("(pointA == pointE) = {0}", pointA == pointE);
            Console.WriteLine("(pointA != pointE) = {0}", pointA != pointE);

            System.Collections.ArrayList list = new System.Collections.ArrayList();
            list.Add(new ThreeDPoint(3, 4, 5));
            Console.WriteLine("pointE.Equals(list[0]): {0}", pointE.Equals(list[0]));

            // Keep the console window open in debug mode.
            System.Console.WriteLine("Press any key to exit.");
            System.Console.ReadKey();
        }
    }

    /* Output:
        pointA.Equals(pointB) = True
        pointA == pointB = True
        null comparison = False
        Compare to some other type = False
        Two null TwoDPoints are equal: True
        (pointE == pointA) = False
        (pointA == pointE) = False
        (pointA != pointE) = True
        pointE.Equals(list[0]): False
    */
}
```
结构体：  
对于结构，Object.Equals(Object)（System.ValueType 中的替代版本）的默认实现通过使用反射来比较类型中每个字段的值，从而执行值相等性检查。也就是实质上结构体派生自System.ValueType，而System.ValueType实现了Equal方法，该方法可以用于比较结构体的值相等性。  
实施者替代结构中的 Equals 虚方法时，目的是提供更高效的方法来执行值相等性检查，并选择根据结构字段或属性的某个子集来进行比较。
```C#
struct TwoDPoint : IEquatable<TwoDPoint>
    {
        // Read/write auto-implemented properties.
        public int X { get; private set; }
        public int Y { get; private set; }

        public TwoDPoint(int x, int y)
            : this()
        {
            X = x;
            Y = x;
        }

        public override bool Equals(object obj)
        {
            if (obj is TwoDPoint)  // 先做类型判断
            {
                return this.Equals((TwoDPoint)obj);  
            }
            return false;
        }

        public bool Equals(TwoDPoint p)
        {
            return (X == p.X) && (Y == p.Y);
        }

        public override int GetHashCode()
        {
            return X ^ Y;
        }

        public static bool operator ==(TwoDPoint lhs, TwoDPoint rhs)
        {
            return lhs.Equals(rhs);
        }

        public static bool operator !=(TwoDPoint lhs, TwoDPoint rhs)
        {
            return !(lhs.Equals(rhs));
        }
    }


    class Program
    {
        static void Main(string[] args)
        {
            TwoDPoint pointA = new TwoDPoint(3, 4);
            TwoDPoint pointB = new TwoDPoint(3, 4);
            int i = 5;

            // Compare using virtual Equals, static Equals, and == and != operators.
            // True:
            Console.WriteLine("pointA.Equals(pointB) = {0}", pointA.Equals(pointB));
            // True:
            Console.WriteLine("pointA == pointB = {0}", pointA == pointB);
            // True:
            Console.WriteLine("Object.Equals(pointA, pointB) = {0}", Object.Equals(pointA, pointB));
            // False:
            Console.WriteLine("pointA.Equals(null) = {0}", pointA.Equals(null));
            // False:
            Console.WriteLine("(pointA == null) = {0}", pointA == null);
            // True:
            Console.WriteLine("(pointA != null) = {0}", pointA != null);
            // False:
            Console.WriteLine("pointA.Equals(i) = {0}", pointA.Equals(i));
            // CS0019:
            // Console.WriteLine("pointA == i = {0}", pointA == i); 

            // Compare unboxed to boxed.
            System.Collections.ArrayList list = new System.Collections.ArrayList();
            list.Add(new TwoDPoint(3, 4));
            // True:
            Console.WriteLine("pointE.Equals(list[0]): {0}", pointA.Equals(list[0]));


            // Compare nullable to nullable and to non-nullable.
            TwoDPoint? pointC = null;
            TwoDPoint? pointD = null;
            // False:
            Console.WriteLine("pointA == (pointC = null) = {0}", pointA == pointC);
            // True:
            Console.WriteLine("pointC == pointD = {0}", pointC == pointD);

            TwoDPoint temp = new TwoDPoint(3, 4);
            pointC = temp;
            // True:
            Console.WriteLine("pointA == (pointC = 3,4) = {0}", pointA == pointC);

            pointD = temp;
            // True:
            Console.WriteLine("pointD == (pointC = 3,4) = {0}", pointD == pointC);

            // Keep the console window open in debug mode.
            System.Console.WriteLine("Press any key to exit.");
            System.Console.ReadKey();
        }
    }

    /* Output:
        pointA.Equals(pointB) = True
        pointA == pointB = True
        Object.Equals(pointA, pointB) = True
        pointA.Equals(null) = False
        (pointA == null) = False
        (pointA != null) = True
        pointA.Equals(i) = False
        pointE.Equals(list[0]): True
        pointA == (pointC = null) = False
        pointC == pointD = True
        pointA == (pointC = 3,4) = True
        pointD == (pointC = 3,4) = True
    */
}
```
## 类型 和 类型转换
### 1、CTS
- 它支持继承原则。 类型可以派生自其他类型（称为基类型）。 派生类型继承（有一些限制，公有私有限制）基类型的方法、属性和其他成员。 基类型可以继而从某种其他类型派生，在这种情况下，派生类型继承其继承层次结构中的两种基类型的成员。 
- 所有类型（包括 System.Int32C# 关键字：int等内置数值类型）最终都派生自单个基类型，即 System.Object（C# 关键字：object。 这样的统一类型层次结构称为通用类型系统 (CTS)。
- CTS 中的每种类型被定义为**值类型或引用类型**。 这包括 .NET 类库中的所有自定义类型以及你自己的用户定义类型。 使用 struct 关键字定义的类型是值类型；所有内置数值类型都是 structs。 使用 class 关键字定义的类型是引用类型。 引用类型和值类型遵循不同的编译时规则和运行时行为。
![](/assets/reference&value-1.png)


### 2、值类型
值类型派生自System.ValueType（派生自 System.Object）。 派生自 System.ValueType 的类型在 CLR 中具有特殊行为。 值类型变量直接包含它们的值，这意味着在声明变量的任何上下文中内联分配内存。 对于值类型变量，没有单独的堆分配或垃圾回收开销。

值类型分为 结构体 和 枚举（枚举定义的是一组已命名的整型常量） 两类。

内置的数值类型（byte，int，int32等）都是结构体，包含可访问的属性和方法。

所有枚举从 System.Enum（继承自 System.ValueType）继承。


### 3、引用类型
定义为类、委托、数组(即使元素是值类型)或接口的类型是引用类型。 在运行时，当声明引用类型的变量时，该变量会一直包含值 null，直至使用 new 运算符显式创建对象，或者为该变量分配已经在其他位置使用 new 创建的对象。

接口必须与实现它的类对象一起初始化。
```C#
IMyInterface iface = new MyClass();
```
创建对象后，内存会在托管堆上进行分配，并且变量只保留对对象位置的引用。 对于托管堆上的类型，在分配内存和 CLR 自动内存管理功能（称为“垃圾回收”）回收内存时都会产生开销。

引用类型完全支持继承。 创建类时，可以继承自其他任何未定义为密封的接口或类，而其他类也可以继承自你的类并重写你的虚方法。

### 类型转换
- 隐式转换
  这种转换是类型安全且不会导致数据丢失，因此无需使用任何特殊语法。
- 显式转换
  必须使用强制转换运算符，才能执行显式转换。 在转换中可能丢失信息时或在出于其他原因转换可能不成功时，必须进行强制转换。  
  若要执行强制转换，请在要转换的值或变量前面的括号中指定要强制转换到的类型。
  ```C#
    class Test
    {
        static void Main()
        {
            double x = 1234.7;
            int a;
            // Cast double to int.
            a = (int)x;
            System.Console.WriteLine(a);
        }
    }
    // Output: 1234
  ```
  对于引用类型，如果需要从基类型转换为派生类型，则必须进行显式强制转换
  ```C#
    // Create a new derived type.  
    Giraffe g = new Giraffe();  
    
    // Implicit conversion to base type is safe.子类隐式转换为基类是safe的  
    Animal a = g;  
    
    // Explicit conversion is required to cast back  
    // to derived type. Note: This will compile but will  
    // throw an exception at run time if the right-side  
    // object is not in fact a Giraffe.  
    // 因为父类不带有子类的全部属性，因此可能导致运行时错误
    Giraffe g2 = (Giraffe) a;  
  ```
- 用户自定义转换
  用户定义的转换是使用特殊方法执行，这些方法可定义为在没有基类和派生类关系的自定义类型之间启用显式转换和隐式转换

使用 is 运算符执行强制转换，该运算符会提前测试兼容性：
```C#
class Animal
{
    public void Eat() { Console.WriteLine("Eating."); }
    public override string ToString()
    {
        return "I am an animal.";
    }
}
class Mammal : Animal { }
class Giraffe : Mammal { }

class SuperNova { }

class Program
{
    static void Main(string[] args)
    {
        Giraffe g = new Giraffe();
        FeedMammals(g);

        TestForMammals(g);

        SuperNova sn = new SuperNova();
        TestForMammals(sn);
    }

    static void FeedMammals(Animal a)
    {
        // Use the is operator to verify the type
        // before performing a cast.
        if (a is Mammal m)
        {
            m.Eat();
        }
    }

    static void TestForMammals(object o)
    {
        // Alternatively, use the as operator and test for null
        // before referencing the variable.
        if (o is Mammal m)  
        {
            Console.WriteLine(m.ToString());
        }
        else
        {
            // variable 'm' is not in scope here, and can't be used.
            Console.WriteLine($"{o.GetType().Name} is not a Mammal");
        }
    }
}
```
如果想要测试变量是否为给定类型，但不将其分配给新变量，则可以对引用类型和可以为 null 的类型使用 is 和 as 运算符。 以下代码演示如何在引入模式匹配以测试变量是否为给定类型前，使用 C# 语言中的 is 和 as 语句.
```C#
class Animal
{
    public void Eat() { Console.WriteLine("Eating."); }
    public override string ToString()
    {
        return "I am an animal.";
    }
}
class Mammal : Animal { }
class Giraffe : Mammal { }

class SuperNova { }


class Program
{
    static void Main(string[] args)
    {
        // Use the is operator to verify the type.
        // before performing a cast.
        Giraffe g = new Giraffe();
        UseIsOperator(g);

        // Use the as operator and test for null
        // before referencing the variable.
        UseAsOperator(g);

        // Use the as operator to test
        // an incompatible type.
        SuperNova sn = new SuperNova();
        UseAsOperator(sn);

        // Use the as operator with a value type.
        // Note the implicit conversion to int? in 
        // the method body.
        int i = 5;
        UseAsWithNullable(i);

        double d = 9.78654;
        UseAsWithNullable(d);
    }

    static void UseIsOperator(Animal a)
    {
        if (a is Mammal)
        {
            Mammal m = (Mammal)a;
            m.Eat();
        }
    }

    static void UsePatternMatchingIs(Animal a)
    {
        if (a is Mammal m)
        {
            m.Eat();
        }
    }

    static void UseAsOperator(object o)
    {
        Mammal m = o as Mammal;  // 先尝试转换o为Mammal，如果不成功则m为null
        if (m != null)
        {
            Console.WriteLine(m.ToString());
        }
        else
        {
            Console.WriteLine($"{o.GetType().Name} is not a Mammal");
        }
    }

    static void UseAsWithNullable(System.ValueType val)
    {
        int? j = val as int?;
        if (j != null)
        {
            Console.WriteLine(j);
        }
        else
        {
            Console.WriteLine("Could not convert " + val.ToString());
        }
    }
}
```

### 装箱拆箱
实质是值类型转引用类型（装箱）以及引用类型转回值类型（拆箱）。装箱是将值类型转换为 object 类型或由此值类型实现的任何接口类型的过程。 CLR 对值类型进行装箱时，会将值包装在 System.Object 实例中并将其存储在托管堆中。 取消装箱将从对象中提取值类型。 装箱是隐式的；取消装箱是显式的。 

```C#
// String.Concat example.
// String.Concat has many versions. Rest the mouse pointer on 
// Concat in the following statement to verify that the version
// that is used here takes three object arguments. Both 42 and
// true must be boxed.
Console.WriteLine(String.Concat("Answer", 42, true));


// List example.
// Create a list of objects to hold a heterogeneous collection 
// of elements.
List<object> mixedList = new List<object>();

// Add a string element to the list. 
mixedList.Add("First Group:");  // 装箱，将string类型转为object

// Add some integers to the list. 
for (int j = 1; j < 5; j++)
{
    // Rest the mouse pointer over j to verify that you are adding
    // an int to a list of objects. Each element j is boxed when 
    // you add j to mixedList.
    mixedList.Add(j);  // 装箱，将int类型转为object
}

// Add another string and more integers.
mixedList.Add("Second Group:");  // 装箱，将string类型转为object
for (int j = 5; j < 10; j++)
{
    mixedList.Add(j);  // 装箱，将int类型转为object
}

// Display the elements in the list. Declare the loop variable by 
// using var, so that the compiler assigns its type.
foreach (var item in mixedList)
{
    // Rest the mouse pointer over item to verify that the elements
    // of mixedList are objects.
    Console.WriteLine(item);
}

// The following loop sums the squares of the first group of boxed
// integers in mixedList. The list elements are objects, and cannot
// be multiplied or added to the sum until they are unboxed. The
// unboxing must be done explicitly.
var sum = 0;
for (var j = 1; j < 5; j++)
{
    // The following statement causes a compiler error: Operator 
    // '*' cannot be applied to operands of type 'object' and
    // 'object'. 
    //sum += mixedList[j] * mixedList[j]);

    // After the list elements are unboxed, the computation does 
    // not cause a compiler error.
    sum += (int)mixedList[j] * (int)mixedList[j];  // 拆箱
}

// The sum displayed is 30, the sum of 1 + 4 + 9 + 16.
Console.WriteLine("Sum: " + sum);

// Output:
// Answer42True
// First Group:
// 1
// 2
// 3
// 4
// Second Group:
// 5
// 6
// 7
// 8
// 9
// Sum: 30
```
int i = 123;  到  object o = i; 这个过程是将i的值拷贝一份后放入o中。  
![](/assets/reference&value-2.png)

取消装箱的操作为：检查对象实例，以确保它是给定值类型的装箱值。然后将该值从实例复制到值类型变量中。  
![](/assets/reference&value-3.png)

无效装箱拆箱会引发InvalidCastException 和 NullReferenceException（尝试拆null）
```C#
class TestUnboxing
{
    static void Main()
    {
        int i = 123;
        object o = i;  // implicit boxing

        try
        {
            int j = (short)o;  // attempt to unbox， 转换为不同类型

            System.Console.WriteLine("Unboxing OK.");
        }
        catch (System.InvalidCastException e)
        {
            System.Console.WriteLine("{0} Error: Incorrect unboxing.", e.Message);
        }
    }
}
```

常见的转换问题解决方法：
1. 字节数组转int：https://docs.microsoft.com/zh-cn/dotnet/csharp/programming-guide/types/how-to-convert-a-byte-array-to-an-int
2. 字符串转数字：https://docs.microsoft.com/zh-cn/dotnet/csharp/programming-guide/types/how-to-convert-a-string-to-a-number
3. 十六进制字符串和数值类型间转换：https://docs.microsoft.com/zh-cn/dotnet/csharp/programming-guide/types/how-to-convert-between-hexadecimal-strings-and-numeric-types

