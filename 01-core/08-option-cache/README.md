# 第 8 课：用配置选项和缓存控制程序输出

## 先读英文

> If no initial `<value>` is provided, boolean `OFF` is the default value.
> If `<variable>` is already set as a normal or cache variable, then the command does nothing (see policy `CMP0077`).

出处：本机 CMake 命令帮助 `cmake --help-command option`。

没有提供初始值时，布尔选项默认关闭。如果同名普通变量或缓存变量已有值，option() 就保留已有设置。initial 指初始化时；is already set 指执行到这行之前，值已经存在。

## 目标与起点

从本目录打开 PowerShell。起始程序沿用 lab_probe，在 LAB v3! 后可选地输出 Diagnostics enabled。先读 CMakeLists.txt 和 main.cpp，找出三处连接点：option 声明配置开关，if 判断开关并为目标添加编译宏，C++ 的 #ifdef 决定是否编入诊断行。程序运行时无需读取缓存。

## 动手验证（40 分钟）

读代码和英文约 8 分钟，实验约 22 分钟，解释结果约 10 分钟。先预测每一步的配置值、程序行数，再运行。若 PowerShell 找不到 cmake，本机可先运行：

```powershell
$clionBin = 'C:\Program Files\JetBrains\CLion 2024.2.0.1\bin'
$env:Path = "$clionBin\cmake\win\x64\bin;$clionBin\ninja\win\x64;$clionBin\mingw\bin;$env:Path"
```

第一次实验使用两个尚未配置过的构建目录。

```powershell
cmake -S . -B build-a -G Ninja -DLAB_VERBOSE=ON
cmake --build build-a
.\build-a\lab_probe.exe

cmake -S . -B build-a
cmake --build build-a
.\build-a\lab_probe.exe

cmake -S . -B build-b -G Ninja
cmake --build build-b
.\build-b\lab_probe.exe

Select-String -Path build-a/CMakeCache.txt,build-b/CMakeCache.txt -Pattern '^LAB_VERBOSE:BOOL='
```

两次运行 A 都应输出两行，B 只输出一行。两个缓存文件分别显示 ON 和 OFF。独立任务：保留两个目录，用配置命令让 A 关闭、B 开启，分别构建并运行。记录实际命令、缓存行和输出。若只配置就运行原来的 exe，预测输出何时改变，再验证。

英文表达：用 “is already set” 解释为什么第二次配置 A 时省略 -D 仍会输出诊断行。
