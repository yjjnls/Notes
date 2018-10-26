const puppeteer = require('puppeteer');

user = 'gtxbolcjc';
(async function test() {
    const browser = await puppeteer.launch({ headless: false });
    // const browser = await puppeteer.launch({ headless: false, slowMo: 250 });
    const page1 = await browser.newPage();

    await page1.goto('https://github.com/login', {
        waitUntil: 'networkidle2' // 网络空闲说明已加载完毕
    });

    await page1.type('#login_field', user);
    await page1.type('#password', 'vurj3574;,fake');
    await page1.click('#login > form > div.auth-form-body.mt-3 > input.btn.btn-primary.btn-block');
    // await page1.waitFor(1000);
    // await page1.click('#user-links > li:nth-child(3) > details > summary > img');
    // await page1.click('#user-links > li:nth-child(3) > details > details-menu > ul > li:nth-child(9) > a');

    // await page1.click('#js-pjax-container > div > div.col-3.float-left.pr-4 > nav:nth-child(1) > a:nth-child(4)');
    // while (page1.url() != 'https://github.com/settings/emails') {
    //     await page1.waitFor(1000);
    // }
    // await page1.waitFor(1000);

    // await page1.type('#email', `${user}@nospam.ze.tc`);
    // await page1.click('#new_user_email > dl > dd > button');


    const page2 = await browser.newPage();

    await page2.goto('http://www.yopmail.com/zh/', {
        waitUntil: 'networkidle2' // 网络空闲说明已加载完毕
    });
    await page2.type('#login', user);
    await page2.click('#f > table > tbody > tr:nth-child(1) > td:nth-child(3) > input');
    await page2.waitFor(3000);

    // await page2.click('#m2 > div > a > span.lms');
    // await page2.waitForSelector('#mailmillieu');
    
    await page2.click('#charset');
    await page2.click('#mailmillieu');
    await page2.click('#mailmillieu > div:nth-child(2) > div > a');



})()