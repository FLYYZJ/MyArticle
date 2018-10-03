# 常用命令：
export 环境变量名=value ： 环境变量设置，注意 = 两边不能有空格，而且该设置仅在当前打开的 terminal 中有效，要想让这个环境变量在全局中起作用，需要将该指令写入~/.bashrc 文件里面，在 source ~/.bashrc即可

unset 环境变量名 ： 删除该环境变量

env | grep 环境变量名： 输出环境变量名（或含有该环境变量名字符串的其它环境变量名）对应的值

echo $环境变量名 ： 这个$不可省略，打印该环境变量名对应的值

\ 除了作为转义字符之外还可以作为续行符使用，就算命令太长转一行继续编写，可以在当前行键入 \ 后（enter）换行然后进入下一行进行编写


# shell 脚本语法

**一个注意点 shell中 0 为真，非0 为假**

### 读取键盘输入
read var =>  将键盘输入内容写入var中

### 命令代换
注意在shell中，变量赋值： var=value，此时value都会作为一个字符串赋给var，因此如果value是某种有含义的可执行的命令，需要做一些命令代换，从而使得var可以得到命令执行的结果，如下：

date：打印当前时间

直接 DATE=date然后echo $DATE无法打印出当前时间，因为并没有执行date这个命令并赋值个DATE，因此需要使用命令代换

DATA=$(date) 或者 DATA=\`date\`，其中$()这种形式可以实现嵌套

### 单双引号的区别
DATE=date然后echo "$DATE" => 输出当前时间， 而 echo '$DATE' => 输出的是$DATE，即仅含字面意义
### 算术代换

$((var)) 将var转换为数值，因为shell中被赋值的变量其值都是字符串

var=45 echo $(($var+3)) => 输出48
### shell中的通配符：
1. \* 匹配0个或多个任意字符
2. ? 匹配一个任意字符
3. \[若干字符\] 匹配方括号中任意一个字符的一次出现

举例：例如当前目录下有t3.sh文件，此时要找到列出该文件可以：
1. ls \*
2. ls \[abcdeft\]?.sh
3. ls t\[0-2\].sh  此时0-2表示可取0，1，2这三个数字

## 条件测试 可用于 if else if else 逻辑中 （在shell中，0为真，1为假）
var=2 => \[ $var -gt 1 \]  <=> test $var -gt 1

**比较符**：
\[ ARG1 OP ARG2 \] OP 可取形式为：
> -eq 等于 -ne 不等于 -lt 小于 -le 小于等于 -gt 大于 -ge 大于等于

判断是否为文件和是否为目录： \[-d DIR \]  \[ -f FILE \] 

判断字符串的长度是否为0：   \[-z STRING \] \[-n STRING \]

判断两个string是否相同： \[ STRING1=STRING2 \] \[ STRING1!=STRING2 \]
> 特别的，当STRING赋值为空时，如 str2=，str1=hello， 然后执行 \[ str1=str2 \] 此时会报错，因为str2为空（不是空字符串），此时=后面需要带有实际的对象，因此可以使用\[ "str1"="str2" \]，此时在双引号内的"str2"会转换为一个空字符串，即是一个实际的对象，因此可以参与比较。

**因此一个重点在于 好的shell编程习惯应该是总是将变量取值放入双引号中，否则就会出现展开为空对象的情况，对于空对象，shell比较等操作会出现错误**

**逻辑操作符**
\[ !expr ] 非 \[ expr1 -a expr2 ] 与 \[ expr1 -o expr2 ] 或

\[ -d Desktop -a $VAR='abc' ] 同时比较是否为目录和是否变量为'abc'

### shell中的条件逻辑
if \[ 逻辑判断 ]; then ... else ... fi(结束if语句块)， 这里的逻辑判断就是使用前面的**条件测试**操作
> 特殊的 注意 ; 号必须存在； if :; 永远为真，即 if :; then ... fi then后面的语句块总是会被执行； 同时 ... 代表多个语句，每个语句用回车进行分隔即可;

case语句
```shell
read var
case "$var" in 
a|b|c) do something
d) do something
通配符 如 [nN]* 表示以N或n开头，后面跟上任意多个字符) do something
esac # 结束标志

```

### shell中的循环
for循环：注意分割符是空格不是逗号，同时记得有分号结束
```shell
for FRUIT in apple banana peach; then
  do something
done

for filename in `ls -l`; then
  do something
end
```
while循环：
```shell
while [ 条件逻辑 ]; do
  do something
done
```
break 和 continue，break\[n]是指定跳出n层循环（注意**循环是可以嵌套的**），continue是跳出当前循环步

### 位置参数和特殊变量
```shell
$? 上一条命令的exit status，即执行结果
$0 相当于C中的argv[0]，是传入参数的第一个参数
$1 $2 ... 即位置参数，相当于C语言中的argv[1] argv[2] ...
$# 相当于C中的argc，即对应命令行输入多少个参数
$@ 表示参数列表  "$1" "$2" "$3" ...  可以用在for循环的in后面
$* 同上
$$ 当前进程号

位置参数左移  shift => "$1" "$2" "$3" ... 转变成 "$2" "$3" "$4" ...

```

### 常用函数
echo： 参数 -e 解析转义字符 -n 不进行回车换行 echo \[option] content

管道： 将一个命令的输出作为另一个命令的输入

tee 将结果输出到标准输出中，另一个副本输出到相应的文件中
```shell
ls -l | tee a.txt => 此时会把结果输出到terminal上也输出到文件a.txt中
```

重定向： >（输出重定向） >>(追加) <(输入重定向，将stdin的文件描述符重定向到某个文件上)，如下特殊示例：
```shell
cmd < file1 > file2 输入输出重定向，指定输入为file1，然后将执行结果写入file2
cmd < file 2>&1  改变文件描述符2 指向 1
cmd < &fd 将文件描述符作为标准输入
cmd < &- 关闭标准输入
```

shell 中定义函数：无参数列表，无返回值
```shell
fun(){
  do something
}
fun #执行函数
```
shell如何实现传参： 
```shell
fun(){
  echo "$0"
  echo "$1"
  echo "$2"
  echo "$3"
}
fun 1 2 3 #执行函数，后带参数
输出为
bash  <-> $0
1 <-> $1
2 <-> $2
3 <-> $3
```

# shell中常用工具

## shell 正则表达式
[shell中的正则表达式总结](http://man.linuxde.net/docs/shell_regex.html)




