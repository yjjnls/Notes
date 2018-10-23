const puppeteer = require('puppeteer');
const stringRandom = require('string-random');

urls = ['https://www.jianshu.com/p/de155f6ef657', 'https://www.jianshu.com/p/04ef9509112f', 'https://www.jianshu.com/p/a56dac78b4ad', 'https://www.jianshu.com/p/890aab800ea6'];

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
                await page.goto(urls[stringRandom(1, '0123')]);
            }
            await page.waitFor(1000);
        }
    } catch (error) {
        console.log('error occured! restart!');
        await browser.close();
    }
})();

