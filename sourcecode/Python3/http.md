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




