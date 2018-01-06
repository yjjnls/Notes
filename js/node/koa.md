koa的目的是用更清晰简洁的代码来实现你的逻辑，同时在错误处理上更加方便

koa比起express另一特点是：express让请求逐个通过中间件，实现处理网络请求，而koa加上了中间件的回溯，即先逐个通过中间件再反穿回来。

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