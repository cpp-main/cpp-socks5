# cpp-socks5

[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Language](https://img.shields.io/badge/language-c++11-red.svg)](https://en.cppreference.com/)
[![Platform](https://img.shields.io/badge/platform-linux-lightgrey.svg)](https://img.shields.io/badge/platform-linux-lightgrey.svg)

[[English]](README.md)

## 介绍
基于 [cpp-tbox](https://gitee.com/cpp-master/cpp-tbox) 框架实现的 socks5 服务。

## 起源
之前我尝试使用其它开源的socks5开源软件，但它总异常崩溃。  
于是我看了一下[SOCKS5的协议文档](https://datatracker.ietf.org/doc/rfc1928/)，感觉比较简单，加上我手上有 [cpp-tbox](https://gitee.com/cpp-master/cpp-tbox) 成熟的架框，写这种东西也很容易。于是蒙生出自己写一个 SOCKS5 服务程序的想法，便有了 cpp-socks5 项目。  
在这个程序完成之后，我对它进行了测试，连续使用它一直播放1080P以上的视频，测试了三天三夜，并连续运行40多天。结果非常满意，程序稳定，性能上也毫无压力。  

以下为连续运行了41天后的使用效果：  
![演示](documents/images/show.gif)

## 功能

- 支持 "No Auth" 验证；
- 支持 "Username/Passowrd" 验证；
- 支持 CONNECT 命令；
- 支持 IPv4；

## 构建
### 步骤一：构建 [cpp-tbox](https://gitee.com/cpp-master/cpp-tbox) 项目  
```
git clone https://gitee.com/cpp-master/cpp-tbox.git
cd cpp-tbox;
STAGING_DIR=$HOME/.tbox make 3rd-party modules RELEASE=1
cd -
```

### 步骤二：构建 cpp-socks5 自身
```
git clone https://gitee.com/cpp-master/cpp-socks5.git
cd cpp-socks5/src;
make
```

## 运行
默认参数运行：
```
./cpp-socks5
```
带参数的运行：
```
./cpp-socks5 -c config/default.conf
```


## 配置说明
你可以在 src/config/full.conf 的基础上修改配置。  
```
{
  "socks5": {
    "bind":"0.0.0.0:1080", // 服务绑定地址与端口，不指定，默认为"0.0.0.0:1080"
    "backlog":20,          // listen(backlog) 参数，默认为20
    "username":"abc",      // Username
    "password":"123",      // Password
  }
}
```
如果同时指定了 username 与 password，则会开启 Username/Password 验证，否则默认为 No Auth 验证。

## 反馈途径
- Issue: 任何问题都欢迎在issue里交流
- 微信: hevake_lee
- QQ群: 738084942 (cpp-tbox 技术交流)

## 鼓励我们
如果你觉得很有用，请您给我更多的鼓励。  
你可以做这些：

- 关注 [cpp-tbox](https://gitee.com/cpp-master/cpp-tbox) 开源项目；
- 给它点亮三连： Star, Watch, Fork；
- 向身边的同事与伙伴推荐，在技术论坛向您的读者推荐；
- 加入上面的QQ群、加我微信进入微信群；
- 积极反馈问题，提出建议；
- 参与项目的开发，贡献您的力量。
