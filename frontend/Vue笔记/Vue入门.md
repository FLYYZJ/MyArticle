## 基础
#### Vue实例  
```html
<div id = root>
    {{ msg }}
</div>
<script>
    new Vue({  // Vue实例
        el : "#root", // 绑定了id=root的标签
        data:{
            msg: "hello world"  
        }
    })
</script>
```

#### 挂载点 and 模板
```html
<div id = root>  <!--  vue实例的挂载点  -->
    <h1> hello {{msg}} </h1>  <!-- 挂载点中的内容都是模板内容 -->
</div>
<script>
    new Vue({  // Vue实例
        el : "#root", // 绑定了id=root的标签，绑定到挂载点
        data:{
            msg: "hello world"  
        }
    })
</script>
```
等价于
```html
<div id = root>  <!--  vue实例的挂载点  -->
    
</div>
<script>
    new Vue({  // Vue实例
        el : "#root", // 绑定了id=root的标签，绑定到挂载点
        template:"<h1> hello {{msg}} </h1>  <!-- 挂载点中的内容都是模板内容 -->",
        data:{
            msg: "hello world"  
        }
    })
</script>
```

#### v-html 和 v-text
插值表达式 {{}} （两个花括号中插入数值变量）

v-text 和 v-html：都是插入文本内容，但是v-html会解析html文本
```html
<div class="root" v-html="content"></div>  <!-- 不转义html -->
<div class="root" v-text="content"></div>  <!-- 转义html -->

<script>
    new Vue({  // Vue实例
        el : ".root", // 绑定了id=root的标签，绑定到挂载点
        template:"<h1> hello {{msg}} </h1>  <!-- 挂载点中的内容都是模板内容 -->",
        data:{
            content: "hello world"  
        }
    })
</script>
```

#### 事件绑定 和 事件修饰符
```html
<div class="root" v-on:click="handleClick"></div> 
<!-- 等价于 -->
<div class="root" @:click="handleClick"></div> 
<!-- 简例1 -->
<script>
    new Vue({  // Vue实例
        el : ".root", // 绑定了id=root的标签，绑定到挂载点
        template:"<h1> hello {{msg}} </h1>  <!-- 挂载点中的内容都是模板内容 -->",
        data:{
            content: "hello world"  
        }
        methods: {  // 绑定的事件
            handleClick: function(){
                this.content = "hello w";  // 点击后将root元素的值改为 hello w
            }
        }
    })
</script>

<!-- 简例2 -->
<script>
    new Vue({  // Vue实例
        el : ".root", // 绑定了id=root的标签，绑定到挂载点
        template:"<h1> hello {{msg}} </h1>  <!-- 挂载点中的内容都是模板内容 -->",
        data:{
            content: "hello world"  
        }
        methods: {  // 绑定的事件
            handleClick: function(){
                alert(12341234);
            }
        }
    })
</script>
```
事件修饰符  
功能类似于用于阻止冒泡事件等。  
```html
<!-- 阻止单击事件继续传播 -->
<a v-on:click.stop="doThis"></a>

<!-- 提交事件不再重载页面 -->
<form v-on:submit.prevent="onSubmit"></form>

<!-- 修饰符可以串联 -->
<a v-on:click.stop.prevent="doThat"></a>

<!-- 只有修饰符 -->
<form v-on:submit.prevent></form>

<!-- 添加事件监听器时使用事件捕获模式 -->
<!-- 即内部元素触发的事件先在此处理，然后才交由内部元素进行处理 -->
<div v-on:click.capture="doThis">...</div>

<!-- 只当在 event.target 是当前元素自身时触发处理函数 -->
<!-- 即事件不是从内部元素触发的 -->
<div v-on:click.self="doThat">...</div>

<!-- 点击事件将只会触发一次 -->
<a v-on:click.once="doThis"></a>

<!-- 滚动事件的默认行为 (即滚动行为) 将会立即触发 -->
<!-- 而不会等待 `onScroll` 完成  -->
<!-- 这其中包含 `event.preventDefault()` 的情况 -->
<div v-on:scroll.passive="onScroll">...</div>
```
其它的事件修饰符（包含键盘事件和键盘修饰符等）： https://cn.vuejs.org/v2/guide/events.html#%E4%BA%8B%E4%BB%B6%E4%BF%AE%E9%A5%B0%E7%AC%A6

