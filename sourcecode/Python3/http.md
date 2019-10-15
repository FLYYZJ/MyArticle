# 使用的基本模块
## re模块
- re.escape(pattern)  
对字符串中所有可能被解释为正则运算符的字符进行转义的应用函数。如果字符串很长且包含很多特殊技字符，而你又不想输入一大堆反斜杠，或者字符串来自于用户(比如通过raw_input函数获取输入的内容)，且要用作正则表达式的一部分的时候，可以使用这个函数。  
    ```python
    re.escape('www.python.org')
    ```

## 字符串
- translate方法：将对应table中对应的字符进行转换，规则由table确定。
    ```python
    str.translate(table)
    bytes.translate(table[, delete])    
    bytearray.translate(table[, delete]) 

    intab = "aeiou"
    outtab = "12345"
    trantab = str.maketrans(intab, outtab)   # 制作翻译表
 
    str = "this is string example....wow!!!"
    print (str.translate(trantab))
    # 输出：th3s 3s str3ng 2x1mpl2....w4w!!!
    ```
## 魔法方法
- \_\_setitem\_\_ 和 \_\_getitem\_\_，这两个魔法方法使得对应的类支持ins\['key'\] = val 和 val = ins\['key'\] 这两种操作。
    ```python
    class DictDemo:
        def __init__(self,key,value):
                self.dict = {}
                self.dict[key] = value
        def __getitem__(self,key):
                return self.dict[key]
        def __setitem__(self,key,value):
                self.dict[key] = value
    dictDemo = DictDemo('key0','value0')
    print(dictDemo['key0']) #value0
    dictDemo['key1'] = 'value1'
    print(dictDemo['key1']) #value1
    ```
- 

# http模块讲解
该模块主要分成5个文件：
- \__init__.py  =>  定义了状态码，如200， 404 这些状态码的枚举值（元组格式，状态码，状态信息,...）
- client.py
- cookiejar.py
- cookies.py
- server.py

## 解析cookie和cookiejar模块
python的cookie模块的主要功能是将HTTP的cookie转换为python字典，遵循RFC2019协议。

合法的网络字符满足：
```python
_LegalChars = r"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&\\'*+-.^_`|~:"

_UnescapedChars =r"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&\\'*+-.^_`|~: ()/<=>?@[]{}"

_Translator = {0: '\\000', 1: '\\001', 2: '\\002', 3: '\\003', 4: '\\004', 5: '\\005', 6: '\\006', 7: '\\007', 8: '\\010', 9: '\\011', 10: '\\012', 11: '\\013', 12: '\\014', 13: '\\015', 14: '\\016', 15: '\\017', 16: '\\020', 17: '\\021', 18: '\\022', 19: '\\023', 20: '\\024', 21: '\\025', 22: '\\026', 23: '\\027', 24: '\\030', 25: '\\031', 26: '\\032', 27: '\\033', 28: '\\034', 29: '\\035', 30: '\\036', 31: '\\037', 34: '\\"', 44: '\\054', 59: '\\073', 92: '\\\\', 127: '\\177', 128: '\\200', 129: '\\201', 130: '\\202', 131: '\\203', 132: '\\204', 133: '\\205', 134: '\\206', 135: '\\207', 136: '\\210', 137: '\\211', 138: '\\212', 139: '\\213', 140: '\\214', 141: '\\215', 142: '\\216', 143: '\\217', 144: '\\220', 145: '\\221', 146: '\\222', 147: '\\223', 148: '\\224', 149: '\\225', 150: '\\226', 151: '\\227', 152: '\\230', 153: '\\231', 154: '\\232', 155: '\\233', 156: '\\234', 157: '\\235', 158: '\\236', 159: '\\237', 160: '\\240', 161: '\\241', 162: '\\242', 163: '\\243', 164: '\\244', 165: '\\245', 166: '\\246', 167: '\\247', 168: '\\250', 169: '\\251', 170: '\\252', 171: '\\253', 172: '\\254', 173: '\\255', 174: '\\256', 175: '\\257', 176: '\\260', 177: '\\261', 178: '\\262', 179: '\\263', 180: '\\264', 181: '\\265', 182: '\\266', 183: '\\267', 184: '\\270', 185: '\\271', 186: '\\272', 187: '\\273', 188: '\\274', 189: '\\275', 190: '\\276', 191: '\\277', 192: '\\300', 193: '\\301', 194: '\\302', 195: '\\303', 196: '\\304', 197: '\\305', 198: '\\306', 199: '\\307', 200: '\\310', 201: '\\311', 202: '\\312', 203: '\\313', 204: '\\314', 205: '\\315', 206: '\\316', 207: '\\317', 208: '\\320', 209: '\\321', 210: '\\322', 211: '\\323', 212: '\\324', 213: '\\325', 214: '\\326', 215: '\\327', 216: '\\330', 217: '\\331', 218: '\\332', 219: '\\333', 220: '\\334', 221: '\\335', 222: '\\336', 223: '\\337', 224: '\\340', 225: '\\341', 226: '\\342', 227: '\\343', 228: '\\344', 229: '\\345', 230: '\\346', 231: '\\347', 232: '\\350', 233: '\\351', 234: '\\352', 235: '\\353', 236: '\\354', 237: '\\355', 238: '\\356', 239: '\\357', 240: '\\360', 241: '\\361', 242: '\\362', 243: '\\363', 244: '\\364', 245: '\\365', 246: '\\366', 247: '\\367', 248: '\\370', 249: '\\371', 250: '\\372', 251: '\\373', 252: '\\374', 253: '\\375', 254: '\\376', 255: '\\377'}
```

合法的cookie字符满足：
```python
_LegalKeyChars  = r"\w\d!#%&'~_`><@,:/\$\*\+\-\.\^\|\)\(\?\}\{\="

