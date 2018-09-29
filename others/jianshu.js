const puppeteer = require('puppeteer');
const stringRandom = require('string-random');

urls = ['https://www.jianshu.com/p/91c4178616fb', 'https://www.jianshu.com/p/a81194479b67', 'https://www.jianshu.com/p/19bbe6b7a73f', 'https://www.jianshu.com/p/ca569196dff6', 'https://www.jianshu.com/p/f82436325fc6', 'https://www.jianshu.com/p/5a89014c3345'];

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