#### 属性绑定和双向数据绑定
属性绑定
```html
<div class="root" v-html="content">
    <div v-bind:title = "title"></div>
    <!--等价于 -->
    <div :title = "title"></div>
</div>
<script>
    new Vue({  // Vue实例
        el : ".root", // 绑定了id=root的标签，绑定到挂载点
        template:"<h1> hello {{msg}} </h1>  <!-- 挂载点中的内容都是模板内容 -->",
        data:{
            content: "hello world" ,
            title: "this is hello world"
        }
    })
</script>
```
双向数据绑定
```html
<div class="root" v-html="content">
    <input v-model="value" />  <!-- 该值发生变化时，下面的div的值也会发生变化， 实现双向数据绑定 -->
    <div>{{value}}</div>
</div>
<script>
    new Vue({  // Vue实例
        el : ".root", // 绑定了id=root的标签，绑定到挂载点
        template:"<h1> hello {{msg}} </h1>  <!-- 挂载点中的内容都是模板内容 -->",
        data:{
            content: "hello world" ,
            title: "this is hello world",
            value: "this is value"
        }
    })
</script>
```

#### 计算属性和侦听器
```html
<div class="root" v-html="content">
    <input v-model="firstName" />  <!-- 该值发生变化时，下面的div的值也会发生变化， 实现双向数据绑定 -->
    <input v-model="lastName" />  <!-- 该值发生变化时，下面的div的值也会发生变化， 实现双向数据绑定 -->
    <div>{{fullName}}</div>
    <div></div>
</div>
<script>
    new Vue({  // Vue实例
        el : ".root", // 绑定了id=root的标签，绑定到挂载点
        template:"<h1> hello {{msg}} </h1>  <!-- 挂载点中的内容都是模板内容 -->",
        data:{
            firstName: "",
            lastName: "",
            count: 0
        },
        computed : {
            fullName: function(){  // 计算属性，一个属性由其它属性计算得来
                this.firstName +" " + this.lastName  
            }
        },
        watch: {  // 当vue实例中的某个属性发生变化时count发生变化
            firstName: function(){
                this.count++;
            },
            lastName: function(){
                this.count++;
            },
            fullName: function(){
                this.count += 2;
            }
        }
    })
</script>
```
#### v-if, v-show, v-for, 
```html
<div class="root" >
    <div v-if="show">hello world</div>  <!-- 当show为true时，hello world 才显示 -->
    <button @click="handleClick">toggle</button>
</div>
<script>
    new Vue({  // Vue实例
        el : ".root", // 绑定了id=root的标签，绑定到挂载点
        methods:{
            handleClick: function{
                this.show = !this.show;
            }
        }
        data: {
            show:true
        }
    })
</script>
```
使用v-show只是为标签添加一个 display=none的属性，而v-if判断完成后会将整个dom删除。v-show性能会较高，频繁更迭情况下。
```html
<div class="root" >
    <div v-show="show">hello world</div>  <!-- 当show为true时，hello world 才显示 -->
    <button @click="handleClick">toggle</button>
    <ul>
        <li v-for="(item, index) of list" : key="index">{{item}} </li> <!-- key可以提高性能，key必须不同 -->
    </ul>
</div>
<script>
    new Vue({  // Vue实例
        el : ".root", // 绑定了id=root的标签，绑定到挂载点
        methods:{
            handleClick: function{
                this.show = !this.show;
            }
        }
        data: {
            show:true, 
            list: [1,2,3,4,5]
        }
    })
</script>
```


