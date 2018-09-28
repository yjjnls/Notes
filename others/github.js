const puppeteer = require('puppeteer');
const stringRandom = require('string-random');
var fs = require("fs");
var schedule = require('node-schedule');
urls = ['https://github.com/join'];

var rule = new schedule.RecurrenceRule();
rule.minute = [0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55];
var j = schedule.scheduleJob(rule, function () {
    console.log('现在时间：', new Date());

    (async function test() {
        // const browser = await puppeteer.launch();
        // const browser = await puppeteer.launch({ headless: false, slowMo: 250 });
        const browser = await puppeteer.launch({ slowMo: 250 });
        try {
            const page = await browser.newPage();
            // await page.waitFor(300000);
            var num = stringRandom(1, '6789');
            var name = stringRandom(num, { numbers: false }).toLowerCase();
            console.log('~~~~~~~~~~~1~~~~~~~~~');
            await page.goto(urls[0]);
            console.log('~~~~~~~~~~~2~~~~~~~~~');
            await page.type('#user_login', name);
            await page.type('#user_email', `${name}@yopmail.com`);
            await page.type('#user_password', 'vurj3574;,fake');
            console.log('~~~~~~~~~~~3~~~~~~~~~');
            await page.click('#signup_button');
            var i = 0;
            while (page.url() != 'https://github.com/join/plan') {
                i += 1;
                await page.waitFor(1000);
                if (i == 30)
                    throw new Error('join timeout');
            }

            // await page.waitForNavigation({ timeout: 30000 });
            console.log('~~~~~~~~~~~4~~~~~~~~~');
            // await page.waitFor(3000);
            await page.click('#js-pjax-container > div.container-lg.p-responsive.py-5 > div.d-md-flex.gutter-md-spacious > div.col-md-8 > div > form > div.SignUpContinueActions > button');

            console.log('~~~~~~~~~~~5~~~~~~~~~');
            await page.waitFor(5000);

            await page.click('#js-pjax-container > div > div.user-identification-questions > div > form > input.btn.btn-primary');

            console.log('~~~~~~~~~~~6~~~~~~~~~');
            fs.appendFile("github.txt", `${name}\n`, (error) => {
                if (error) return console.log("追加文件失败" + error.message);
                console.log(`追加成功 ${name}\n`);
            });
        } catch (error) {
            console.log('error occured! restart! ' + error.message);
            await browser.close();
        }
        await browser.close();

    })();

});

