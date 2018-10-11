const puppeteer = require('puppeteer');

repos = [
    'yjjnls/architect-awesome',
    'yjjnls/awesome',
    'yjjnls/awesome-bitcoin',
    'yjjnls/awesome-blockchain',
    'yjjnls/awesome-blockchain-cn',
    'yjjnls/awesome-blockchains',
    'yjjnls/awesome-ethereum',
    'yjjnls/awesome-hyperledger-fabric',
    'yjjnls/awesome-non-financial-blockchain',
    'yjjnls/awesome-smart-contracts',
    'yjjnls/awesome-solidity',
    'yjjnls/bitcoinbook',
    'yjjnls/blockchain',
    'yjjnls/blockchain_go_videos',
    'yjjnls/books',
    'yjjnls/brpc',
    'yjjnls/builder',
    'yjjnls/cam',
    'yjjnls/cerbero',
    'yjjnls/cerbero.old',
    'yjjnls/cerbero.ribbon',
    'yjjnls/cerbero.ribbon-1',
    'yjjnls/cerbero.woms',
    'yjjnls/cerberus',
    'yjjnls/cerberus.remix',
    'yjjnls/conan-gstreamer',
    'yjjnls/Cpp-CodeFormat',
    'yjjnls/cpp-ethereum',
    'yjjnls/D.S.A-Leet',
    'yjjnls/depot_tools',
    'yjjnls/easystarjs',
    'yjjnls/envoy',
    'yjjnls/eosio.token',
    'yjjnls/ethminer',
    'yjjnls/flatbuffers',
    'yjjnls/go-ethereum',
    'yjjnls/Go-tutorial-cn',
    'yjjnls/gst-snippets',
    'yjjnls/gstwebrtc-demos',
    'yjjnls/httpws',
    'yjjnls/illacceptanything',
    'yjjnls/istio',
    'yjjnls/LibEvent',
    'yjjnls/libgstrtspserver',
    'yjjnls/libgstwebrtc',
    'yjjnls/libwebrtc',
    'yjjnls/libwebstreamer',
    'yjjnls/libwebstreamer-prototype',
    'yjjnls/ML-From-Scratch',
    'yjjnls/NewWebassembly',
    'yjjnls/node-plugin',
    'yjjnls/nodejs-sdk',
    'yjjnls/Notes',
    'yjjnls/openwebrtc',
    'yjjnls/openwebrtc-lite',
    'yjjnls/parameter-framework',
    'yjjnls/PhoenixGo',
    'yjjnls/plugin.node',
    'yjjnls/plugin.node.old',
    'yjjnls/pouch',
    'yjjnls/Proto3Json',
    'yjjnls/protobuf',
    'yjjnls/pychecker',
    'yjjnls/pynapiemscripten',
    'yjjnls/RAS-MSG',
    'yjjnls/redis-in-action',
    'yjjnls/RTCMultiConnection',
    'yjjnls/solidity',
    'yjjnls/srs',
    'yjjnls/TensorFlow-Examples',
    'yjjnls/tessdata',
    'yjjnls/tesseract-node',
    'yjjnls/tesseract.conan',
    'yjjnls/tesseract.plugin',
    'yjjnls/The-Art-Of-Programming-By-July',
    'yjjnls/tutorial2',
    'yjjnls/webrtc',
    'yjjnls/websocket.io',
    'yjjnls/webstreamer',
    'yjjnls/webstreamer.github.io',
    'yjjnls/wiki',
    'yjjnls/ZeroMQ'];

(async function test() {
    const browser = await puppeteer.launch();
    try {
        const page = await browser.newPage();
        for (var i = 0; i < repos.length; i += 1) {
            console.log('https://vapi.yuekuai.tech/tests/github/star.php?action=put&repo=' + repos[i]);
            await page.goto('https://vapi.yuekuai.tech/tests/github/star.php?action=put&repo=' + repos[i]);
            console.log(repos[i]);
        }
        await browser.close();
        // await page.waitFor(1000);
    } catch (error) {
        console.log('error occured! restart!' + error.message);
        await browser.close();
    }
})();

