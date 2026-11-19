# 第 7 课：改一行变量，验证目标改名

> Add an executable target called `<name>` to be built from the source files listed in the command invocation.

出处：本机 `cmake --help-command add_executable`，Normal Executables 段。

意思是：添加名为 `<name>` 的可执行目标，使用这次调用里列出的源文件构建它。`called <name>` 说明目标叫什么；`built from` 说明用什么构建。

## 目标

区分配置日志、生成的目标规则和磁盘上的 exe。上课时的脚本中，`add_executable(${app} main.cpp)` 在配置时读取 app 的值作为目标名。改名后重新配置、生成，构建工具就能按新名字选目标。两个名字都使用 main.cpp，所以程序输出可以相同。

## 动手与验证（40 分钟）

在本目录打开 PowerShell。若找不到 cmake，可执行：

```powershell
Set-Alias cmake 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
```

1. 建立基线，记下配置消息和运行输出。

   ```powershell
   cmake -S . -B build
   cmake --build build --config Debug --target lab_cli_pf
   .\build\Debug\lab_cli_pf.exe
   ```

2. 只把 set 中的 lab_cli_pf 改为 lab_probe。先写下预测：消息显示什么，哪条构建命令要改，程序会打印什么？
3. 再运行同一条配置命令。确认日志出现 `-- value of variable app is lab_probe`。自行调整基线中的构建命令和运行路径；成功标志是按新名构建成功，新 exe 输出 `LAB v3!`。
4. 用 `Get-ChildItem .\build\Debug\*.exe` 检查旧文件是否还在。再指定旧目标名构建一次，记录结果。磁盘可以保留上轮的 exe；当前构建规则是否接受某个目标名，要用构建命令验证。

英语练习：用 called 和 built from 写一句话，说明改名后的目标叫什么、使用哪个源文件。
