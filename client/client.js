var net = require('net');
var fs = require("fs");
var readline = require('readline');

var HOST = '127.0.0.1';
var PORT = 7474;

//创建readline接口实例
var rl = readline.createInterface({
    input:process.stdin,
    output:process.stdout
});

var client = new net.Socket();
client.connect(PORT, HOST, function() {

    console.log('CONNECTED TO: ' + HOST + ':' + PORT);
    // 建立连接后立即向服务器发送数据，服务器将收到这些数据 
    client.write('chuimaoyu chuimaoyu');

});

// 为客户端添加“data”事件处理函数
// data是服务器发回的数据
client.on('data', function(data) {

    console.log('DATA: ' + data);
    // 完全关闭连接
    //client.destroy();

});

//"error"处理
client.on('error',function(error){
    console.log('error:'+error);
    client.destory();
});

// 为客户端添加“close”事件处理函数
client.on('close', function() {
    console.log('Connection closed');
    rl.close();
});

client.on('message', function(message){
    console.log('message: ' + message);
});

//得到键盘输入
rl.on('line', function(line){
    var readin = line.trim()
    switch(readin) {
        case 'copy':
            console.log("复制");
            break;
        case 'hello':
            console.log('world!');
            break;
        case 'help':
            console.log('help: you can ... ');
            break;
        case 'send':
            console.log('send a file?')
            client.write('send something!');
            break;
        case 'logout':
            console.log('ready to logout');
            client.write('o $%^&*()');
            break;
        case 'close':
            rl.close();
            break;
        default:
            console.log('没有找到命令！');
            break;
    }
});

rl.on('close', function() {
    console.log('bye bye');
    process.exit(0);
});