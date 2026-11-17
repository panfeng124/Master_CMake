# 第 1 课 · 从 main.cpp 到 lab_cli.exe（40 分钟）

> Add an executable target called `<name>` to be built from the source files listed in the command invocation.
>
> The `<name>` corresponds to the logical target name and must be globally unique within a project.
>
> 节选自本机 CMake 命令帮助 `cmake --help-command add_executable`，Normal Executables 一节。

**怎么读这两句？** 第一句的主干是 *Add an executable target*（添加一个可执行目标）。后面的 *called …* 交代它叫什么，*to be built from …* 交代将用哪些源文件来构建它。第二句说，这个名字对应目标的逻辑名称，在整个项目中必须唯一。

拿 `add_executable(lab_cli main.cpp)` 对照：*called* 后的名字是 `lab_cli`；*the source files listed in the command invocation* 指这次调用括号里列出的源文件，这里只有 `main.cpp`。英语里说 *build A from B*，就是“用 B 构建 A”；换成被动语态，便是 *A is built from B*。

**读完试一试。** 如果再写一行 `add_executable(lab_cli_pf main.cpp)`，哪一项必须换名字，哪一项可以复用？从第二句找出依据，再用 *Both targets are built from …* 说说两个目标和源文件的关系。

本课目标是亲手构建 LAB 的第一个命令行程序，并能从日志指出哪一步生成构建规则、哪一步生成可执行文件。

## 写下第一个目标（15 分钟）

在本目录创建 `main.cpp`，引入 `<iostream>`，在 `int main()` 中用 `std::cout` 打印一行 `LAB ready`。再创建 `CMakeLists.txt`，写入下面三行。

```cmake
cmake_minimum_required(VERSION 3.20)
project(Lab LANGUAGES CXX)
add_executable(lab_cli main.cpp)
```

第一行声明项目要求 CMake 3.20 或以上。第二行给项目命名为 Lab，并启用 C++。第三行声明一个名叫 lab_cli 的可执行目标，其源文件是 main.cpp。目标名随后可传给构建命令。

## 生成规则，构建并运行（15 分钟）

在 PowerShell 进入练习目录，使用本机 CLion 自带的工具。路径设置只在当前 PowerShell 会话中生效。

```powershell
Set-Location 'F:\PF\LAB\Master_CMake\01-core\01-first-target'
$clion = 'C:\Program Files\JetBrains\CLion 2025.3.2\bin'
$env:Path = "$clion\cmake\win\x64\bin;$clion\ninja\win\x64;$clion\mingw\bin;$env:Path"
```

依次运行，每一步成功后再继续。

```powershell
cmake -S . -B build -G Ninja
cmake --build build --target lab_cli
.\build\lab_cli.exe
```

1. **配置和生成**。第一条命令指定源码目录（`-S .`）、构建目录（`-B build`）和生成器（`-G Ninja`）。CMake 在配置阶段读取项目描述、识别编译器并建立目标，在生成阶段把构建规则写入 `build/build.ninja`。观察日志中的 `Configuring done` 和 `Generating done`。
2. **构建**。第二条命令让 Ninja 按规则构建 lab_cli。首次构建时，`Building CXX object` 表示编译 main.cpp 得到目标文件，`Linking CXX executable` 表示链接生成 lab_cli.exe。
3. **运行**。第三条命令启动刚生成的程序。预期看到一行 `LAB ready`。

## 独立验证（10 分钟）

只把 main.cpp 中的输出改为 `LAB v2` 并保存。先预测此时直接运行已有的 exe 会看到什么，再运行验证。接着只执行构建和运行两条命令，观察哪些构建步骤再次出现。

成功标准是你能先后观察到旧输出和新输出，并能用日志解释：修改源码后，究竟在哪一步得到了新的 lab_cli.exe？
