# Asynchronization of node

## 难点

### 异常处理
**异步IO的实现主要分为两节阶段：提交请求和处理结果。这两个阶段中间是有其他时间循环的调度，两者彼此不关联。**  
异步方法通常在第一阶段提交请求后就会立即返回一个结果，但是真正的处理和结果返回在另一个阶段，这也是异常发生的地方。  
**try/catch只能捕获档次循环内的异常，当callback执行时抛出的异常无法捕捉。**

一般约定在回调函数callback中将第一个参数设err，如果err不为空，说明发生了异常。

### 函数嵌套过深

## 解决方案
### 事件发布/订阅模式
通过events模块中的信号来处理，提交异步处理时订阅相关信号，当处理完成时，处理模块发射相应信号。这样在之后的循环中，就会触发该信号的回调。


### promise

async 表示这是一个async函数，await只能用在这个函数里面

await 表示在这里等待promise返回结果了，再继续执行。

#### 获得返回值
await等待的虽然是promise对象，但不必写.then(..)，直接可以得到返回值。
```js
var sleep = function (time) {
    return new Promise(function (resolve, reject) {
        setTimeout(function () {
            // 返回 ‘ok’
            resolve('ok');
        }, time);
    })
};

var start = async function () {
    let result = await sleep(3000);
    console.log(result); // 收到 ‘ok’
};
```

#### 捕捉错误
既然.then(..)不用写了，那么.catch(..)也不用写，可以直接用标准的try catch语法捕捉错误。
```js
var sleep = function (time) {
    return new Promise(function (resolve, reject) {
        setTimeout(function () {
            // 模拟出错了，返回 ‘error’
            reject('error');
        }, time);
    })
};

var start = async function () {
    try {
        console.log('start');
        await sleep(3000); // 这里得到了一个返回错误
        
        // 所以以下代码不会被执行了
        console.log('end');
    } catch (err) {
        console.log(err); // 这里捕捉到错误 `error`
    }
};
```

https://www.cnblogs.com/YikaJ/p/4996174.html