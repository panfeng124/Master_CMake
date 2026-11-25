# 第 12 课：让静态库传递头文件搜索路径

> PUBLIC and INTERFACE items will populate the INTERFACE_INCLUDE_DIRECTORIES property of <target>.

出处：[CMake 官方文档 · target_include_directories](https://cmake.org/cmake/help/latest/command/target_include_directories.html)

**populate the … property** 意为“把给定的值写入……属性”。这里写入的是目录。编译链接了 lab_core 的目标时，CMake 会把库的这项接口属性用于生成编译参数。

## 目标

让库实现和应用都能写 `#include <lab/diagnostics.h>`，只在库目标上声明一次搜索路径。上节课两个源文件与头文件同目录，用双引号包含时编译器就能找到它。移动头文件后，构建规则要告诉编译器从哪里开始找。

## 动手与验证（40 分钟）

1. 阅读和预测（5 分钟）。在本课目录查看两个 C++ 文件。库和应用各自在哪一步需要头文件？
2. 调整布局（10 分钟）。把 app/diagnostics.h 移到 app/include/lab/diagnostics.h。把两个源文件的包含都改为 `#include <lab/diagnostics.h>`。搜索根应是 app/include，因为编译器会在它后面拼上 lab/diagnostics.h。
3. 制造一次可解释的失败（10 分钟）。在 app/CMakeLists.txt 的 add_library 后写 `target_include_directories(lab_core PRIVATE include)`。相对路径以当前源码目录 app 为基准。在本课根目录运行：

   ```powershell
   cmake -S . -B build
   cmake --build build --config Debug --target lab_core
   cmake --build build --config Debug --target lab_probe
   ```

   预期库能编译，随后编译 main.cpp 时提示找不到 lab/diagnostics.h。记下第一条涉及源文件的报错。
4. 修复和验证（15 分钟）。把刚加的 PRIVATE 改为 PUBLIC，保留原来的 target_link_libraries(lab_probe PRIVATE lab_core)。重新运行上面的配置和构建命令，再运行程序：

   ```powershell
   $exe = Get-ChildItem build -Recurse -Filter lab_probe.exe | Select-Object -First 1
   & $exe.FullName
   ```

   成功时依次看到 `LAB v3!` 和 `Diagnostics enabled`。PUBLIC 让 include 既用于编译库，又进入库的接口。应用私有链接该库，仍会消费这个接口。

**英语练习**：用 “populate the … property” 解释为什么把 PRIVATE 换为 PUBLIC 后，main.cpp 能通过编译。