_LegalValueChars = r"\\w\\d!#%&\\'~_`><@,:/\\$\\*\\+\\-\\.\\^\\|\\)\\(\\?\\}\\{\\=\\[\\]"

# cookie 符合的模式
_CookiePattern = re.compile(r"""
    \s*                            # Optional whitespace at start of cookie
    (?P<key>                       # Start of group 'key'
    [""" + _LegalKeyChars + r"""]+?   # Any word of at least one letter
    )                              # End of group 'key'
    (                              # Optional group: there may not be a value.
    \s*=\s*                          # Equal Sign
    (?P<val>                         # Start of group 'val'
    "(?:[^\\"]|\\.)*"                  # Any doublequoted string
    |                                  # or
    \w{3},\s[\w\d\s-]{9,11}\s[\d:]{8}\sGMT  # Special case for "expires" attr
    |                                  # or
    [""" + _LegalValueChars + r"""]*      # Any word or empty string
    )                                # End of group 'val'
    )?                             # End of optional value group
    \s*                            # Any number of spaces.
    (\s+|;|$)                      # Ending either at space, semicolon, or EOS.
    """, re.ASCII | re.VERBOSE)    # re.ASCII may be removed if safe.
```

首先还是符合模块化的思想，将复杂的模块分块。作者先设计了Morsel模块，该模块主要存放单一的键值对，因为每个cookie出了键值对之外，还存在这样几个特殊的属性，例如域（domain）、版本（version），max-age（保存时间）等。Morsel模块继承自字典，其中就保留cookie的键值以及其它特殊的属性。Morsel类型自身带有key和value两个字段，用于保存键值，而使用dict的赋值方式，即d\[k]\=v 这样赋值时，是进行cookie的属性赋值。  
```python
# 属性赋值
def __setitem__(self, K, V):  # 这种形式对应在 d[k] = v 这种赋值形式时调用，这种赋值方式用于设置cookie的attribute
    K = K.lower()  # 全部转换为小写形式
    if not K in self._reserved:  # 不是cookie的保留字，则报错
        raise CookieError("Invalid attribute %r" % (K,))
    dict.__setitem__(self, K, V)
# 键值对赋值
def set(self, key, val, coded_val):
    if key.lower() in self._reserved:  # 必须是非保留字
        raise CookieError('Attempt to set a reserved key %r' % (key,))
    if not _is_legal_key(key):  # 非法字符
        raise CookieError('Illegal key %r' % (key,))

    # It's a good key, so save it.
    self._key = key
    self._value = val
    self._coded_value = coded_val
