# cpp-socks5

[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Language](https://img.shields.io/badge/language-c++11-red.svg)](https://en.cppreference.com/)
[![Platform](https://img.shields.io/badge/platform-linux-lightgrey.svg)](https://img.shields.io/badge/platform-linux-lightgrey.svg)

## 介绍
基于 cpp-tbox 框架实现的 socks5 服务。

## 起源
之前我尝试使用其它开源的socks5开源软件，但它总异常崩溃，我也懒得去帮它查bug了。  
于是我看了一下SOCKS5的协议文档，感觉很简单，加上我手上有 cpp-tbox 成熟的架框，写这种东西轻而易举。于是就决定自己写一个socks5服务程序，便有了cpp-socks5项目。  
在这个程序完成之后，我对它进行了测试，连续使用它开播放1080P以上连续剧，测试了三天三夜。结果非常稳定，性能上也毫无压力。  

## 功能

- 支持 "No Auth" 验证；
- 支持 "Username/Passowrd" 验证；
- 支持 CONNECT 命令；
- 支持 IPv4；

## 构建
先构建 cpp-tbox 项目。  

然后再构建自身：
```
cd cpp-socks3/src;
make
```

## 运行

```
./cpp-socks5 -c config.json
```
