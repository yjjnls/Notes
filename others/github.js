const puppeteer = require('puppeteer');
const stringRandom = require('string-random');

urls = ['https://github.com/join'];

(async function test() {
    // const browser = await puppeteer.launch();
    const browser = await puppeteer.launch({ headless: false, slowMo: 250 });
    try {
        const page = await browser.newPage();
        var num = stringRandom(1, '6789');
        var name = stringRandom(num, { numbers: false }).toLowerCase();
        await page.goto(urls[0]);
        await page.type('#user_login', name);
        await page.type('#user_email', `${name}@yopmail.com`);
        await page.type('#user_password', 'vurj3574;,fake');
        await page.click('#signup_button');
        await page.click('#js-pjax-container > div.container-lg.p-responsive.py-5 > div.d-md-flex.gutter-md-spacious > div.col-md-8 > div > form > div.SignUpContinueActions > button');
        await page.click('#js-pjax-container > div > div.user-identification-questions > div > form > input.btn.btn-primary');


    } catch (error) {
        console.log('error occured! restart!');
        await browser.close();
    }
    await browser.close();

})();

