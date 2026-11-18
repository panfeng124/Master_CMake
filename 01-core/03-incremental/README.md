# 第 3 课：从日志看懂增量构建

本课 40 分钟：英文阅读 5 分钟，建立基线 10 分钟，对照实验 20 分钟，整理结论 5 分钟。

## 先读英文

> Enable verbose output - if supported - including the build commands to be executed.

出处：[CMake 官方手册，Build a Project，--verbose](https://cmake.org/cmake/help/latest/manual/cmake.1.html#build-a-project)。

自然释义：如果构建工具支持，就启用详细输出，其中包括将要执行的构建命令。

- **verbose output**：详细输出；本课用于观察编译、链接是否真正执行。
- **if supported**：如果支持；不同构建工具的日志格式和详细程度会不同。
- **commands to be executed**：将要执行的命令；`to be executed` 修饰 commands，表示这些命令将被执行。

阅读题：这句话能否说明 `--verbose` 会强制重新编译所有源码？请指出它实际改变的是什么。

## 目标与起点

解释三个现象：首次构建需要编译和链接；输入未变时重复构建可跳过这些工作；共享源码变化后，只构建一个目标不会自动更新另一个独立目标。

本目录的 `main.cpp` 和 `CMakeLists.txt` 来自第二课，起始输出是 `LAB v2 new!`。两个可执行目标都是从同一个 main.cpp 构建的，但各自有目标文件和 exe。没有复制上课的构建目录；新目录需要重新配置。

## 建立本课基线（10 分钟）

在 PowerShell 中执行；每一步成功后再继续。若已能直接使用 cmake，可省略 Path 设置。

```powershell
Set-Location 'F:\PF\LAB\Master_CMake\01-core\03-incremental'
$cmakeBin = 'C:\Program Files\JetBrains\CLion 2025.3.2\bin\cmake\win\x64\bin'
$env:Path = "$cmakeBin;$env:Path"
cmake -S . -B build -G 'Visual Studio 16 2019'
cmake --build build --config Debug --verbose
.\build\Debug\lab_cli.exe
.\build\Debug\lab_cli_pf.exe
```

本课沿用 Visual Studio / MSBuild。第一条 cmake 命令读取 CMakeLists.txt，配置并生成构建规则；第二条调用构建工具，按规则编译 main.cpp 并链接程序。两个程序应都输出 `LAB v2 new!`。

日志中寻找编译器 `cl.exe` 和链接器 `link.exe` 的实际调用。`main.cpp → .obj → .exe` 分别对应源文件、编译得到的目标文件、链接得到的可执行文件。注意：CMake 的“目标”（target）与编译器的“目标文件”（object file）是不同概念。

## 用同一条命令做对照（20 分钟）

先不改任何文件，执行下面这条命令两次：

```powershell
cmake --build build --config Debug --target lab_cli --verbose
```

基线已经构建成功，两次通常都无需重新编译和链接。构建工具仍会检查依赖是否需要更新，所以终端仍可能打印许多信息。

Visual Studio 日志可能出现 `ClCompile`、`Link` 段落名以及“所有输出均为最新”等信息。段落名出现不代表编译器或链接器执行了；要看实际命令和跳过提示。若详细程度不足，可额外观察：

```powershell
cmake --build build --config Debug --target lab_cli -- /v:detailed
```

这里 `--` 把后面的选项交给底层工具；`/v:detailed` 是 MSBuild 的详细日志选项。

现在只把 main.cpp 的输出改为 `LAB v3!`，保存；先直接运行两个 exe，记录结果，再执行：

```powershell
cmake --build build --config Debug --target lab_cli --verbose
.\build\Debug\lab_cli.exe
.\build\Debug\lab_cli_pf.exe
```

修改源码并不会直接改写已有 exe。构建 lab_cli 时，构建工具发现它的输入已更新，重新编译并链接；另一个目标与它没有依赖关系，这次没有被选中，因此暂时保留旧输出。

立刻再执行同一条 lab_cli 构建命令。这次应恢复到无需重新编译和链接的状态。`--target` 选择构建范围（目标及所需依赖）；在该范围内，构建工具根据依赖和文件更新时间等信息决定哪些工作需要重做。这就是本课观察的增量构建。

本次只改 C++ 源码，不需要手动重新运行配置命令。不要删除 build、切换配置或使用 --clean-first，否则会改变对照条件。

## 独立任务与验证（5 分钟）

1. 自己写一条命令更新 lab_cli_pf，再运行两个程序，确认均输出 `LAB v3!`。
2. 再执行一次默认构建，查看两个目标是否还执行编译和链接。
3. 创建 `我的记录.md`，记录下表中的实际观察，每种情况保留少量关键日志，不必粘贴整份输出。

| 操作 | 是否实际编译、链接 | lab_cli 输出 | lab_cli_pf 输出 |
| --- | --- | --- | --- |
| 首次构建两个目标 | 待观察 | 待观察 | 待观察 |
| 输入不变，只构建 lab_cli | 待观察 | 待观察 | 待观察 |
| 保存新源码，尚未构建 | 未执行构建 | 待观察 | 待观察 |
| 修改后只构建 lab_cli | 待观察 | 待观察 | 待观察 |
| 立即重复构建 lab_cli | 待观察 | 待观察 | 待观察 |
| 更新 lab_cli_pf 后默认构建 | 待观察 | 待观察 | 待观察 |

成功标准：用日志证明“命令执行成功”不等于“重新编译过”，并能解释为什么两个 exe 曾输出不同内容。

英文表达：补全 `The build tool skipped compilation because ___.`，用一句话解释一次没有编译工作的构建。

完成后回复“打卡”或“完成”，再整理个人笔记、登记课时并提交推送。
