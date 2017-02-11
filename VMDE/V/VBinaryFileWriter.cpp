//=============================================================================
// ■ VBinaryFileWriter.cpp
//-----------------------------------------------------------------------------
//   在各计算机之间兼容的二进制文件写出类（Little-endian）。
//=============================================================================

#include "../global.hpp"

namespace V {
	//-------------------------------------------------------------------------
	// ● 常量
	//-------------------------------------------------------------------------
	const char* VBinaryFileWriter::ERROR_MESSAGE = "error: lorem ipsum";
	//-------------------------------------------------------------------------
	// ● 构造
	//-------------------------------------------------------------------------
	VBinaryFileWriter::VBinaryFileWriter(const char* filename) {
		f = fopen(filename, "wb");
		if (!f) error(ERROR_MESSAGE);
	}
	//-------------------------------------------------------------------------
	// ● 析构
	//-------------------------------------------------------------------------
	VBinaryFileWriter::~VBinaryFileWriter() {
		fclose(f);
	}
	//-------------------------------------------------------------------------
	// ● 写入4字节int
	//-------------------------------------------------------------------------
	#define WRITE_CHAR \
		if (fputc(value & 0xff, f) < 0) error(ERROR_MESSAGE)
	#define WRITE_CHAR_ \
		if (fputc((value >>= 8) & 0xff, f) < 0) error(ERROR_MESSAGE)
	template <> void VBinaryFileWriter::write(int32_t value) {
		WRITE_CHAR;
		WRITE_CHAR_;
		WRITE_CHAR_;
		WRITE_CHAR_;
	}
	//-------------------------------------------------------------------------
	// ● 写入8字节int
	//-------------------------------------------------------------------------
	template <> void VBinaryFileWriter::write(int64_t value) {
		WRITE_CHAR;
		WRITE_CHAR_;
		WRITE_CHAR_;
		WRITE_CHAR_;
		WRITE_CHAR_;
		WRITE_CHAR_;
		WRITE_CHAR_;
		WRITE_CHAR_;
	}
	#undef WRITE_CHAR
	#undef WRITE_CHAR_
	//-------------------------------------------------------------------------
	// ● 写入4字节unsigned
	//-------------------------------------------------------------------------
	template <> void VBinaryFileWriter::write(uint32_t value) {
		// Once I imagined I was writing HTML.
		write<int32_t>(static_cast<int32_t>(value));
	}
	//-------------------------------------------------------------------------
	// ● 写入8字节unsigned
	//-------------------------------------------------------------------------
	template <> void VBinaryFileWriter::write(uint64_t value) {
		write<int64_t>(static_cast<int64_t>(value));
	}
}
