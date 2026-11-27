# 第 14 课：用 INTERFACE 目标提供纯头文件版本库

> “may specify usage requirements for dependents but does not compile sources and does not produce a library artifact on disk.”
>
> 节选自 [CMake 官方文档 · add_library · Interface Libraries](https://cmake.org/cmake/help/latest/command/add_library.html#interface-libraries)。

这句话的主语是接口库目标。它可以声明依赖方使用它时需要的条件，自身不编译源文件，也不在磁盘上生成库文件。**usage requirements** 在本课具体指编译器查找版本头文件所需的目录。**dependents** 指依赖这个目标的一方，本课就是应用 `lab_probe`。**artifact on disk** 指构建后能在磁盘上找到的产物，例如上一课的静态库。

## 本课目标

让应用通过 `#include <lab/version.h>` 调用纯头文件函数，并用一次编译失败、一次成功运行和一份产物清单说明接口目标起了什么作用。

起点是上一课可运行的 LAB 程序。新增的 `app/version/include/lab/version.h` 已定义 `lab::version()`，返回字符串 `"v4"`。它有独立的目录，这样上一课由 `lab_core` 提供的 `app/include` 找不到它。头文件里的函数定义会随包含它的源文件一起编译。

## 从头文件到应用

在 `app/CMakeLists.txt` 中，下面两行分别建立接口库目标、声明它提供给依赖方的头文件搜索目录。

```cmake
add_library(lab_version INTERFACE)
target_include_directories(lab_version INTERFACE version/include)
```

第一个 `INTERFACE` 指库的类型，第二个指使用范围。相对目录按当前 `app/CMakeLists.txt` 所在位置解析。编译器拿到这个目录后，把包含指令里的 `lab/version.h` 接在它后面，便能定位文件。[CMake 官方文档 · target_include_directories](https://cmake.org/cmake/help/latest/command/target_include_directories.html)

在声明 `lab_probe` 之后，把 `lab_version` 加入应用现有的 `target_link_libraries(lab_probe PRIVATE lab_status)` 的 `PRIVATE` 列表。CMake 据此把版本库的搜索目录加到编译 `main.cpp` 的命令里。最终链接仍然使用原有静态库，版本函数的定义已由编译器读入。

## 独立练习（40 分钟）

在本课目录打开 PowerShell。如果找不到 `cmake`，可以先在当前窗口运行：

```powershell
Set-Alias cmake 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
```

1. **读原文，作预测（5 分钟）。** 阅读版本头文件。根据 *does not produce a library artifact on disk*，用一句英语预测构建后能否找到 `lab_version.lib`。
2. **制造一次可解释的失败（10 分钟）。** 在 `app/main.cpp` 包含 `<lab/version.h>`，把原先写死的欢迎语改为输出 `"LAB "`、`lab::version()` 和 `"!\n"`。保留诊断调用。声明接口目标和搜索目录，暂时保留应用原有的链接列表。执行：

   ```powershell
   cmake -S . -B build
   cmake --build build --config Debug --target lab_probe --verbose
   ```

   配置和生成应成功，编译 `main.cpp` 时应报告找不到 `lab/version.h`。记下第一条相关错误。
3. **接上依赖并运行（15 分钟）。** 把 `lab_version` 加到应用的依赖列表，重新执行上面的配置和构建命令。从详细日志中找到编译 `main.cpp` 的命令，检查其中的 `version/include`（Windows 路径也可能显示为反斜杠）。运行本次构建的程序：

   ```powershell
   $probe = Get-ChildItem .\build -Recurse -Filter lab_probe.exe | Select-Object -First 1
   & $probe.FullName
   ```

   应依次看到 `LAB v4!` 和 `Diagnostics enabled`。
4. **核对产物（10 分钟）。** 执行下面的命令。指出哪两个目标生成了静态库，哪个目标提供了头文件搜索路径。结合编译日志，用一句话说明应用声明这条链接依赖带来了什么变化。

   ```powershell
   Get-ChildItem .\build -Recurse -File -Include *.lib,*.a | Select-Object -ExpandProperty Name
   ```
