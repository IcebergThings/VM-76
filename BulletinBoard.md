Bulletin Board
==============

请写可移植、可维护的代码，可以吗？
----------------------------------

### 不要用typedef定义结构体！

不要用typedef定义结构体！<br>
不要用typedef定义结构体！

这不仅会给你带来巨大的命名困难（一个结构体要两个名字——struct里的和typedef里的），还会导致你在使用的时候不知道这是什么玩意。请**直接**用struct定义结构体，有效避免起名困难。

另外，C和C++本身已经在全局的类型命名空间中放了许多东西，请不要再污染它了。

> ~~要优雅，不要污！~~

#### 真实案例

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

#### 参考资料

 + [Linux kernel coding style] Chapter 5: Typedefs

### 不要定义_t后缀！

_t后缀是POSIX标准所保留的。

如果一定要用（例如函数指针类型的定义），我自己一般用type_前缀。

### 不要相信time_t！

参见[time_t的说明](http://en.cppreference.com/w/c/chrono/time_t)。

> Although **not defined** by the C standard, this is **almost** always an integral value holding the number of seconds (not counting leap seconds) since 00:00, Jan 1 1970 UTC, corresponding to POSIX time.

[Linux kernel coding style]: https://www.kernel.org/doc/Documentation/CodingStyle

### 不要掺CRLF！

CR不是金坷垃，LF掺了CR，*nix下的工具都一脸懵逼了。

### ~~不要乱用C++类~~

来自一个学C/C++数月的人的建议：C太过神奇，C++太过奇异。在这个世界，还是尽量用C式C++（“C with namespaces”）为好。

### printf（log）

有些数据类型确实会让人不知道怎么在printf里表达。

| 数据类型 | printf |
| -------- | ------ |
| void* | %p |
| size_t | %zu |
| ptrdiff_t | %td |

### 不要乱写Makefile

仅供参考，因为我也不知道怎么写。

> "Suffix rules" are the old-fashioned way of defining implicit rules for
> 'make'.  Suffix rules are obsolete because pattern rules are more
> general and clearer.  They are supported in GNU 'make' for compatibility
> with old makefiles.

— 10.7 Old-Fashioned Suffix Rules

> Use the following 'make' variables to run those programs:
>
> $(AR) $(BISON) $(CC) $(FLEX) $(INSTALL) $(LD) $(LDCONFIG) $(LEX)
> $(MAKE) $(MAKEINFO) $(RANLIB) $(TEXI2DVI) $(YACC)

— 16.2 Utilities in Makefiles

这些提交消息十分滑稽
--------------------

下面是按照时间从新到旧的我认为滑稽的提交消息。如果这些并不合你口味，`git log --oneline`里总会有你喜欢的。

 - e49946a：摸鱼并提交
 - 1cdaa57：Merge! Merge! Merge!
 - a5d77bc：Merge! Though not working
 - ca04f2d：终于搞定啦！！！！！！！RenderChain能用啦，VISIBLE有用啦！！！！(顺便在demo里加入了
 - 4f7b620：终于会叫了
 - 1aa763d：添加UNUSED宏。GCC吵死了。
 - f59f9dc：吃屎了，忘记打引号
 - c8ebdce：添加了这玩意是怎么绘图的
 - f59e328：让extconf.rb更好看
 - f80f396：听说这破引擎已经能画三角形了？
 - a20c377：我非洲电脑的Windows编译速度提升了5倍！
 - c95a8c0：合并分支satgo1546-dev → master：提交完这些我就睡觉（FLAG）
 - 01b11dc、dc1abac、798983e、538cc0e：整理代码（四）、整理代码（三）、整理代码（二）、整理代码（一）
 - 166dd7a：Structure.md的第5行的圆太大了
     - 这个提交下面真的有个滑稽！
 - 74fddc3：加了个fps的功能，不做死直接改master了，回devbranch
 - 92ad155：More WTF stuff
 - 03e6185：Fv*k the windoges
 - 3dde095：Get rid of C

### 另一些滑稽的东西

> \* 我们只要在Audio里加入锯齿波、方波、余弦波、正切波、余切波、正割波、余割波、和弦、混合波、引力波之类的（有奇怪的东西混进来了<br />
> 就可以把audio.cpp和global.hpp的行数成倍增长(#滑稽)

[贡献者列表](https://github.com/IcebergOS/VM-76/graphs/contributors)里可以看到鬼畜的统计数据。
