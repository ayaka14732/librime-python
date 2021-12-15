# librime-python

**为什么你需要 librime-python？**

1\. Python 是解释执行的脚本语言，因此可以在 rime 输入法的基础上动态地添加功能，而无需重新编译源代码、重启输入法程序，甚至无需重新部署。

2\. 与 librime 本身使用的 C++ 语言相比，Python 提供了丰富的库函数，因此可以非常方便地拓展 rime 输入法的功能。Python 常见的应用场景包括字符串处理（自动纠错）、连接 HTTP 服务器（云输入）、深度学习（智能输入引擎）等。

## 一、示例

### 1.1. `simple_translator`

输出当前日期：[`python_simple_translator@datetime`](examples/datetime.simple_translator.py)

<img alt="python_simple_translator@datetime" src="images/datetime.png" height="75">

### 1.2. `translator`

汉语拼音（普通话）云输入：[`python_translator@putonghua_cloud_input`](examples/putonghua_cloud_input.translator.py)

<img alt="python_translator@putonghua_cloud_input" src="images/putonghua_cloud_input.png" height="75">

### 1.3. `simple_filter`

过滤拓展区汉字：[`python_simple_filter@charset`](examples/charset.simple_filter.py)

过滤前

<img alt="rime-loengfan" src="images/charset-0.png" height="75">

过滤后

<img alt="python_simple_filter@charset" src="images/charset-1.png" height="75">

### 1.4. `filter`

一个复杂的例子，删除长度为 2 的候选，并将长度为 3 的候选转为简体：[`python_filter@complex`](examples/complex.filter.py)

过滤前

<img alt="rime-cantonese" src="images/complex-0.png" height="75">

过滤后

<img alt="python_filter@charset" src="images/complex-1.png" height="75">

**FIXME**：由上图可以看出，长度为 2 的候选并没有被完全刪除，下一个版本修复。

## 二、用法

| 在输入方案中指定的组件名称 | 对应的 Python 脚本文件名 |
| :- | :- |
| `python_simple_translator@<name>` | `<name>.simple_translator.py` |
| `python_translator@<name>` | `<name>.translator.py` |
| `python_simple_filter@<name>` | `<name>.simple_filter.py` |
| `python_filter@<name>` | `<name>.filter.py` |

在 Python 脚本文件中按照下面的要求定义 `rime_main` 函数，即可实现相应组件的功能。

### 2.1. `simple_translator`

`simple_translator` 要求 `rime_main` 函数的输入为字符串，输出为字符串的列表。

其中，输入的字符串为待翻译的输入码；输出的列表为多个翻译结果，每一项表示一个翻译结果。

```python
def rime_main(s: str) -> Optional[List[str]]:
    ...
```

### 2.2. `translator`

`translator` 要求 `rime_main` 函数的输入为 `TranslatorQuery`，输出为 `TranslatorAnswer` 的列表。

其中，输入的 `TranslatorQuery` 为待翻译的內容，其中包含待翻译的输入码等多项信息；输出的列表为多个翻译结果，每一项为一个 `TranslatorAnswer`，其中包含翻译结果、`comment` 等多项信息。

```python
from rimeext import TranslatorQuery, TranslatorAnswer

def rime_main(translator_query: TranslatorQuery) -> Optional[List[TranslatorAnswer]]:
    ...
```

### 2.3. `simple_filter`

`simple_filter` 要求 `rime_main` 函数的输入为字符串，输出为布尔值。

其中，输入的字符串为翻译结果；输出的布尔值表示是否保留该项翻译结果，为真则保留，否则删除。

```python
def rime_main(s: str) -> bool:
    ...
```

### 2.4. `filter`

`filter` 要求 `rime_main` 函数的输入为 `FilterQuery`，输出为 `FilterAnswer`。

其中，输入的 `FilterQuery` 为翻译结果，其中包含翻译结果文本、`comment` 等多项信息；输出的 `FilterAnswer` 有三种：跳过（保持该翻译结果不变）、删除（将该翻译结果从候选列表中移除）和重写（修改翻译结果中的某些信息）。

```python
from rimeext import FilterQuery, FilterAnswer

def rime_main(filter_query: FilterQuery) -> FilterAnswer:
    ...
```

## 三、安裝

安裝 librime-python 分为两部分：Python 部分需要安装 rimeext 库，C++ 部分需要编译并安装带有 pythonext 插件的 librime。

### 3.1. Arch Linux

Python 部分：

```sh
cd python
pip install .
```

