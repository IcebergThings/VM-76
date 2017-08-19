//=============================================================================
// ■ windoge_run.cpp
//-----------------------------------------------------------------------------
//   Windoge上，为了解决不能在命令行下指定GPU运行的问题，
//   请在需要使用非默认的GPU运行VM/76的时候使用这个程序。
//   例如，通过右键菜单—“用图形处理器运行”菜单。
//   这个程序是单独编译的，你可以用任何能用的编译器编译它，比如cl.exe。
//   PS：经测试，在简体中文版Windows上，用cl.exe编译这个程序
//       还是会出现喜闻乐见的编码错误：
//           warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。
//           请将该文件保存为 Unicode 格式以防止数据丢失
//=============================================================================

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char cmd[1024];
char buf[1000];
const char* presets[] = {
	"make run",
	"make debug",
};

int main(int argc, char** argv) {
	system("title Windoge run");
	system("color 2f");
	puts("=-=- Windoge run -=-=");
	for (size_t i = 0; i < sizeof(presets) / sizeof(char*); i++) {
		printf("[%zu] %s\n", i, presets[i]);
	}
	printf("[ ] make ");
	fgets(buf, sizeof(buf), stdin);
	if (buf[1] == '\n') {
		if (buf[0] >= '0' && buf[0] <= '9') {
			strcpy(cmd, presets[buf[0] - '0']);
		} else {
			strcpy(cmd, "echo Wrong choice!");
		}
	} else {
		strcpy(cmd, "make ");
		strcpy(cmd + 5, buf);
	}
	system("cls");
	system(cmd);
	system("pause");
	return 0;
}
