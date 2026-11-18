# 第 5 课：移走 main.obj，跟踪重新编译和链接

> Enable verbose output - if supported - including the build commands to be executed.

出处：本机运行 `cmake --build` 显示的 `--verbose` 帮助。意思是：如果构建工具支持，就显示详细输出，其中包括将要执行的构建命令。**to be executed** 修饰 **build commands**。阅读题：加上这个选项，会强制执行编译吗？用最后两次日志回答。

## 建立基线（10 分钟）

目标：用日志解释缺失一个中间产物如何影响同一目标的后续步骤。起点是上一课的两个目标和同一份 main.cpp。在本目录打开 PowerShell。若找不到 cmake，可先运行本机这行：

~~~powershell
$env:Path = 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;' + $env:Path
~~~

~~~powershell
cmake -S . -B build -G 'Visual Studio 17 2022'
cmake --build build --config Debug
Get-ChildItem build -Recurse -Filter main.obj | Select-Object FullName
~~~

两个目标各自编译 main.cpp，各有一份 main.obj。根据路径找出属于 lab_cli 的那份。写下预测：保留 exe，只移走这份 obj，再构建 lab_cli 会执行哪几步？

## 只改变一个条件（15 分钟）

将下方占位文字换成刚查到的完整路径，给该文件改名。保持源码和 exe 原样。

~~~powershell
Rename-Item -LiteralPath '这里填 lab_cli 的 main.obj 路径' -NewName main.obj.saved
cmake --build build --config Debug --target lab_cli --verbose
.\build\Debug\lab_cli.exe
~~~

检查 main.obj 是否重新出现。编译器用 main.cpp 生成新的 main.obj，链接器再用它更新 lab_cli.exe。找出日志中实际处理 main.cpp 和生成 lab_cli.exe 的命令，留意 cl.exe 和 link.exe 的输入、输出。

## 用重复构建验证（15 分钟）

原样再执行一次带 --verbose 的命令。对照两次日志，分别摘出编译、链接实际执行或跳过的证据。记录程序是否输出 LAB v3!，并解释为什么单看这行输出无法判断它经历了哪些构建步骤。
