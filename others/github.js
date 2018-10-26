const puppeteer = require('puppeteer');
const stringRandom = require('string-random');
var fs = require("fs");
var schedule = require('node-schedule');
urls = ['https://github.com/join'];

var rule = new schedule.RecurrenceRule();
rule.minute = [0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55];
// rule.minute = [0, 10,  20, 30, 40, 50];
function sleep(numberMillis) {
    var now = new Date();
    var exitTime = now.getTime() + numberMillis;
    while (true) {
        now = new Date();
        if (now.getTime() > exitTime)
            return;
    }
}
var j = schedule.scheduleJob(rule, function () {
    sleep(stringRandom(1, '123456789') * 1324);
    myDate = new Date();
    console.log('现在时间：', myDate.toLocaleDateString() + ' ' + myDate.toLocaleTimeString());

    (async function test() {
        // const browser = await puppeteer.launch();
        const browser = await puppeteer.launch({ headless: false, slowMo: 250 });
        // const browser = await puppeteer.launch({ slowMo: 250 });
        try {
            const page = await browser.newPage();
            // await page.waitFor(300000);
            var num = stringRandom(1, '6789');
            var name = stringRandom(num, { numbers: false }).toLowerCase();
            console.log('~~~~~~~~~~~1~~~~~~~~~');
            await page.goto(urls[0], {
                waitUntil: 'networkidle2' // 网络空闲说明已加载完毕
            });
            console.log('~~~~~~~~~~~2~~~~~~~~~');
            await page.type('#user_login', name);
            await page.type('#user_email', `${name}@nospam.ze.tc`);
            await page.type('#user_password', 'vurj3574;,fake');
            console.log('~~~~~~~~~~~3~~~~~~~~~');
            await page.click('#signup_button');
            var i = 0;
            while (page.url() != 'https://github.com/join/plan') {
                i += 1;
                await page.waitFor(5000);
                // page.click('#signup_button');
                if (i == 20)
                    throw new Error('join timeout');
            }

            // await page.waitForNavigation({ timeout: 30000 });
            console.log('~~~~~~~~~~~4~~~~~~~~~');
            // await page.waitFor(3000);
            try {
                await page.click('#js-pjax-container > div.container-lg.p-responsive.py-5 > div.d-md-flex.gutter-md-spacious > div.col-md-8 > div > form > div.SignUpContinueActions > button');
            } catch (err) {
                await page.click('#js-pjax-container > div > form > div.SignUpContinueActions > button');
            }

            console.log('~~~~~~~~~~~5~~~~~~~~~');
            await page.waitFor(5000);

            await page.click('#js-pjax-container > div > div.user-identification-questions > div > form > input.btn.btn-primary');

            console.log('~~~~~~~~~~~6~~~~~~~~~');
            fs.appendFile("github.txt", `${name}\n`, (error) => {
                if (error) return console.log("追加文件失败" + error.message);
                console.log(`追加成功 ${name}\n`);
            });
        } catch (error) {
            if (error.message == 'No node found for selector: #js-pjax-container > div.container-lg.p-responsive.py-5 > div.d-md-flex.gutter-md-spacious > div.col-md-8 > div > form > div.SignUpContinueActions > button') {
                await page.click('#js-pjax-container > div > form > div.SignUpContinueActions > button');

                console.log('~~~~~~~~~~~5~~~~~~~~~');
                await page.waitFor(5000);

                await page.click('#js-pjax-container > div > div.user-identification-questions > div > form > input.btn.btn-primary');

                console.log('~~~~~~~~~~~6~~~~~~~~~');
                fs.appendFile("github.txt", `${name}\n`, (error) => {
                    if (error) return console.log("追加文件失败" + error.message);
                    console.log(`追加成功 ${name}\n`);
                });
                await browser.close();

            } else {
                console.log('error occured! restart! ' + error.message);
                await browser.close();
            }
        }
        await browser.close();

    })();

});

