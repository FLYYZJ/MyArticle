## 基本socket使用
```C#
using System.Net;
using System.Net.Sockets;
public static void ShortLink(string input)
{
    //设定服务器IP地址  
    IPAddress ip = IPAddress.Parse("127.0.0.1");
    Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
    try
    {
        clientSocket.Connect(new IPEndPoint(ip, 2000)); //配置服务器IP与端口  
        Console.WriteLine("连接服务器成功");
    }
    catch
    {
        Console.WriteLine("连接服务器失败，请按回车键退出！");
        Console.ReadLine();
        return;
    }

    string sendMessage = "你好";//发送到服务端的内容
    clientSocket.Send(Encoding.UTF8.GetBytes(sendMessage));//向服务器发送数据，需要发送中文则需要使用Encoding.UTF8.GetBytes()，否则会乱码
    Console.WriteLine("向服务器发送消息：" + sendMessage);

    //接受从服务器返回的信息
    string recvStr = "";
    byte[] recvBytes = new byte[1024];
    int bytes;
    bytes = clientSocket.Receive(recvBytes, recvBytes.Length, 0);    //从服务器端接受返回信息 
    recvStr += Encoding.UTF8.GetString(recvBytes, 0, bytes);
    Console.WriteLine("服务端发来消息：{0}", recvStr);//回显服务器的返回信息
    //每次完成通信后，关闭连接并释放资源
    clientSocket.Close();
    Console.ReadLine();
}

public static void LongLink()
{
    //设定服务器IP地址  
    IPAddress ip = IPAddress.Parse("127.0.0.1");
    Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
    try
    {
        clientSocket.Connect(new IPEndPoint(ip, 2000)); //配置服务器IP与端口  
        Console.WriteLine("连接服务器成功");
    }
    catch
    {
        Console.WriteLine("连接服务器失败，请按回车键退出！");
        Console.ReadLine();
        return;
    }

    //循环读取输入数据
    while (true)
    {
        Console.WriteLine("请输入");
        string sendstr = Console.ReadLine();
        SentMsg(sendstr, clientSocket);
    }
}
```
## 封装后的TCPClient、TCPListener和UDPClient，以及NetworkStream（网络流，用于真正的读写数据）
TcpListener & TcpClient，可以看作为对socket的进一步封装(基于tcp协议)，TcpListener为服务器端，TcpClient为客户端。  
TcpClient 和 TcpListener 使用 NetworkStream 类表示网络。使用 GetStream 方法返回网络流，然后调用该流的 Read 和 Write 方法。NetworkStream 不拥有协议类的基础套接字，因此关闭它并不影响套接字。  
UdpClient 类使用字节数组保存 UDP 数据文报。使用 Send 方法向网络发送数据，使用 Receive 方法接收传入的数据文报。


## 参考
[c# socket编程](https://www.cnblogs.com/iprograming/archive/2012/10/15/csharp_socket_4.html)
[c# socket关于短连接和长连接](https://blog.csdn.net/yangwohenmai1/article/details/92589072)