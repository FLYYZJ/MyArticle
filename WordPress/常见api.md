插件相关：
register_activation_hook函数：
    寄存一个插件函数，该插件函数在插件被激活时运行。使用方法为<?php register_activation_hook($file, $function); ?>，其中$file为插件所在的目录（一般可以用__FILE__ 来代替，function表示启用该插件时调用的回调函数）

register_deactivation_hook函数 
    寄存一个插件函数，该插件函数在插件被禁用时运行。 <?php register_deactivation_hook($file, $function); ?>，其中$file为插件所在的目录（一般可以用__FILE__ 来代替，function表示启用该插件时调用的回调函数）

    
    
    
