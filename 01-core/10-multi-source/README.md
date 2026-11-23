# 第 10 课：把诊断输出拆到第二个源文件

> Add an executable target called `<name>` to be built from the source files listed in the command invocation.

创建名为 `<name>` 的可执行目标，用这次调用中列出的源文件来构建它。**built from** 表示“由……构建”，**listed in the command invocation** 表示“在这次命令调用中列出”。读 `add_executable(lab_probe main.cpp)` 时，先找目标名 `lab_probe`，再看它后面列了哪些源文件。这里只有 `main.cpp`。[CMake 官方手册 · add_executable](https://cmake.org/cmake/help/latest/command/add_executable.html#normal-executables)

## 跑通起点（5 分钟）

**打卡版本复查。** 当前源码已完成拆分，`app/CMakeLists.txt` 中的 `set(LAB_VERBOSE ON)` 已注释。用下面的命令从全新构建目录运行时，默认只输出 `LAB v3!`。要验证拆分后的两行输出，执行 `cmake -S . -B build -DLAB_VERBOSE=ON`，再构建并运行。下文记录的是本课起始版本和练习过程。

在本课目录打开 PowerShell。沿用第 9 课的行为，顶层缓存选项为 OFF，`app` 子目录把普通变量设为 ON，给 `lab_probe` 定义了编译宏 `LAB_VERBOSE`。本课要保留两行输出，让 `main.cpp` 管程序入口，让新的 `diagnostics.cpp` 管诊断输出。

如果当前窗口找不到 `cmake`，先把本机 Visual Studio 自带的 CMake 加入当前窗口的 PATH。

```powershell
$cmakeBin = 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin'
$env:Path = "$cmakeBin;$env:Path"
```

```powershell
cmake -S . -B build -G "Visual Studio 18 2026"
cmake --build build --config Debug --target lab_probe
.\build\app\Debug\lab_probe.exe
```

第一条命令读取两层 CMake 脚本并生成 VS 工程，第二条实际编译并链接 Debug 目标。第三条运行刚生成的程序，应依次看到 `LAB v3!` 和 `Diagnostics enabled`。后面两次故意失败的构建只读日志，等最后一次构建成功后再运行程序。

## 动手拆分，读两次失败日志（25 分钟）

1. 打开已给出的 `app/diagnostics.h`。`void print_diagnostics();` 是函数声明，告诉编译器怎样调用它。在同目录新建 `diagnostics.cpp`，包含 `"diagnostics.h"` 和 `<iostream>`，写出函数定义。把 `main.cpp` 里从 `#ifdef LAB_VERBOSE` 到 `#endif` 的整段移进函数体。在 `main()` 原位置写一行 `print_diagnostics();`，保留第一行输出。此时先让 `main.cpp` 只包含原有的 `<iostream>`，CMake 的源文件清单也暂时沿用起始版本。
2. 执行下面的构建命令。找到同时提到 `main.cpp`、行号和 `print_diagnostics` 的第一条错误。MSVC 通常报告 `C3861`。编译器单独处理 `main.cpp` 时，调用处看不到函数声明。把 `#include "diagnostics.h"` 加到 `main.cpp` 顶部，预测下一次构建会走到哪一步。
3. 重复同一条构建命令。这次找提到 `main.obj` 和 `print_diagnostics` 的 `LNK2019`。这说明调用已编译成目标文件，链接器在本次链接的输入里却找不到函数定义。即使 `diagnostics.cpp` 已经写好并保存在磁盘上，当前的目标清单也只会安排编译 `main.cpp`。

```powershell
cmake --build build --config Debug --target lab_probe --verbose
```

## 修复并验收（10 分钟）

读回开头的英文，在 `app/CMakeLists.txt` 里找出 `lab_probe` 的源文件清单，自己补上实现文件。这里的源文件路径相对于 `app/CMakeLists.txt` 所在的目录。然后在课时目录执行：

```powershell
cmake -S . -B build
cmake --build build --config Debug --target lab_probe --verbose
.\build\app\Debug\lab_probe.exe
```

重新生成后，构建日志应能找到对 `diagnostics.cpp` 的编译和成功的链接。两份源文件属于同一个目标，所以已有的目标编译宏也适用于新文件。运行时应恢复起点的两行输出：

```text
LAB v3!
Diagnostics enabled
```

在自己的记录里，各摘一条能定位故障的原始日志，写明失败阶段和让它通过的那一处改动。最后用 **is built from** 写一句英文，说明修复后的 `lab_probe` 由哪两个源文件构建。
