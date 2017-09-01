//=============================================================================
// ■ asm76util.cxx
//-----------------------------------------------------------------------------
//   独立的ASM76实用工具。
//=============================================================================

#include "ASM76.hpp"

void help(const char* argv0 = "asm76util") {
	printf("Usage: %s <command> <argument 1> [argument 2]\n\n", argv0);
	puts("c: assembly (1) -> object code (2)");
	puts("d: assembly (2) <- object code (1)");
	puts("e: print (1) to stderr and exit 1");
	puts("r: run (1) with characters in (2) meaning the following:");
	puts("    d: run a debug session");
	puts("    r: dump registers in the end");
	puts("    m: dump memory in the end");
}

void e(const char* msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

char* read_file(const char* filename) {
	FILE* f = fopen(filename, "rb");
	if (!f) e("cannot open the input file");
	if (fseek(f, 0, SEEK_END) != 0) e("fseek does not work");
	long size_long = ftell(f);
	if (size_long == -1) e("ftell does not work");
	size_t size = size_long;
	rewind(f);
	char* r = new char[size];
	if (fread(r, 1, size, f) != size) e("cannot completely read the input");
	if (fclose(f) != 0) e("fclose does not work");
	return r;
}

void write_file(const char* filename, size_t size, const void* buf) {
	FILE* f = fopen(filename, "wb");
	if (!f) e("cannot open output file");
	if (fwrite(buf, 1, size, f) != size) e("cannot completely write the output");
	if (fclose(f) != 0) e("fclose does not work");
}

void c(const char* f1, const char* f2) {
	char* buf = read_file(f1);
	ASM76::Assembler a(buf);
	a.scan();
	ASM76::Program p = a.assemble();
	if (!ASM76::ObjectCode::write_file(f2, p)) e("cannot write object code");
	free(p.instruct);
	delete[] buf;
}

void d(const char* f1, const char* f2) {
	ASM76::Program p = ASM76::ObjectCode::read_file(f1);
	ASM76::Disassembler d(p);
	char* buf = d.disassemble();
	FILE* f = fopen(f2, "w");
	if (!f) e("cannot open output file");
	if (fputs(buf, f) < 0) e("fputs failed");
	if (fclose(f) != 0) e("fclose does not work");
	free(buf);
	free(p.instruct);
}

void r(const char* filename, const char* debug_flags) {
	ASM76::Program p;
	{
		char* buf = read_file(filename);
		if (memcmp(buf, ASM76::ObjectCode::magic_array, 8) == 0) {
			p = ASM76::ObjectCode::read_file(filename);
		} else {
			ASM76::Assembler a(buf);
			a.scan();
			p = a.assemble();
		}
		delete[] buf;
	}
	bool d = false, r = false, m = false;
	while (*debug_flags) switch (*debug_flags++) {
	case 'd': d = true; break;
	case 'r': r = true; break;
	case 'm': m = true; break;
	}
	{
		ASM76::VM v(p);
		v.execute(d);
		if (r) v.dump_registers();
		if (m) v.dump_memory();
	}
	free(p.instruct);
}

int main(int argc, char** argv) {
	if (argc <= 1) goto z;
	if (argv[1][1]) goto z;
	if (strchr("cd", argv[1][0]) && argc != 4) goto z;
	if (strchr("e", argv[1][0]) && argc != 3) goto z;
	switch (argv[1][0]) {
	case 'c':
		c(argv[2], argv[3]);
		break;
	case 'd':
		d(argv[2], argv[3]);
		break;
	case 'e':
		e(argv[2]);
		break;
	case 'r':
		r(argv[2], argc == 4 ? argv[3] : "");
		break;
	default:
		goto z;
	}
	return 0;
z:
	help(argv[0]);
	return 1;
}