#### 动态绑定CSS样式
```html
<!-- 指定了该div的class为classname1和classname2， -->
<div v-bind:class="{classname1:true, classname2:true}"></div>
```
示例
```html
<style>
    span {
        background: red;
        display: inline-block;
        padding: 10px;
        color: #fff;
        margin: 10px 0;
    }
    .changeColor span{
        background: green;
    }
    .changeLength span:after{
        content:"length";
        margin-left: 10px;
    }
</style>
...
<div id = "vue-app">
    <!-- 按钮点击后会修改vue实例中changeColor的值，从而修改了下面div的背景颜色 -->
    <button v-on:click="changeColor = !changeColor"></button>
     <!-- compClasses计算属性 -->
    <div v-bind:class="compClasses">
        <span>Hello vue</span>
    </div>
</div>

<script>
    new Vue({
        el:"#vue-app",
        data:{
            changeColor:false,
            changeLength:false
        },
        methods:{

        },
        computed:{
            compClasses: function{
                return {
                    changeColor: this.changeColor,
                    changeLength: this.changeLength
                }
            }
        }
    })
</script>

```

## 组件开发， 路由等高级知识

#### 组件开发
组件可以看作是一种模板，例如网页的结构一般分为页首，内容，侧边栏，页尾等，可以将这几个部分抽象成组件，以便后续重复利用。
组件语法：
```html
<script>
    // 定义一个名为 button-counter 的新组件，此时该组件会转变为一个标签供使用
    Vue.component('button-counter', {
    data: function () {  // data 必须是一个函数，而不是对象
        return {
        count: 0
        }
    },
    template: '<button v-on:click="count++">You clicked me {{ count }} times.</button>'
    })
</script>
...
<div id="components-demo">
    <!-- 利用组件名得到加载组件 -->
  <button-counter></button-counter>  
</div>
```
在脚手架工程中使用组件，可以在单独的vue文件中定义组件，组件一般由3部分构成—— template，script和css，一个示例的vue组件如下：
```html
<!-- 模板，该组件的显示结构 -->
<template>
    <footer>
        <p>{{ copyright }}</p>
    </footer>
</template>
<!-- 行为，注意data必须是函数形式 -->
<script>
export default {
    data(){
        return{
            copyright: 'Copyright 2017 Vue Ninjas'
        }
    }
}
</script>
<!-- 样式部分，注意加入scoped表示该css只作用于该组件 -->
<style scoped>
footer{
    background: #222;
    padding: 6px;
}
p{
        color: lightgreen;
        text-align: center;
}
</style>

```
而在主组件（一般是App.vue）中调用子组件，一般的方式如下：
```html
<template>
    <div>
        <app-header></app-header>
        <app-ninjas></app-ninjas>
        <app-footer></app-footer>
    </div>
</template>

<script>
// Imports 引入了组件
import Header from './components/Header.vue';
import Footer from './components/Footer.vue';
import Ninjas from './components/Ninjas.vue';

export default {
    components: {
        'app-header': Header,
        'app-footer': Footer,
        'app-ninjas': Ninjas
    },
    data () {
        return {

        }
    }
}
</script>

<style>
body{
    margin: 0;
    font-family: 'Nunito SemiBold';
}
</style>

```
而入口的main.js文件会这样调用App:
```html
import Vue from 'vue'
import App from './App.vue'

new Vue({
  el: '#app',
  render: h => h(App)  // 渲染主App.vue
})
```

