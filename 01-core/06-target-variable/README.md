# 第 6 课：用普通变量给目标命名

> A variable reference is replaced by the value of the specified variable.

出处：CMake 自带手册，运行 `cmake --help-manual cmake-language.7`，查找 Variable References。

目标是在配置日志中读出变量的值，再用这个值选中并构建目标。原文中的 variable reference 指 `${app}`，is replaced by 表示“会被……替换”。例如：

```cmake
set(app lab_cli)
message(STATUS "Configuring target ${app}")
add_executable(${app} main.cpp)
```

配置时，set 给普通变量赋值，后两行分别把这个值写进日志、用作目标名。生成完成后，构建工具就能按这个名字找到规则。

## 独立练习（40 分钟）

从本目录沿用的两个目标开始。保留 lab_cli，把上例改写到第二个目标的位置，先用 app 保存原名 lab_cli_pf。预测配置消息，再运行下面的 PowerShell 命令。如果终端找不到 cmake，先在当前会话执行：

```powershell
Set-Alias cmake 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
```

```powershell
cmake -S . -B build
cmake --build build --config Debug --target lab_cli_pf
.\build\Debug\lab_cli_pf.exe
```

第二轮只把 set 里的值改为 lab_probe。重新配置，按预测修改构建命令和运行路径。验证标准是配置日志出现 Configuring target lab_probe，按新名构建成功，新程序输出 LAB v3!。记录两轮的配置消息与目标名。

英语练习：用 “is replaced by” 解释第二轮配置时 CMake 怎样处理 `${app}`。
