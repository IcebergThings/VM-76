# VM-76
大概是一个沙盒游戏与对应的引擎。

## 运行的方法

### 需要的库
* [GLFW](http://www.glfw.org/)
* [GLEW](http://glew.sourceforge.net/)
* [GLM](http://glm.g-truc.net/)
* [PortAudio](http://www.portaudio.com)
* [libogg & libvorbis](https://xiph.org/downloads/)（直接使用的是其中的libvorbisfile）
* [SOIL](http://www.lonesock.net/soil.html)

### Windows :dog: Windoge
对于Windows用户，需要预先配置好库文件目录。做法是：

* 在每个msw.makefile旁放置windows_flags.txt和windows_libs.txt两个文件，内容分别为编译和链接的附加参数。
* 在项目根目录下放置windows_path.bat指定%PATH%环境变量。

这些是我电脑上的编译参数，更新于 426029b。仅供参考，但保证能在我的电脑上编译通过。使用时需要根据自己的情况修改其中的路径，因为你肯定不会有个Y:盘的。

要进行自动编译，还需要MinGW中的mingw32-make工具。在项目根目录执行`mingw32-make run`就可以运行了。

本项目设定的默认编译器是clang。如果你没有，可以去[下载](http://releases.llvm.org/download.html)；如果你想使用MinGW GCC，可以在用如下方式运行make：`mingw32-make run GCC=1`。但是，无论编译使用的是clang还是GCC，链接时总是会使用GCC。

不支持使用Visual Studio中的NMAKE工具自动编译，因为它的语法与MinGW/GNU make不兼容。

#### ASM76/build/windows_flags.txt
```
-isystem Y:\glm-0.9.7.5
```

#### ASM76/build/windows_libs.txt
*(空)*

#### VMDE/build/windows_flags.txt
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

#### VMDE/build/windows_libs.txt
```
Y:\SOIL\lib\libSOIL.a
Y:\glew-2.0.0-src\src\glew.a
-LY:\project76-lib
-LY:\glfw-3.2.bin.WIN32\lib-mingw-w64
-lglfw3dll
-lopengl32
-lportaudio_x86
-lvorbisfile
```

#### VMGS/build/windows_flags.txt
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

#### VMGS/build/windows_libs.txt
```
Y:\SOIL\lib\libSOIL.a
Y:\glew-2.0.0-src\src\glew.a
-LY:\project76-lib
-LY:\glfw-3.2.bin.WIN32\lib-mingw-w64
-lglfw3dll
-lopengl32
-lportaudio_x86
-lvorbisfile
```

#### windows_path.bat
```batch
path Y:\mingw32\bin;Y:\project76-lib
```

### *nix (macOS & Linux) :dog: Dogix
通常在安装完库之后，无需配置即可编译。

在项目根目录执行`make run`就可以运行了。

## 关于许可证
* 本游戏及引擎使用AGPL授权，动态链接视为对代码的引用。
* 如需以其它协议个人/商业使用，请与我们联系。
* Fork该repository不改变本协议与附加要求。
* 我们（原作者）保留对该游戏/引擎的一切权利。
* 本软件/游戏/引擎的名称与商标为原作者所有，禁止随意使用。

## STAFF
\* 此列表未完待续。

* Windows平台的支持
	* @satgo1546
* macOS平台的支持
	* @bobcao3