#### 组件传值——子传父 和 父传子
属性传值（子传父）主要使用了Props方法，具体的应用示例如下
```html
<template>
    <!-- id指定了组件名 -->
    <div id="ninjas">
        <ul>
            <li v-for="ninja in ninjas" v-on:click="ninja.show = !ninja.show">
                <h2>{{ ninja.name }}</h2>
                <h3 v-show="ninja.show">{{ ninja.speciality }}</h3>
            </li>
        </ul>
        <button v-on:click="deleteNinja">Delete a Ninja</button>
    </div>
</template>
<script>
export default {
    props: {  // 该对象会接收到父组件传来的数据
      ninjas: {  // 父组件传来的数据名，必须对应
        type: Array,  // 表示该数据是array
        required: true 
      }
    },
    data(){
        return{

        }
    },
    methods: {
      deleteNinja: function(){
        this.ninjas.pop();
      }
    }
}
</script>
<style scoped>
#ninjas{
    width: 100%;
    max-width: 1200px;
    margin: 40px auto;
    padding: 0 20px;
    box-sizing: border-box;
}
ul{
    display: flex;
    flex-wrap: wrap;
    list-style-type: none;
    padding: 0;
}
li{
    flex-grow: 1;
    flex-basis: 300px;
    text-align: center;
    padding: 30px;
    border: 1px solid #222;
    margin: 10px;
}
</style>

```
而在父组件中，需要
```html
<template>
    <div>
        <app-header v-bind:title="title"></app-header>
        <app-ninjas v-bind:ninjas="ninjas"></app-ninjas>
        <ul>
          <li v-for="ninja in ninjas">{{ ninja.name }}</li>
        </ul>
        <app-footer v-bind:title="title"></app-footer>
    </div>
</template>

<script>
// Imports
import Header from './components/Header.vue';
import Footer from './components/Footer.vue';
import Ninjas from './components/Ninjas.vue';

export default {
    components: {
        'app-header': Header,
        'app-footer': Footer,
        'app-ninjas': Ninjas
    },
    data () {
        return {
          ninjas: [  // 此时该数据会被子组件ninjas监听到
              {name: 'Ryu', speciality: 'Vue Components', show: false},
              {name: 'Crystal', speciality: 'HTML Wizardry', show: false},
              {name: 'Hitoshi', speciality: 'Click Events', show: false},
              {name: 'Tango', speciality: 'Conditionals', show: false},
              {name: 'Kami', speciality: 'Webpack', show: false},
              {name: 'Yoshi', speciality: 'Data Diggin', show: false}
          ],
          title: 'Vue Wizards'
        }
    }
}
</script>

<style>
body{
    margin: 0;
    font-family: 'Nunito SemiBold';
}
</style>

```
传值和传引用和C等语言的含义是一样的，一般传递数组和对象都是引用，此时在子组件中修改这些引用中的数据也会影响到父组件或其它调用该对象/数组的元素内容，而传值则不会。

#### 生命周期
![](/assets/vue实例生命周期.png)  
生命周期的含义在图中显示很清楚，一般在mounted之后页面才开始显示内容。调用这些Vue预留的方法也比较简单，直接在VUE实例中使用对应名字的属性即可，属性内容为函数，如下：  
```html
<template>
    <footer>
        <p>Copyright 2017 {{ title }}</p>
    </footer>
</template>
<script>
// imports
import { bus } from '../main';

export default {
    props: {
      title: {
        type: String,
        required: true
      }
    },
    data(){
        return{

        }
    },
    created(){  // 类似于methods那样的调用方式，作为函数存在
        bus.$on('titleChanged', (data) => {
            this.title = data;
        });
    }
}
</script>
<style scoped>
footer{
    background: #222;
    padding: 6px;
}
p{
        color: lightgreen;
        text-align: center;
}
</style>

```

#### 路由和请求
路由使用了vue-route模块，而请求使用vue-resource模块。入口main.js文件中要全局注册路由模块和resource模块，大致如下：
```javascript
// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
// 引入这两个模块
import VueRouter from 'vue-router'
import VueResource from 'vue-resource'
import App from './App'
import HelloWorld from './components/HelloWorld'
import Home from './components/Home'
// import Users from './components/Users'

Vue.config.productionTip = false
// 使用这两个模块（全局可用）
Vue.use(VueRouter)
Vue.use(VueResource)

// 配置路由
const router = new VueRouter({
	routes:[
		{path:"/",component:Home},
		{path:"/helloworld",component:HelloWorld}
	],
	mode:"history"
})

// 全局注册组件
// Vue.component("users",Users);

/* eslint-disable no-new */
new Vue({
  router,
  el: '#app',
  template: '<App/>',
  components: { App }
})

// index.html -> main.js -> App.vue

```


