const puppeteer = require('puppeteer');
const stringRandom = require('string-random');

urls = ['https://www.jianshu.com/p/e00f7a19a589', 'https://www.jianshu.com/p/5cbf6fbaec79', 'https://www.jianshu.com/p/92c1f60e3963', 'https://www.jianshu.com/p/fc271206bb7b','https://www.jianshu.com/p/f079a90f2148','https://www.jianshu.com/p/0f339580deab'];

(async function test() {
    const browser = await puppeteer.launch();
    try {
        const page = await browser.newPage();
        while (true) {
            for (var k = 0; k < 100; k++) {
                // for (var i = 0; i < urls.length; i += 1) {
                //     await page.goto(urls[i]);
                //     // sleep(500);
                // }
                await page.goto(urls[stringRandom(1, '012345')]);
            }
            await page.waitFor(1000);
        }
    } catch (error) {
        console.log('error occured! restart!');
        await browser.close();
    }
})();

