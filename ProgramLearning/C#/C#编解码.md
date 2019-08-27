# hex编码和解码
unicode字符串转hex编码字符串
```C#
/// 从字符串转换到16进制表示的字符串
/// 编码,如"utf-8","gb2312"
/// 是否每字符用逗号分隔
public static string ToHex(string s, string charset, bool fenge)
{
    if ((s.Length % 2) != 0)
    {
        s += " ";//空格
        //throw new ArgumentException("s is not valid chinese string!");
    }
    System.Text.Encoding chs = System.Text.Encoding.GetEncoding(charset);
    byte[] bytes = chs.GetBytes(s);
    string str = "";
    for (int i = 0; i < bytes.Length; i++)
    {
        str += string.Format("{0:X}", bytes[i]);
        if (fenge && (i != bytes.Length - 1))
        {
            str += string.Format("{0}", ",");
        }
    }
    return str.ToLower();
}
```
hex编码字符串转Unicode字符串
```C#
/// 从16进制转换成utf编码的字符串
/// 编码,如"utf-8","gb2312"
public static string UnHex(string hex, string charset)
{
    if (hex == null)
        throw new ArgumentNullException("hex");
    hex = hex.Replace(",", "");
    hex = hex.Replace("\n", "");
    hex = hex.Replace("\\", "");
    hex = hex.Replace(" ", "");
    if (hex.Length % 2 != 0)
    {
        hex += "20";//空格
        throw new ArgumentException("hex is not a valid number!", "hex");
    }
    // 需要将 hex 转换成 byte 数组。
    byte[] bytes = new byte[hex.Length / 2];
    for (int i = 0; i < bytes.Length; i++)
    {
        try
        {
            // 每两个字符是一个 byte。
            bytes[i] = byte.Parse(hex.Substring(i * 2, 2),
            System.Globalization.NumberStyles.HexNumber);
        }
        catch
        {
            // Rethrow an exception with custom message.
            throw new ArgumentException("hex is not a valid hex number!", "hex");
        }
    }
    System.Text.Encoding chs = System.Text.Encoding.GetEncoding(charset);
    return chs.GetString(bytes);
}
```

# 编码转换