主vue（App.vue）使用路由的方式如下，一般我们要引入对应的组件，然后使用router-link来显示这些超链接，不用a标签，因为a标签会刷新内容，影响效率。     
```html
<!-- 1模板: html结构 -->
<template>
  <div id="app">
    <ul>
        <!-- 注意要用router-link标签 -->
      <li><router-link to="/">Home</router-link></li>
      <li><router-link to="/helloworld">Hello</router-link></li>
    </ul>
    <router-view></router-view>
  </div>
</template>

<!-- 2行为:处理逻辑 -->
<script>

export default {
  name: 'app',
  data(){
    return {
      
    }
  }
}
</script>

<!-- 3样式:解决样式 -->
<style scoped>
h1{
  color:purple;
}
</style>
```
其余的跳转到的组件和普通组件的写法一直，注意跳转的组件名要和router-link中显示的组件名是一致的。

数据获取，vue-resource的使用大致如下，就是this.$http.get方法去访问对应的链接获取数据，然后将这些数据放入到
```html
<!-- 1模板: html结构 -->
<template>
  <div id="home">
    <app-header v-on:titleChanged="updateTitle($event)" v-bind:title="title"></app-header>
    <users v-bind:users="users"></users>
    <users v-bind:users="users"></users>
    <app-footer v-bind:title="title"></app-footer>
  </div>
</template>

<!-- 2行为:处理逻辑 -->
<script>
// 局部注册组件
import Users from './Users'
import Header from './Header'
import Footer from './Footer'

export default {
  name: 'home',
  data(){
    return {
      // title:"这是我的第一个Vue脚手架项目!"
      users:[
        // {name:"Henry",position:"Web开发",show:false},
        // {name:"Henry",position:"Web开发",show:false},
        // {name:"Henry",position:"Web开发",show:false},
        // {name:"Henry",position:"Web开发",show:false},
        // {name:"Henry",position:"Web开发",show:false},
        // {name:"Henry",position:"Web开发",show:false},
        // {name:"Henry",position:"Web开发",show:false},
        // {name:"Henry",position:"Web开发",show:false},
        // {name:"Henry",position:"Web开发",show:false}
      ],
      title:"传递的是一个值,(number string boolean)"
    }
  },
  methods:{
    updateTitle(title){
      this.title = title;
    }
  },
  components:{
    "users":Users,
    "app-header":Header,
    "app-footer":Footer
  },
  created(){
    this.$http.get("http://jsonplaceholder.typicode.com/users")
        .then((data) => {
          // console.log(data);
          this.users = data.body;
        })
  }
}
</script>

<!-- 3样式:解决样式 -->
<style scoped>
h1{
  color:purple;
}
</style>

```

## axios库 —— 用于ajax和后台进行交互
安装方式  
cnpm install --save --save-exact axios vue-axios  
使用方式  
```javascript
import axios from "axios"
import VueAxios from "vue-axios"
Vue.use(VueAxios, axios)
```

GET请求
```javascript
axios.get('/user?ID=123')
    .then( res => {
         // 请求数据成功并返回数据。
        console.info(res)
    }).catch( e => {
        if(e.response){
        //请求已发出，服务器返回状态码不是2xx。
            console.info(e.response.data)
            console.info(e.response.status)
            console.info(e.response.headers)
        }else if(e.request){
            // 请求已发出，但没有收到响应
            // e.request 在浏览器里是一个XMLHttpRequest实例，
            // 在node中是一个http.ClientRequest实例
            console.info(e.request)
        }else{
            //发送请求时异常，捕捉到错误
            console.info('error',e.message)
        }
        console.info(e.config)
    })
// 等同以下写法
axios({
    url: '/user',
    method: 'GET',
    params: {
        ID: 123
    }
}).then( res => {
    console.info(res)
}).catch( e => {
    console.info(e)
})
```

POST请求
```javascript
axios.post('/user', {
    firstName: 'Mike',
    lastName: 'Allen'
}).then( res => {
    console.info(res)
}).catch( e => {
    console.info(e)
})
// 等同以下写法
axios({
    url: '/user',
    method: 'POST',
    data: {
        firstName: 'Mike',
        lastName: 'Allen'
    }
}).then( res => {
    console.info(res)
}).catch( e => {
   console.info(e)
})
```
官方示例：https://cn.vuejs.org/v2/cookbook/using-axios-to-consume-apis.html

#### 跨域问题