# 说明

测试c++的一些特性

## 入口说明

* `src/main.cpp` 为主程序入口，对应的可执行文件为`build/src/TestCpp`
* `tests/main.cpp` 为测试程序入口，对应的可执行文件为`build/tests/TestApp`

## 前置条件

* 运行环境: `Ubuntu 20.04 LTS`
* 安装gcc: `sudo apt install gcc`
* 安装cmake: `sudo apt install cmake`
* 安装entr: `sudo apt-get install entr`

## 指令

* 生成构建文件: `cmake -S . -B build` 
* 构建项目: `cmake --build build` 
* 运行主程序: `./build/src/TestCpp` 
* 运行测试: `./build/tests/TestApp` 

## 脚本

* 以watch方式运行TestCpp: `./scripts/watch.sh`
* 以watch方式运行TestApp: `./scripts/watch_test.sh`
