# cpp-socks5

[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Language](https://img.shields.io/badge/language-c++11-red.svg)](https://en.cppreference.com/)
[![Platform](https://img.shields.io/badge/platform-linux-lightgrey.svg)](https://img.shields.io/badge/platform-linux-lightgrey.svg)

[[中文]](README_CN.md)

## About
A socks5 proxy server base on [cpp-tbox](https://github.com/cpp-main/cpp-tbox)

## Origin
I tried to use other open source socks5 open source software before, but it always crashed abnormally.
So I took a look at the [SOCKS5 protocol document](https://datatracker.ietf.org/doc/rfc1928/), it feels relatively simple, and I have [cpp-tbox](https://github.com/cpp-main/cpp-tbox) mature framework, it is easy to write this kind of thing. So Meng gave birth to the idea of writing a SOCKS5 service program by himself, and the cpp-socks5 project came into being.
After the program was completed, I tested it and used it continuously to play videos above 1080P for three days and three nights, and it continued to run for more than 40 days. The result is very satisfactory, the program is stable, and there is no pressure on performance. 

The following is the use effect after 41 days of continuous operation:  
![show](documents/images/show.gif)

## Features

- Support "No Auth" authentication;
- Support "Username/Password" authentication;
- Support CONNECT command;
- Support IPv4；

## Build
### Step 1：build [cpp-tbox](https://github.com/cpp-main/cpp-tbox) first  
```
git clone https://gitee.com/cpp-master/cpp-tbox.git
cd cpp-tbox;
STAGING_DIR=$HOME/.tbox make 3rd-party modules RELEASE=1
cd -
```

### Step 2：Build cpp-socks5 self
```
git clone https://gitee.com/cpp-master/cpp-socks5.git
cd cpp-socks5/src;
make
```

## Run
Run with default parameters：
```
./cpp-socks5
```
Run with arguments:
```
./cpp-socks5 -c config/default.conf
```


## Configure
You can modify the configuration based on src/config/full.conf.
```
{
  "socks5": {
    "bind":"0.0.0.0:1080", // Service binding address and port, if not specified, the default is "0.0.0.0:1080"
    "backlog":20,          // listen(backlog), default is 20
    "username":"abc",      // Username
    "password":"123",      // Password
  }
}
```
If username and password are specified at the same time, Username/Password authentication will be enabled, otherwise No Auth authentication will be defaulted.

## Feedback
- Issue: Any questions are welcome to communicate in issue
- WeChat: hevake_lee
- QQ group: 738084942 (cpp-tbox 技术交流)

## Encourage us
If you find it useful, please give me more encouragement.
You can do these:

- Follow [cpp-tbox](https://github.com/cpp-main/cpp-tbox) open source project;
- Light up three combos for it: Star, Watch, Fork;
- Recommend to colleagues and partners around you, and recommend to your readers in technical forums;
- Join the above QQ group, add me on WeChat to enter the WeChat group;
- Positive feedback on issues and suggestions;
- Participate in the development of the project and contribute your strength.
