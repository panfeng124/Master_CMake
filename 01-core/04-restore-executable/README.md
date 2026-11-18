# 第 4 课：只移走 exe，观察链接怎样恢复它

> Add an executable target called `<name>` to be built from the source files listed in the command invocation.

出处：本机命令帮助 `cmake --help-command add_executable`，Normal Executables 一节。意思是：添加一个名为 <name> 的可执行目标，使用这次命令调用中列出的源文件来构建它。**be built from** 说明来源。本例的 lab_cli 最终来自 main.cpp。阅读题：这句话能告诉你源文件是什么，但能单凭它判断这次构建有没有运行编译器吗？还要查哪条日志？

## 建立基线（10 分钟）

目标：在源码和目标文件仍然有效时，通过实际命令判断恢复缺失的程序需要哪一步。进入本目录，在 PowerShell 中运行：

```powershell
Set-Location 'F:\PF\LAB\Master_CMake\01-core\04-restore-executable'
$env:Path = 'C:\Program Files\JetBrains\CLion 2025.3.2\bin\cmake\win\x64\bin;' + $env:Path
cmake -S . -B build -G 'Visual Studio 16 2019'
cmake --build build --config Debug
.\build\Debug\lab_cli.exe
```

应看到 LAB v3!。打开 build/lab_cli.dir/Debug，找到 main.obj。编译器把 main.cpp 编译成这个文件，链接器再用它和所需库生成 build/Debug/lab_cli.exe。

## 改变一个条件（15 分钟）

```powershell
Rename-Item .\build\Debug\lab_cli.exe lab_cli.saved.exe
Test-Path .\build\Debug\lab_cli.exe
Test-Path .\build\lab_cli.dir\Debug\main.obj
cmake --build build --config Debug --target lab_cli --verbose
.\build\Debug\lab_cli.exe
```

两次检查应依次返回 False、True。改名保留了原程序，同时让构建规则指定的输出路径变为空缺。观察 ClCompile 下的跳过提示，以及 Link 下带 /OUT: 的 link.exe 命令。查看命令末尾的 main.obj，再运行恢复的 exe，核对输出。保持所有文件原样再构建一次，比较链接器是否再次运行。

## 自己验证（15 分钟）

从仍然完整的 lab_cli_pf 开始。自己找到它的 exe 和 main.obj，先写下预测，再只给 exe 改名，连续构建该目标两次。将两轮的实际编译、链接情况，各自的关键日志，以及恢复后的程序输出写进个人笔记。验收时应能指出：两轮使用了相同的源码和命令，是什么文件状态导致执行步骤不同？

用英语补全：The linker ran because ___. The compiler skipped main.cpp because ___.
