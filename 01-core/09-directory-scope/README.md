# 第 9 课：让子目录里的 LAB 程序单独开启诊断

> Each of the Directories in a source tree has its own variable bindings.

源码树里的每个目录都有自己的变量绑定。这里的 **binding** 是变量名与值的对应关系，**its own** 强调每个目录都有自己的作用域。处理子目录前，CMake 会用父目录当时的普通变量绑定初始化子目录。读取变量时，如果没有同名的普通变量绑定，就会查找缓存。本课用同一个名字观察这两种取值。[CMake 官方手册 · Directory Scope](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#variables)

## 目标与起点

用 40 分钟回答一个问题：怎样让缓存里的 `LAB_VERBOSE` 保持 `OFF`，同时让 `app` 里的程序打印诊断行？从本目录打开 PowerShell，先读顶层和 `app` 里的两份 `CMakeLists.txt`。

顶层的 `option()` 建立缓存开关。`add_subdirectory(app)` 会立即处理子目录的脚本，完成后才执行顶层下一行。子目录的 `if(LAB_VERBOSE)` 根据当时可见的值，决定是否为 `lab_probe` 定义同名的 C++ 宏。编译 `app/main.cpp` 时，`#ifdef LAB_VERBOSE` 决定是否把第二条输出编入程序。

## 先跑起始版本（10 分钟）

下面按本机的 Visual Studio 生成器操作。如果当前 PowerShell 找不到 cmake，先在当前窗口执行：

```powershell
$cmakeBin = 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin'
$env:Path = "$cmakeBin;$env:Path"
```

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -DLAB_VERBOSE=OFF
cmake --build build --config Debug --target lab_probe
.\build\app\Debug\lab_probe.exe
```

按出现顺序找出 `parent before`、`app entry`、`app target`、`parent after`。起始版本四处都读到 OFF，程序只输出 `LAB v3!`。目标现在声明在 app 子目录，Debug 程序也位于 build 下对应的子目录。

## 独立实验（20 分钟）

只编辑 `app/CMakeLists.txt`。在两条消息之间的注释处，用学过的 `set()` 将普通变量 `LAB_VERBOSE` 设为 `ON`。写下对四条配置消息的预测，然后复用同一个构建目录：

```powershell
cmake -S . -B build
Select-String -Path build/CMakeCache.txt -Pattern '^LAB_VERBOSE:BOOL='
cmake --build build --config Debug --target lab_probe
.\build\app\Debug\lab_probe.exe
```

成功时，只有 `app target` 显示 ON，回到父目录后仍显示 OFF。缓存行仍为 `LAB_VERBOSE:BOOL=OFF`，程序却多出一行 `Diagnostics enabled`。

## 用证据解释（10 分钟）

记录哪两条消息能证明赋值生效及其作用范围。把新增的 `set()` 移到 `endif()` 后面，先预测再重新配置、构建并运行。结合 `if()` 执行的时机解释结果。

英语练习：用 **has its own** 写一句话，解释为什么父目录最后仍读到 OFF。
