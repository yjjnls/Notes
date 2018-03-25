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


### 异步模式（async/await）

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

Promise.then
Promise.catch
Promise.resolve
Promise.reject
Promise.all
Promise.race

https://promisesaplus.com/

## 发展历史
### callback
```js
mongoDb.open(function(err, db){
    if(!err){
        db.collection("users", function(err, collection){
            if(!err){
                let person = {name: "yika", age: 20};
                collection.insert(person, function(err, result){
                    if(!err){
                        console.log(result);
                    }
                });
            }
        })
    }
});
```
### promise
```js
let person = {name: "yika"};
mongoDb
    .open()
    .then(function(database){
      return database.collection("users");
    })
    .then(function(collection){
      return collection.insert(person);
    })
    .then(function(result){
      console.log(result);
    })
    .catch(function(e){
      throw new Error(e);
    })
```
promise是一个异步编程的抽象，它是一个返回值或抛出exception的代理对象  
* promise只有三种状态，未完成，完成(fulfilled)和失败(rejected)。  
* promise的状态可以由未完成转换成完成，或者未完成转换成失败。  
* promise的状态转换只发生一次  
promise有一个then方法，`then方法可以接受3个函数作为参数`。**前两个函数对应promise的两种状态fulfilled, rejected的回调函数。第三个函数用于处理进度信息。**

```js
var promise = readFile();
promise.then(console.log, console.error);
```
**`.then()总是返回一个新的promise.`**   
例如第一个例子中，mongoDb.open()可以看做一个promise，打开数据库这个异步操作成功后，调用第一个then，执行下一个异步操作`database.collection("users")`，然后这个then返回一个新的promise对象，该对象就表示执行`database.collection("users")`这个异步操作。

reject 是拒绝，跳转到catch error
resolve 是解决，下一步，即跳转到下一个promise操作

如果异步行为没有返回promise对象，那么就要自己再包一层，例如
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


### async/await
```js
async function insertData(person){
    let db, collection, result; 
    try{
        db = await mongoDb.open();
        collection = await db.collection("users");
        result = await collection.insert(person);
    }catch(e){
        console.error(e.message);
    }
    console.log(result);
} 

insertData({name: "yika"});
```