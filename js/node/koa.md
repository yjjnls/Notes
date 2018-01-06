# koa
koa的目的是用更清晰简洁的代码来实现你的逻辑，同时在错误处理上更加方便

[koajs](https://koa.bootcss.com/)


## 中间件
koa比起express的一个特点是：express让请求逐个通过中间件，实现处理网络请求，而koa加上了中间件的回溯，即先逐个通过中间件再反穿回来。  

下面以 “Hello World” 的响应作为示例，首先请求流通过 x-response-time 和 logging 中间件来请求何时开始，然后继续移交控制给 response 中间件。当一个中间件调用 next() 则该函数暂停并将控制传递给定义的下一个中间件。当在下游没有更多的中间件执行后，堆栈将展开并且每个中间件恢复执行其上游行为。  

```js
const Koa = require('koa');
const app = new Koa();

// x-response-time

app.use(async (ctx, next) => {
  const start = Date.now();
  await next();
  const ms = Date.now() - start;
  ctx.set('X-Response-Time', `${ms}ms`);
});

// logger

app.use(async (ctx, next) => {
  const start = Date.now();
  await next();
  const ms = Date.now() - start;
  console.log(`${ctx.method} ${ctx.url} - ${ms}`);
});

// response

app.use(async ctx => {
  ctx.body = 'Hello World';
});

app.listen(3000);
```


`app.use(function)` 将给定的中间件方法添加到此应用程序。
**koa相当于一个框架，而每一个中间件相当于一个特定功能的插件，可以是装载起来供其他中间件调用，比如特定的logger插件，处理网页视图的插件等；也可以是直接在里面就执行功能代码。**

### 常用middleware

[koa_middleawre](https://github.com/koajs/koa/wiki#middleware)

koa-views - Render your views with almost any templating engine
koa-static - static file serving middleware
koa-router - RESTful resource router

## 错误处理
```js
app.on('error', err => {
  log.error('server error', err)
});
app.on('error', (err, ctx) => {
  log.error('server error', err, ctx)
});
```


