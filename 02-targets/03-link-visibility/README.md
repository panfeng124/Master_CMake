# 第 13 课：用三个目标验证 PUBLIC 与 PRIVATE 链接

> Usage requirements from linked library targets will be propagated. Usage requirements of a target's dependencies affect compilation of its own sources.

[CMake 官方文档 · `target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html)

被链接的库目标会传递自身的使用要求。一个目标的依赖所带来的使用要求，会影响这个目标自己的源文件如何编译。

- **usage requirements** 是使用库时需要的构建设置。本课观察的是 `lab_core` 通过 `PUBLIC include` 提供的头文件搜索路径。
- **will be propagated** 是“会被传递”。直接链接 `lab_core` 的 `lab_status` 会获得这条路径。`lab_status` 选用的链接作用域决定这条路径能否继续传给 `lab_probe`。

**英语练习**：在 `PRIVATE` 实验后，用 “usage requirements” 写一句英文，说明为什么 `status.cpp` 可以包含 `lab/diagnostics.h`，而 `main.cpp` 报找不到这个头文件。

## 工程问题

把诊断调用封装在一个中间库里，掌握怎样让应用只依赖中间库的接口。依赖方向如下。

```text
lab_probe（可执行程序） → lab_status（静态库） → lab_core（静态库）
```

`lab_status` 无论用 `PUBLIC` 还是 `PRIVATE` 链接 `lab_core`，编译自己的 `status.cpp` 时都能使用 `lab_core` 的头文件搜索路径。`PUBLIC` 会把该编译要求继续传给 `lab_probe`。选用 `PRIVATE` 时，搜索路径止于 `lab_status`。静态库只归档自己的目标文件，`lab_status` 里对 `print_diagnostics()` 的引用要等到链接可执行程序时才解析，因此 `PRIVATE` 模式下最终链接仍需要 `lab_core`。[CMake 构建系统手册 · 静态库](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#static-libraries)

## 动手与验证（40 分钟）

以下命令在本课目录中运行。若当前 PowerShell 找不到 `cmake`，可在同一窗口先运行下面这行，使用这台机器已安装的版本。

```powershell
Set-Alias cmake 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
```

1. **读原文并预测（5 分钟）。** 起点是第 12 课的 `lab_core` 和 `lab_probe`。画出三目标关系，标出 `app/include` 在两种作用域下能传到哪里。
2. **接上中间库（15 分钟）。** 在 `app/` 新建 `status.h` 和 `status.cpp`，声明并实现 `void print_status()`。实现包含 `"status.h"` 和 `<lab/diagnostics.h>`，调用 `print_diagnostics()`。在 `app/CMakeLists.txt` 中用 `add_library(lab_status STATIC status.cpp)` 建立目标，让它 `PUBLIC` 链接 `lab_core`，再让 `lab_probe` 只 `PRIVATE` 链接 `lab_status`。让 `main.cpp` 包含 `"status.h"`，保留欢迎语并改调 `print_status()`。暂时保留原来的 `<lab/diagnostics.h>`，用它检查头文件路径能否传到应用。

   ```powershell
   cmake -S . -B build-public
   cmake --build build-public --config Debug --target lab_probe --verbose
   ```

   预期 `status.cpp` 和 `main.cpp` 都能编译。用 Visual Studio 生成器时，运行 `& .\build-public\app\Debug\lab_probe.exe`，应依次看到 `LAB v3!` 和 `Diagnostics enabled`。

3. **缩小依赖的可见范围（10 分钟）。** 将 `lab_status` 到 `lab_core` 这一条链接的作用域改为 `PRIVATE`，暂时保留应用里的两个 `#include`。

   ```powershell
   cmake -S . -B build-private
   cmake --build build-private --config Debug --target lab_status --verbose
   cmake --build build-private --config Debug --target lab_probe --verbose
   ```

   预期 `lab_status` 能构建，随后 `main.cpp` 在编译阶段报找不到 `lab/diagnostics.h`。从日志记下源文件、头文件和第一条错误。

4. **验证最终链接（10 分钟）。** 只移除 `main.cpp` 中临时保留的 `<lab/diagnostics.h>`，让应用通过 `status.h` 调用 `print_status()`。重跑下面的构建，找出最终链接命令中的 `lab_status.lib` 和 `lab_core.lib`（使用 MinGW 时查找对应的 `.a` 文件）。

   ```powershell
   cmake --build build-private --config Debug --target lab_probe --verbose
   & .\build-private\app\Debug\lab_probe.exe
   ```

   上面的运行路径适用于 Visual Studio 生成器。在单配置生成器下，通常是 `build-private/app/lab_probe.exe`。程序应再次输出同样两行。用这次的链接命令和上一步的编译错误解释 `PRIVATE` 对这条依赖的影响。
