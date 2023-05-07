# cpp-socks5

[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Language](https://img.shields.io/badge/language-c++11-red.svg)](https://en.cppreference.com/)
[![Platform](https://img.shields.io/badge/platform-linux-lightgrey.svg)](https://img.shields.io/badge/platform-linux-lightgrey.svg)

## 介绍
基于 cpp-tbox 框架实现的 socks5 服务

之前我尝试使用其它开源的socks5开源软件，但它总异常崩溃，我也懒得去帮它查bug了。  
于是我看了一下SOCKS5的协议文档，感觉很简单，加上我手上有 cpp-tbox 成熟的架框。于是就决定自己写一个socks5服务程序。便有了cpp-socks5项目。  
这个程序完成之后，我对它进行了测试。我连续使用它开播放连续剧，稳定可靠，毫无压力。  

## 功能

- 支持 "No Auth" 验证；
- 支持 "Username/Passowrd" 验证；
- 支持 CONNECT 命令；
- 支持 IPv4；

## 构建

```
cd cpp-socks3/src;
make
```

## 运行

```
./cpp-socks5 -c config.json
```
