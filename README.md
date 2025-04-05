# Krkr 相关研究

与 KiriKiri 引擎相关的一些实用工具。注：这些工具可能不适用于大多数人，仅供研究用。

## LocateFuncAddr

查询导入函数的指针地址，适用于 krkrz，未测试 krkr2 及其他版本。

使用本工具需要对 krkr 的插件初始化逻辑有一定了解，见 `tp_stub.h`。

请先关闭 ASLR，否则地址可能会变动。

使用方法：拖动主程序到 KrkrLocateFuncAddrLoader.exe（保证同一目录下有 KrkrLocateFuncAddr.dll）。
