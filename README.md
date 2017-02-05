# VM-76
大概是一个沙盒游戏与对应的引擎。

## 运行的方法
在项目根目录执行`make run`（Windows上为`mingw32-make run`）。

### Windows :dog: Windoge
对于Windows用户，需要预先配置好库文件目录。做法是在每个msw.makefile旁放置windows_flags.txt和windows_libs.txt两个文件，内容分别为编译和链接的附加参数。

这些是我电脑上的编译参数，更新于 d40f7b5。仅供参考，但保证能在我的电脑上编译通过。使用时需要根据自己的情况修改其中的路径，因为你肯定不会有个Y:盘的。

* VMDE/build/windows_flags.txt
	```
	-DGLFW_DLL
	-DGLEW_STATIC
	-isystem Y:\glfw-3.2.bin.WIN32\include
	-isystem Y:\glew-2.0.0\include
	-isystem Y:\glm-0.9.7.5
	-isystem Y:\portaudio\include
	-isystem Y:\libogg-1.3.2\include
	-isystem Y:\libvorbis-1.3.5\include
	-isystem Y:\SOIL\src
	```
* VMDE/build/windows_libs.txt
	```
	Y:\SOIL\lib\libSOIL.a
	Y:\glew-2.0.0-src\src\glew.a
	-LY:\bin
	-LY:\glfw-3.2.bin.WIN32\lib-mingw-w64
	-lglfw3dll
	-lopengl32
	-lportaudio_x86
	-lvorbisfile
	```
* VMGS/build/windows_flags.txt
	```
	-DGLFW_DLL
	-DGLEW_STATIC
	-isystem Y:\glfw-3.2.bin.WIN32\include
	-isystem Y:\glew-2.0.0\include
	-isystem Y:\glm-0.9.7.5
	-isystem Y:\portaudio\include
	-isystem Y:\libogg-1.3.2\include
	-isystem Y:\libvorbis-1.3.5\include
	-isystem Y:\SOIL\src
	-I..\VMDE
	```
* VMGS/build/windows_libs.txt
	```
	Y:\SOIL\lib\libSOIL.a
	Y:\glew-2.0.0-src\src\glew.a
	-LY:\bin
	-LY:\glfw-3.2.bin.WIN32\lib-mingw-w64
	-lglfw3dll
	-lopengl32
	-lportaudio_x86
	-lvorbisfile
	```

### *nix (macOS & Linux) :dog: Dogix

## 关于许可证
* 本游戏及引擎使用AGPL授权，动态链接视为对代码的引用
* 如需以其它协议个人/商业使用，请与我们联系
* Fork该repo不改变本协议与附加要求
* 我们（原作者）保留对该游戏/引擎的一切权利
* 本软件/引擎的名称与商标为原作者所有，禁止随意使用

## STAFF
\* 此列表未完待续。

* Windows平台的支持
	* @satgo1546
* macOS平台的支持
	* @bobcao3
