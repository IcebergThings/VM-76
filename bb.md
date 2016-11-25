Bulletin Board
==============

printf
------

有些数据类型确实会让人不知道怎么在printf里表达。

| 数据类型 | printf |
| -------- | ------ |
| void* | %p |
| size_t | %zu |
| ptrdiff_t | %td |

这些提交消息十分滑稽
--------------------

下面是按照时间从新到旧的我认为滑稽的提交消息。如果这些并不合你口味，`git log --oneline`里总会有你喜欢的。

- 619b65b：以最大172fps的速度画出了256*256个方块！！在我无数次git stash之后，画这么大规模方块的速度终于从3fps提高到172fps！
- 0096b97：大战深度测试300回合
- eedf10d：玄学小优化，能加个2-3fps
- 51e4eda：纹理加载成功！！！哈哈哈哈哈哈哈哈哈，打个tag庆祝一下如何？
- 1f5aad7：修复了「上面和下面这堆东西全放在一个函数里，真节约啊」
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
- 03e6185：Fv*k the windoges
- 3dde095：Get rid of C

[贡献者列表](https://github.com/IcebergOS/VM-76/graphs/contributors)里可以看到鬼畜的统计数据。

为什么会有make.rb？
-------------------

~~make.rb自古以来就是VM/76神圣而不可侵犯的程序。~~

make.rb起初是为了让Ruby在Windoge上顺利运行而诞生的。现在改用纯C++了，照理来说它也没什么用了，为什么还放着呢？

这个问题的答案在每次我思考上述问题，决定取消make.rb的构建系统而审查Makefile时会自然显现。

为什么这个世界上就没有一个更灵活的构建系统呢？

—— @satgo1546
