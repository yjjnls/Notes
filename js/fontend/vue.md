<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

-   [vue](#vue)
    -   [install](#install)
        -   [quick start](#quick-start)
            -   [toc](#toc)
    -   [components](#components)


# vue

## install

npm install vue -g  
npm install vue-cli -g  

### quick start

创建一个基于 webpack 模板的新项目  
vue init webpack my-project

```bash
To get started:

  cd my-project
  npm run dev

Documentation can be found at https://vuejs-templates.github.io/webpack
```

错误
卸载npm uninstall webpack-dev-server，在安装这个 npm i webpack-dev-server@2.9.7

### toc

* * *

`/build`  用于打包代码 npm run build  

* * *

`/config`  相关环境的配置，**主要关注index.js**，里面有关于开发dev和打包build的配置

build：

```js
    // Template for index.html
    index: path.resolve(__dirname, '../dist/index.html'),

    // Paths
    assetsRoot: path.resolve(__dirname, '../dist'),
    assetsSubDirectory: 'static',
    assetsPublicPath: '/',
```

打包后最终提取dist目录下的文件  

dev是开发调试网页的一些配置，包括端口，代理等，调试时需要把后端服务开启

```js
proxyTable: {
        '/webmedia/': {
            target: 'http://localhost:3002'
        }
    },
```
实际运行时，应该是网页直接把请求发到后端，而调试时，是先将请求发送到dev-server，例如/webmedia/xxx，而dev-server会将其转发到http://localhost:3002。这样后端收到的请求也是/webmedia/xxx。   

如果需要跨域那么需要加上参数changeOrigin:true

* * *

`src`   **主要开发目录**

-   \\assets: 放置一些图片，如logo等。（感觉用处不大）
-   `\components`: 目录里面放了一个组件文件，可以不用（指默认生成的）。
-   `\router`: 里面默认会生成一个index.js，表示界面路由。indexjs包含一个router对象，表示http访问路径对应需要渲染的组件
      比如下面的路由中，访问/create目录时，需要渲染三个组件，主要是CreateMeetingContent这个组件（在\\components目录中）。

```js
export default new Router({
  routes: [
    {
        path: '/',
        redirect: '/create'
    },
    {
      path: '/create',
      name: 'Main',
      components: {
          default: Main,
          content: CreateMeetingContent,
          footer: PageFooter
      }
    },
}
```

-   App.vue: 项目入口文件，我们也可以直接将组件写这里，而不使用 components 目录。（感觉用处不大）
-   main.js: 项目的核心文件。（感觉用处不大）

页面组件可以放在components中，也可以放在其他的文件夹里。src中还可以根据需要添加用于实现其他功能的文件夹。

* * *

`\static`   静态资源目录，如图片、字体等  

* * *

`index.html`   首页入口文件  


* * *

## components
每个vue文件代表网页上的一个组件，vue文件的结构分为 template, script, style 三个部分。