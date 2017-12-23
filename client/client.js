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
    client.write('connect! good!');

});

// 为客户端添加“data”事件处理函数
// data是服务器发回的数据
client.on('data', function(data) {
    //var backdata = data.trim()
    var arr = data.toString().split(" ")
    var username_unavailable = "U"
    var login_success = "S"
    var wrong_username = "E";
    var wrong_password = "P";
    var users_full = "F"
    switch(arr[0][0]){
        case username_unavailable[0]:
            console.log('This username is not available, please choose another one.');
            break;
        case login_success[0]:
            console.log('Login success!!!');
            break;
        case wrong_username[0]:
            console.log('Wrong username!!!');
            break;
        case wrong_password[0]:
            console.log('Wrong password!!!');
            break;
        case users_full[0]:
            console.log('There are too many users!!!');
            break;
        default:
            console.log('DATA: ' + arr[0] + '??' + arr.length);
            break;
    }
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

//得到键盘输入
rl.on('line', function(line){
    var readin = line.trim()
    var arr = readin.split(" ")
    switch(arr[0]) {
        case 'help':
            console.log('help: you can ... ');
            console.log('register [username] [password]     -To register')
            console.log('login [username] [password] 		—To login');
            console.log('logout           				    —To log out');
            console.log('send [send_to_username] [content] 	—To send a message to a friend');
            console.log('help						        —To help');
            console.log('check					            —To check all your friends');
            console.log('checkonline					    —To get all your online friends');
            break;
        case 'register':
            if(arr.length == 3)
            {
                console.log('ready to register~');
                client.write('R' + ' ' + arr[1] + ' ' + arr[2]);
            }
            else
            {
                console.log('Wrong format!');
                console.log('Please input: register [username] [password]');
            }
            break;
        case 'login':
            if(arr.length == 3)
            {
                console.log('ready to login~');
                client.write('L' + ' ' + arr[1] + ' ' + arr[2]);
            }
            else
            {
                console.log('Wrong format!');
                console.log('Please input: login [username] [password]');
            }
            break;
        case 'logout':
            console.log('ready to logout');
            client.write('O $%^&*()');
            rl.close();
            break;
        case 'send':
            if(arr.length == 3)
            {
                console.log('ready to send a message~');
                client.write('M' + ' ' + arr[1] + ' ' + arr[2]);
            }
            else
            {
                console.log('Wrong format!');
                console.log('Please input: send [send_to_username] [content]');
            }
            break;
        case 'check':
            client.write('A');
            break;
        case 'checkonline':
            client.write('S');
            break;
        default:
            console.log('没有找到命令！');
            console.log("use 'help' for help!")
            break;
    }
});

rl.on('close', function() {
    console.log('bye bye');
    process.exit(0);
});