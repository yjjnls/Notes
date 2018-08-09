const puppeteer = require('puppeteer');

urls = ['https://www.jianshu.com/p/1ebbd6708e42', 'https://www.jianshu.com/p/d16a6f20c1e1', 'https://www.jianshu.com/p/793f74646026', 'https://www.jianshu.com/p/f34dfb9781cb', 'https://www.jianshu.com/p/cdb14a3344ba', 'https://www.jianshu.com/p/e658d59d79e7', 'https://www.jianshu.com/p/381b3115f92e', 'https://www.jianshu.com/p/45cf14869b93', 'https://www.jianshu.com/p/b9bf34299c2e', 'https://www.jianshu.com/p/a9407bc9c4e0', 'https://www.jianshu.com/p/b6c6438744bf', 'https://www.jianshu.com/p/d249fcdf4114', 'https://www.jianshu.com/p/c5727e749ba6', 'https://www.jianshu.com/p/a7241c3ca2ff', 'https://www.jianshu.com/p/81ccaf987dc0', 'https://www.jianshu.com/p/f733881dc13b', 'https://www.jianshu.com/p/3aa571d8f8c3', 'https://www.jianshu.com/p/4d891e16f087'];


async function test() {
    const browser = await puppeteer.launch();
    const page = await browser.newPage();
    for (var i = 0; i < urls.length; i += 1)
        await page.goto(urls[i]);
    await browser.close();
}

setInterval(test, 3000);