```
BaseCookie可以作为通用解析Cookie字符串的类，其它特殊的解析形式可以通过继承BaseCookie类，覆写方法进行修改。  
BaseCookie继承自dict，自身带的元素是cookie-key:cookie-value，而每个cookie-value都是Morsel类型，可以携带key-value之外，也携带对应的key-attribute（即该cookie的属性），下面这段代码体现了对cookie字符串的解析，用上述的正则表达式模式进行匹配，且必须先获得键值信息，后续才能记录该键值对的属性信息，否则属于非法cookie。
```python
    def __parse_string(self, str, patt=_CookiePattern):
        i = 0                 # Our starting point
        n = len(str)          # Length of string
        parsed_items = []     # Parsed (type, key, value) triples
        morsel_seen = False   # A key=value pair was previously encountered  已经找到了key=value这个对，必须先找到这个键值对，才能添加属性信息

        TYPE_ATTRIBUTE = 1
        TYPE_KEYVALUE = 2

        # We first parse the whole cookie string and reject it if it's
        # syntactically invalid (this helps avoid some classes of injection
        # attacks). 必须先找到key-value对，而且属性部分是带在这个key-value对后面的
        while 0 <= i < n:  # 总共识别的长度不可以大于实际的cookie字符串长度
            # Start looking for a cookie
            match = patt.match(str, i)
            if not match:
                # No more cookies
                break

            key, value = match.group("key"), match.group("val")
            i = match.end(0)

            if key[0] == "$":
                if not morsel_seen:
                    # We ignore attributes which pertain to the cookie
                    # mechanism as a whole, such as "$Version".  $开头的cookie键值对主要表示整体的cookie属性，不需要考虑其中
                    # See RFC 2965. (Does anyone care?)
                    continue
                parsed_items.append((TYPE_ATTRIBUTE, key[1:], value))
            elif key.lower() in Morsel._reserved:
                if not morsel_seen:
                    # Invalid cookie string
                    return
                if value is None:
                    if key.lower() in Morsel._flags:
                        parsed_items.append((TYPE_ATTRIBUTE, key, True))  # httponly 和 secure 属性
                    else:
                        # Invalid cookie string
                        return
                else:
                    parsed_items.append((TYPE_ATTRIBUTE, key, _unquote(value)))
            elif value is not None:
                parsed_items.append((TYPE_KEYVALUE, key, self.value_decode(value)))
                morsel_seen = True
            else:
                # Invalid cookie string
                return

        # The cookie string is valid, apply it.
        M = None         # current morsel
        for tp, key, value in parsed_items:
            if tp == TYPE_ATTRIBUTE:
                assert M is not None
                M[key] = value
            else:
                assert tp == TYPE_KEYVALUE  # TYPE_KEYVALUE是先找到的，然后才轮到TYPE_ATTRIBUTE
                rval, cval = value
                self.__set(key, rval, cval)
                M = self[key]
```

# http模块详解
```python
class HTTPServer(socketserver.TCPServer):

    allow_reuse_address = 1    # Seems to make sense in testing environment

    def server_bind(self):
        """Override server_bind to store the server name."""
        socketserver.TCPServer.server_bind(self)
        host, port = self.server_address[:2]
        self.server_name = socket.getfqdn(host)
        self.server_port = port
```
简单版的HttpServer继承自socketserver的TCPServer模块，这是一个单线程模式的HTTP服务器

```python
class ThreadingHTTPServer(socketserver.ThreadingMixIn, HTTPServer):
    daemon_threads = True
```
多线程版本也比较简单，在继承原始单线程的HTTP服务器的基础上加入socketserver.ThreadingMixIn来提供多线程支持。

这里比较重要的是如何处理HTTP请求，数据的接收是由socket或多线程socket实现的，这一块依赖于socketserver，也就是上面两个代码块实现的效果。而socketserver的运用主要是依赖于handler，即处理函数的实现。

这里继承了socketserver中定义的StreamRequestHandler处理方法