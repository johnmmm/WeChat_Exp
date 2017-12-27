var net = require('net');
var fs = require("fs");
var readline = require('readline');

var HOST = '127.0.0.1';
var PORT = 7474;

var txturl = './info/'
var saveurl = "./download/"
var who_am_i = 'no one'
var tmp_who

var is_file = 0
var file_format = "txt"

var flag = 1
require('crypto').randomBytes(16, function(ex, buf) {  
    var token = buf.toString('hex');  
    flag = token
});  
console.log(flag); 

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
    if(is_file == 1)
    {
        //receive the file
 
        console.log("save?")
        console.log(data)
        console.log("an?")
        console.log(saveurl + flag.toString() + '.' + file_format)
        var urls = "./download/" + flag.toString() + '.' + file_format
        console.log(file_format.length)
        console.log(urls)
        fs.writeFile(urls, data.toString(), function(err) {
            if(err) {
                return console.log(err);
            }
            console.log("The file was saved!");
        });
        is_file = 0
    }

    //var backdata = data.trim()
    var arr = data.toString().split(" ")
    var success = "C"
    var username_unavailable = "U"
    var register_success = "G"
    var login_success = "S"
    var wrong_username = "E";
    var wrong_password = "P";
    var already_login = "A";
    var not_friend = "I";
    var already_friend = "L"
    var users_full = "F"
    var not_login = "N"
    var not_online = "O"
    var fake_person = "K"
    var new_friend = 'Q'
    var check_friend = 'H'
    var delete_success = 'D'
    var cache_message = 'B'
    var file_message = 'Z'
    switch(arr[0][0]){
        case success[0]:
            console.log('User: ' + arr[1] + ' send you a message: ')
            console.log(arr[2])
            var data = fs.readFileSync(txturl + who_am_i + '.txt');

            var more_text = data.toString()
            more_text = more_text + who_am_i + ' send to ' + arr[1] + ': ' + arr[2] + '\n'

            fs.writeFile(txturl + who_am_i + '.txt', more_text, function(err) {
                if (err) {
                    return console.error(err);
                }
            })
            break
        case username_unavailable[0]:
            console.log('This username is not available, please choose another one.');
            break;
        case register_success[0]:
            console.log('Register success!!!')
            who_am_i = tmp_who
            fs.writeFile(txturl + who_am_i + '.txt', "", function(err) {
                if(err) {
                    return console.log(err);
                }
                console.log("The file was saved!");
            });
            break;
        case login_success[0]:
            console.log('Login success!!!');
            who_am_i = tmp_who
            break;
        case wrong_username[0]:
            console.log('Wrong username!!!');
            break;
        case wrong_password[0]:
            console.log('Wrong password!!!');
            break;
        case already_login[0]:
            console.log('Failed! You have already been online!!!');
            break;
        case not_friend[0]:
            console.log('Failed! This user is not your friend!!!')
            break;
        case already_friend[0]:
            console.log('Failed! This user is already your friend!');
            break;
        case users_full[0]:
            console.log('There are too many users!!!');
            break;
        case not_login[0]:
            console.log('You have not been online, please sign in first!!!')
            break;
        case not_online[0]:
            console.log("Target is not online now, please try later~")
            break;
        case fake_person[0]:
            console.log('You are sending message to a fake person!!!')
            break;
        case new_friend[0]:
            console.log('User ' + arr[1] + ' want to be your friend!!!')
            var data = fs.readFileSync(txturl + who_am_i + '.txt');

            var more_text = data.toString()
            more_text = more_text + 'User ' + arr[1] + ' want to be your friend!!!' + '\n'

            fs.writeFile(txturl + who_am_i + '.txt', more_text, function(err) {
                if (err) {
                    return console.error(err);
                }
            })
            break;
        case check_friend[0]:
            console.log('Your friend list: ')
            for(var i = 1; i < arr.length; i++)
            {
                console.log(arr[i])
            }
            break;
        case delete_success[0]:
            console.log('Delete friend success!!!')
            break;
        case cache_message[0]:
            console.log('Login success!!!');
            who_am_i = tmp_who
            console.log('Message during your offline:')
            for(var i = 1; i < arr.length; i++)
            {
                console.log(arr[i])
            }
            break;
        case file_message[0]:
            console.log('Somebody want to send you a file~')
            is_file = 1
            file_format = arr[1]
            break;
        default:
            console.log(arr[0]);
            break;
    }
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
    var register = 'R'
    var login = 'L'
    var help = 'H'
    var chat_message = 'M'
    var ask_friend_list = 'A'
    var ask_friend_online = 'S'
    var friend_request = 'Q'
    var delete_friend = 'D'
    var file_request = 'I'
    var logout = 'O'
    var unknown = 'U'

    switch(arr[0]) {
        case 'help':
            console.log('help: you can ... ');
            console.log('register [username] [password]     —To register')
            console.log('login [username] [password]        —To login');
            console.log('logout                             —To log out');
            console.log('send [send_to_username] [content]  —To send a message to a friend');
            console.log('help                               —To help');
            console.log('check                              —To check all your friends');
            console.log('checkonline                        —To get all your online friends');
            console.log('friend [username]                  -To send a friend request to other users')
            console.log('dfriend [username]                 -To delete a friend in your list')
            console.log('file [username] [fileurl]          -To make a file sending request')
            break;
        case 'register':
            if(arr.length == 3)
            {
                console.log('ready to register~');
                client.write(register + ' ' + arr[1] + ' ' + arr[2]);
                tmp_who = arr[1]
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
                client.write(login + ' ' + arr[1] + ' ' + arr[2]);
                tmp_who = arr[1]
            }
            else
            {
                console.log('Wrong format!');
                console.log('Please input: login [username] [password]');
            }
            break;
        case 'logout':
            console.log('ready to logout');
            client.write(logout + '$%^&*()');
            rl.close();
            break;
        case 'send':
            if(arr.length == 3)
            {
                console.log('ready to send a message~');
                client.write(chat_message + ' ' + arr[1] + ' ' + arr[2]);

                var data = fs.readFileSync(txturl + who_am_i + '.txt');

                var more_text = data.toString()
                more_text = more_text + who_am_i + ' send to ' + arr[1] + ': ' + arr[2] + '\n'

                fs.writeFile(txturl + who_am_i + '.txt', more_text, function(err) {
                    if (err) {
                        return console.error(err);
                    }
                })
            }
            else
            {
                console.log('Wrong format!');
                console.log('Please input: send [send_to_username] [content]');
            }
            break;
        case 'check':
            client.write(ask_friend_list);
            break;
        case 'checkonline':
            client.write(ask_friend_online);
            break;
        case 'friend':
            if(arr.length == 2)
            {
                client.write(friend_request + ' ' + arr[1])
            }
            else
            {
                console.log('Wrong format!');
                console.log('Please input: friend [username]')
            }
            break;
        case 'dfriend':
            if(arr.length == 2)
            {
                client.write(delete_friend + ' ' + arr[1])
            }
            else
            {
                console.log('Wrong format!');
                console.log('Please input: dfriend [username]')
            }
            break;
        case 'file':
            if(arr.length == 3)
            { 
                fs.exists(arr[2], function(exists) {  
                    if(exists)
                    {
                        var data = fs.readFileSync(arr[2]);
                        var fileurls = arr[2].split('.')
                        client.write(file_request + ' ' + arr[1] + ' ' + fileurls[fileurls.length-1])
                        console.log(file_request + ' ' + arr[1] + ' ' + fileurls[fileurls.length-1])
                        var count = 0
                        for(var i = 0; i < 1000000; i++)
                        {
                            count++
                        }
                        client.write(data)
                    }
                    else
                    {
                        console.log('File not found!!!')
                    }
                });
            }
            else
            {
                console.log('Wrong format!');
                console.log('Please input: file [username] [fileurl]');
            }
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