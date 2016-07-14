Bulletin Board
==============

请写可移植、可维护的代码，可以吗？

不要用typedef定义结构体！
-------------------------

不要用typedef定义结构体！<br>
不要用typedef定义结构体！

这不仅会给你带来巨大的命名困难（一个结构体要两个名字——struct里的和typedef里的），还会导致你在使用的时候不知道这是什么玩意。请**直接**用struct定义结构体，有效避免起名困难。

另外，C和C++本身已经在全局的类型命名空间中放了许多东西，请不要再污染它了。

> ~~要优雅，不要污！~~

### 真实案例

```c
typedef struct _VMDE_State_t {
	bool freeze;
} VMDE_State_t;

typedef struct _VMDE_t {
	VMDE_State_t* States;
	long framecount;
	long millis;
	int fps;
} VMDE_t;
```

**不要这样用！**此案例除了上述讲的问题之外，还乱用_t后缀，参见下一条。

### 参考资料

 + [Linux kernel coding style] Chapter 5: Typedefs

不要定义_t后缀！
----------------

_t后缀是POSIX标准所保留的。

如果一定要用（例如函数指针类型的定义），我自己一般用type_前缀。

不要相信time_t！
----------------

参见[time_t的说明](http://en.cppreference.com/w/c/chrono/time_t)。

> Although **not defined** by the C standard, this is **almost** always an integral value holding the number of seconds (not counting leap seconds) since 00:00, Jan 1 1970 UTC, corresponding to POSIX time.

[Linux kernel coding style]: https://www.kernel.org/doc/Documentation/CodingStyle

不要掺CRLF！
------------

CR不是金坷垃，LF掺了CR，*nix下的工具都一脸懵逼了。
