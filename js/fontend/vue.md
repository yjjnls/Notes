#vue
## install
npm install vue -g  
npm install vue-cli -g  
npm install webpack -g  
npm install webpack-dev-server -g  

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


`/build`用于打包代码 npm run build  
`/config`相关环境的配置，主要关注index.js，里面有关于开发dev和打包build的配置

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