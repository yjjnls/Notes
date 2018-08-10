const puppeteer = require('puppeteer');

urls = ['https://blog.csdn.net/yjjnls/article/details/81283731', 'https://blog.csdn.net/yjjnls/article/details/81283755', 'https://blog.csdn.net/yjjnls/article/details/81283801', 'https://blog.csdn.net/yjjnls/article/details/81283840', 'https://blog.csdn.net/yjjnls/article/details/81297245', 'https://blog.csdn.net/yjjnls/article/details/81298831', 'https://blog.csdn.net/yjjnls/article/details/81321528', 'https://blog.csdn.net/yjjnls/article/details/81324434', 'https://blog.csdn.net/yjjnls/article/details/81324442', 'https://blog.csdn.net/yjjnls/article/details/81324451', 'https://blog.csdn.net/yjjnls/article/details/81324517', 'https://blog.csdn.net/yjjnls/article/details/81352176', 'https://blog.csdn.net/yjjnls/article/details/81380786', 'https://blog.csdn.net/yjjnls/article/details/81407120', 'https://blog.csdn.net/yjjnls/article/details/81435204', 'https://blog.csdn.net/yjjnls/article/details/81436884', 'https://blog.csdn.net/yjjnls/article/details/81474473'];

function sleep(numberMillis) {
    var now = new Date();
    var exitTime = now.getTime() + numberMillis;
    while (true) {
        now = new Date();
        if (now.getTime() > exitTime)
            return;
    }
}

pages = [];
(async function test() {
    const browser = await puppeteer.launch();
    // const browser = await puppeteer.launch({ headless: false, slowMo: 250});
    const page = await browser.newPage();
    await page.goto('https://passport.csdn.net/account/login');
    await page.waitFor(1000);
    await page.click('body > div.main > div > div > div:nth-child(2) > div > h3 > a');
    await page.waitFor(1000);
    await page.type('#username', '');
    await page.type('#password', '');
    await page.click('#fm1 > input.logging');
    await page.waitFor(1000);

    await page.click('body > div.main > div > div.register-info > div > ul > li > a:nth-child(2)');

    await page.waitFor(1000);

    for (var i = 0; i < urls.length; i += 1) {
        const new_page = await browser.newPage();
        pages.push(new_page);
        await pages[i].goto(urls[i]);
    }
    while (true) {
        for (var i = 0; i < urls.length; i += 1) {
            await pages[i].reload();
            // await page.waitFor(1000);
            // sleep(500);
        }
        sleep(10000);
    }
    await browser.close();
})();

