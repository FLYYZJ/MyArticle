# 什么是wordpress中的hook

wordpress 中主要有两种类型的hook，一种是filter hook，一种是action hook。其中filter hook主要作用是在WP执行和传递数据过程中，它可以实现在完成某一动作（如输出到浏览器或写入数据库等）之前对数据进行过滤（该过滤由自定义的过滤函数实现）再处理的目的。而action hook则是表示当WP执行到特定的位置时，执行某个动作（该动作由自定义的动作函数执行）

### Action Hook
WP核心（或主题、插件）在做它们该做的事时，如果执行到有埋 action hook 的代码（即是 do_action 语法） 时，会去找寻对应到的hook functions，进而执行这些 hook functions（即那些通过add_action() 加入的 hook functions），从而完成定制功能。WP核心并不需要Action Hook functions会有返回值（即函数不需要return），所以这里的hook function只被视为一个”独立切出来运作的功能“（就是自定义的一些函数）。

WP核心做它该做的事，你做你想做的事，做完就各自结束。

### Filter Hook
跟 Action Hook 一样，WP核心（或主题、插件）在做它们该做的事时，如果执行到有埋 filter hook 的代码（即是apply_filters语法）时，就会去找寻对应的 hook functions ，进而执行这些 hook functions（即那些通过add_filter() 加入的 hook functions ），从而完成定制功能。与 Action Hook 不同之处是，所有”鈎上“ Filter Hook 的 hook functions 通常都会接收到参数，而WP核心需要开发者自定义的函数可以拿到其提供的参数，并做完你想做的事后，要回传（return）一个值，让WP核心再利用你回传的值来接着完成它该做的事。

开发者通过自定义的函数修改了WP核心传入的参数，随后WP核心再接着拿你改过的参数，继续完成它该做的事，此动作就像”过滤“的动作，因而得名 filter（过滤器）。

一个较为形象地理解hook机制的图：
![](https://github.com/undersunshine/MyArticle/blob/master/WordPress/images/wpdaxue.com-201303508.png)

## action hook的几个方法


## filter hook的几个方法

add_filter( string $tag, callable $function_to_add, int $priority = 10, int $accepted_args = 1 ) // 将某一个函数加入对应的filter hook中

第一个参数表示要添加的filter hook的名字，而第二个参数是自定义的回调函数（匿名函数等），第三个参数表示的是优先级，数值越小，优先级越高，高优先级的回调函数会被更早地调用，同一优先级的回调函数会按加入顺序进行调用。注意，执行这个hook后这些挂在该hook上的函数都会被调用。

apply_filters( $tag, $value, $var... ); // 创建和执行filter hook时都会用到

创建或执行某个hook。









# 相关阅读
[filter hook 源码分析](http://www.ecdoer.com/post/wordpress-filter.html)

[action hook 源码分析](http://www.ecdoer.com/post/wordpress-action.html)

[filter hook的一些解析 1](https://blog.csdn.net/meegomeego/article/details/38536085)





