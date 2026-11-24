# 第 11 课：把诊断实现拆成静态库

> an archive of object files for use when linking other targets.

出处：CMake 命令帮助 `cmake --help-command add_library`，Normal Libraries 小节对 STATIC 的解释。[官方文档](https://cmake.org/cmake/help/latest/command/add_library.html#normal-libraries)

静态库把目标文件归档，供其他目标在链接时使用。这里的 **object files** 指编译器处理源文件后生成的文件，**when linking other targets** 说的是使用时机。本例中，diagnostics.cpp 编译得到的目标文件进入 lab_core.lib，链接 lab_probe.exe 时才从库中取用函数实现。

## 动手任务（40 分钟）

起点是上节已能运行的工程。目标是只改 app/CMakeLists.txt，让诊断实现属于 lab_core，保留 LAB_VERBOSE 开关的效果。

1. 阅读两个 cpp 文件。哪个调用 print_diagnostics()？哪个在编译时检查 LAB_VERBOSE？
2. 用 `add_library(<目标名> STATIC <源文件>)` 声明 lab_core。让 lab_probe 的 add_executable 只列 main.cpp。两个目标都写在 app 目录，头文件沿用原位置。
3. 用 `target_link_libraries(<使用方> PRIVATE <提供方>)` 表达依赖。PRIVATE 在这里表示库是程序自身的链接依赖。构建程序时，CMake 生成的规则会先备好它需要的库。
4. 检查已有 if 块，把编译宏交给实际编译诊断实现的目标。若程序能运行却少了一行输出，回到第 1 步核对宏的归属。

## 验证

在本课目录运行 PowerShell。以下使用本机 Visual Studio 多配置生成器。如终端找不到 cmake，先执行：

~~~powershell
Set-Alias cmake 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
~~~

~~~powershell
cmake -S . -B build -G "Visual Studio 18 2026" -DLAB_VERBOSE=ON
cmake --build build --config Debug --target lab_core
Get-ChildItem build/app/Debug
cmake --build build --config Debug --target lab_probe
.\build\app\Debug\lab_probe.exe
~~~

使用全新的 build 目录时，第一次构建后应能找到 lab_core.lib，此时还没有 lab_probe.exe。第二次构建后，程序依次输出 LAB v3! 和 Diagnostics enabled。再把配置命令的 ON 改为 OFF，重新配置并构建 lab_probe，运行后应只剩第一行。

**英语练习**：用 “when linking” 写一句话，说明 lab_probe 在哪个阶段用到 lab_core。可把句子和两次运行的输出记在本目录的“我的记录.md”。
