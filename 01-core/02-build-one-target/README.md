# 第 2 课：只构建一个目标

> Build `<tgt>` instead of the default target. Multiple targets may be given, separated by spaces.

指定目标名后，构建工具会构建所选目标；可以用空格分隔多个目标名。default target 指省略 `--target` 时的默认目标，本例的默认构建包含两个程序。[CMake 官方手册](https://cmake.org/cmake/help/latest/manual/cmake.1.html#build-a-project)

## 目标和起点

用两个程序的输出解释一次增量构建的范围。沿用上课的 Visual Studio 生成器，在本目录打开上课用的 PowerShell。两个 add_executable 调用各自定义一个目标，共用 main.cpp，各自产生一个 exe。

## 动手验证

先配置并生成构建规则，再构建 Debug 版本，运行两个程序建立对照。

```powershell
cmake -S . -B build
cmake --build build --config Debug
.\build\Debug\lab_cli.exe
.\build\Debug\lab_cli_pf.exe
```

确认两次均输出 LAB ready!。在本课 main.cpp 中把字符串改成 LAB v2! 并保存。先写下预测，再执行：

```powershell
cmake --build build --config Debug --target lab_cli
.\build\Debug\lab_cli.exe
.\build\Debug\lab_cli_pf.exe
```

第一行要求更新 lab_cli 及其所需依赖。两个程序互不依赖，因此应依次看到新、旧输出。源码变化只有经过编译和链接才会进入相应的 exe。

独立任务：写一条命令只更新第二个程序，再运行两者。用三组实际输出验证两者从同旧、到一新一旧、再到同新。英文练习：用 “I built ___ instead of ___.” 说明第二轮选择了哪个目标。
