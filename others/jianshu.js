const puppeteer = require('puppeteer');

urls = ['https://www.jianshu.com/p/70f6a29a6296'];

(async function test() {
    const browser = await puppeteer.launch();
    try {
        const page = await browser.newPage();
        while (true) {
            for (var k = 0; k < 100; k++) {
                for (var i = 0; i < urls.length; i += 1) {
                    await page.goto(urls[i]);
                    // sleep(500);
                }
            }
            await page.waitFor(1000);
        }
    } catch (error) {
        console.log('error occured! restart!');
        await browser.close();
    }
})();