C++ 部分：

```sh
cd archlinux
makepkg -si
```

在编译时，Boost.Python 1.76.0 可能会报错，经过判断这是 Boost.Python 库本身的问题（见 [boostorg/python issue #359](https://github.com/boostorg/python/issues/359)），这时自行修改两个系统头文件即可正常编译。

**FIXME**：在 `PKGBUILD` 中，编译选项 `-DBUILD_MERGED_PLUGINS=Off` 被修改为 `On`，这是因为如果不修改，用户在 Python 脚本中 `import math` 会产生如下 `ImportError`。

```
ImportError: /usr/lib/python3.9/lib-dynload/math.cpython-39-x86_64-linux-gnu.so: undefined symbol: PyLong_AsLongLongAndOverflow
```

下一个版本修复。

### 3.2. Windows

首先要保证Python运行平台与librime(thirdparty、boost、librime主程序)一致，如librime 以Win32编译时，Python须为32bit，且PyBind安装也应为32bit.

然后进行Python部份的安装

```sh
cd python
pip install .
pip install pybind
```

在把该项目置于librime/plugins目录下后，在该项目录下需有CMakeLists.txt 因此先用CMD建立链接

```sh
mklink CMakeLists.txt cpp\CMakeLists.txt
mklink /D src cpp\src
```

在 librime 项目下使用build.bat编译即可

```sh
build.bat # 32位
build.bat x64 #64位
```

## 四、用户 Q&A

**Q1. Python 的脚本文件放在哪里？**

rime 用户文件夹或 rime 系统文件夹。

**Q2. 我更新了 Python 脚本，应如何通知 rime 输入法？**

重新部署，或者切换到其他方案再切回来。

**Q3. 我在某个方案中使用了 librime-python，但一选择那个方案，输入法就会崩溃，怎么知道哪里出了问题呢？**

查看 rime 的错误日志。

**Q4. 不同的 Python 脚本文件中的变量会相互冲突吗？**

**FIXME**：目前会相互冲突，因为 pybind11 不支持 [sub-interpreter](https://pybind11.readthedocs.io/en/stable/advanced/embedding.html#sub-interpreter-support)，下一个版本修复。

**Q5. `rime_main` 函数会被调用几次？在什么时候被调用呢？**

**TODO**：下一个版本补充。

**Q6. 我想实现一个计数器的功能，应该如何使用全局变量？**

**TODO**：下一个版本补充。

## 五、开发指南

### 5.1. 项目原理

利用 [pybind11](https://pybind11.readthedocs.io/en/stable/index.html)。pybind11 提供了对 [Python/C API](https://docs.python.org/3/c-api/) 的 C++ 封装。对本项目而言，pybind11 提供的最重要的功能是在 C++ 代码中调用 Python 解释器。

### 5.2. 项目结构

分为 Python 部分和 C++ 部分。Python 部分定义了数个类，但并不是对 rime C++ API 的绑定，而是 librime-python 自定义的简化版的接口，因此不依赖于 C++ 代码。

### 5.3. 代码准则

如果你使用任何编辑器，请确保编辑器会读取项目根目录的 `.clang-format`，并在保存时自动格式化代码。

本项目是 Python 插件，因此遵守 Python 之禅：Explicit is better that implicit。

特别地，对于字符串类型，使用 `std::string` 的写法，而不使用 rime 定义的别名 `rime::string`，这是为了保证与 Python API 交互部分的代码不会引起误解。

由于本项目是输入法插件，当插件在运行时产生异常时，异常必须在插件内部处理完毕，不应该使输入法崩溃，且结果必须等价于插件不存在。

使用正确的现代 C++17 语法。

## 六、开发 Q&A

**Q1. Python 部分使用了哪些外部库？**

Python 部分没有使用外部库，只使用了标准库，但是用户可以安装并引入其他外部库。

**Q2. C++ 部分使用了哪些外部库？**

使用了 pybind11。

**Q3. librime-python 用的是 C++17，但 librime 是 C++14，是否会出现问题？**

[不会](https://stackoverflow.com/a/49119902)。

**Q4. Python 部分是对 librime API 的绑定吗？**

不是。Python 部分是独立于 C++ 的。是 librime-python 自定义的简化版的接口。

**Q5. 未来还需要做什么？**

1\. 修复上文提到的 bug；

2\. 为 librime-python 自定义的简化版的接口添加更多功能，如读取输入法开关状态等；

3\. 其他功能添加和优化。
