const puppeteer = require('puppeteer');

(async () => {
    const browser = await puppeteer.launch();
    const page = await browser.newPage();
    await page.goto('https://www.jianshu.com/p/1ebbd6708e42');
    await browser.close();
})();
