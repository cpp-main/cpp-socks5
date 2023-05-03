# cpp-socks5

[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Language](https://img.shields.io/badge/language-c++11-red.svg)](https://en.cppreference.com/)
[![Platform](https://img.shields.io/badge/platform-linux-lightgrey.svg)](https://img.shields.io/badge/platform-linux-lightgrey.svg)

## 介绍
基于 cpp-tbox 框架实现的 socks5 服务

## 功能

- 支持 "No Auth" 验证；
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