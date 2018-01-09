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