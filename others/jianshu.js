const puppeteer = require('puppeteer');

urls = ['https://www.jianshu.com/p/1ebbd6708e42', 'https://www.jianshu.com/p/d16a6f20c1e1', 'https://www.jianshu.com/p/793f74646026', 'https://www.jianshu.com/p/f34dfb9781cb', 'https://www.jianshu.com/p/cdb14a3344ba', 'https://www.jianshu.com/p/e658d59d79e7', 'https://www.jianshu.com/p/381b3115f92e', 'https://www.jianshu.com/p/45cf14869b93', 'https://www.jianshu.com/p/b9bf34299c2e', 'https://www.jianshu.com/p/a9407bc9c4e0', 'https://www.jianshu.com/p/b6c6438744bf', 'https://www.jianshu.com/p/d249fcdf4114', 'https://www.jianshu.com/p/c5727e749ba6', 'https://www.jianshu.com/p/a7241c3ca2ff', 'https://www.jianshu.com/p/81ccaf987dc0', 'https://www.jianshu.com/p/f733881dc13b', 'https://www.jianshu.com/p/3aa571d8f8c3', 'https://www.jianshu.com/p/4d891e16f087', 'https://www.jianshu.com/p/efc3235ceb0d', 'https://www.jianshu.com/p/2dbf0ec017b2', 'https://www.jianshu.com/p/6c90f0513084', 'https://www.jianshu.com/p/d06f0ee82d7f', 'https://www.jianshu.com/p/c8ec1fc4cfdb', 'https://www.jianshu.com/p/0c8372728ecf', 'https://www.jianshu.com/p/672e0a5426fa', 'https://www.jianshu.com/p/496ef6a5685c'];

(async function test() {
    const browser = await puppeteer.launch();
    try {
        const page = await browser.newPage();
        while (true) {
            for (var i = 0; i < urls.length; i += 1) {
                await page.goto(urls[i]);
                // sleep(500);
            }
            await page.waitFor(1000);
        }
    } catch (error) {
        console.log('error occured! restart!');
        await browser.close();
    }
})();

