# 特性
在C#语言中，元数据一般就是类中定义的字段，属性和方法等。特性目的是在不改变元数据的情况下，为元数据附加额外的信息（附加类，结构体，字符串等，类能携带的信息更多）。  

特性可以加在很多地方，包括类，类方法，字段，属性......

特性一般和反射一起使用，当另一个类（或者本类的其它方法）调用该类的方法时，会通过反射获取该类某个元数据的所有特性，然后根据该元数据所带的特性进行信息提取并动作。

下面一个比较通俗的特性例子，该例子通过对属性A添加Range特性来限制A的取值范围。
```c#
public class Test
{
    private int a;
    [Range(1, 10, ErrorMessage = "超出范围")]
    public int A
    {
        get { return a; }
        set
        {
            ValidProperty("A", value);
        }
    }

    private void ValidProperty(string property, object value)
    {
        var type = typeof(Test);
        PropertyInfo propertyA = type.GetProperty("A");
        IEnumerable<Attribute> attributes = propertyA.GetCustomAttributes();
        foreach (var attribute in attributes)
        {
            if (attribute is RangeAttribute)
            {
                RangeAttribute rangeAttribute = attribute as RangeAttribute;
                if (!rangeAttribute.IsValid(value))
                {
                    ErrorMessages.Add(rangeAttribute.ErrorMessage);
                }
            }
        }
    }

    public List<string> ErrorMessages { get; set; } = new List<string>();
}

```
一般我们自己会定义自定义的特性，通过自定义的特性为某些元数据添加注解，自定义特性一般继承自Attribute类。