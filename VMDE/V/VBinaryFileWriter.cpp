//=============================================================================
// ■ VBinaryFileWriter.cpp
//-----------------------------------------------------------------------------
//   在各计算机之间兼容的二进制文件写出类。（Little-endian）
//=============================================================================

#include "../global.hpp"

namespace V {
	#define ERROR_MESSAGE "error: lorem ipsum"
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
	// ● 写入1字节int
	//-------------------------------------------------------------------------
	void VBinaryFileWriter::write_i8(int8_t value) {
		write_u8((uint8_t) value);
	}
	//-------------------------------------------------------------------------
	// ● 写入4字节int
	//-------------------------------------------------------------------------
	#define WRITE_CHAR \
		if (fputc(value & 0xff, f) < 0) error(ERROR_MESSAGE)
	#define WRITE_CHAR_ \
		if (fputc((value >>= 8) & 0xff, f) < 0) error(ERROR_MESSAGE)
	void VBinaryFileWriter::write_i32(int32_t value) {
		WRITE_CHAR;
		WRITE_CHAR_;
		WRITE_CHAR_;
		WRITE_CHAR_;
	}
	//-------------------------------------------------------------------------
	// ● 写入8字节int
	//-------------------------------------------------------------------------
	void VBinaryFileWriter::write_i64(int64_t value) {
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
	// ● 写入1字节unsigned
	//-------------------------------------------------------------------------
	void VBinaryFileWriter::write_u8(uint8_t value) {
		if (fputc(value, f) < 0) error(ERROR_MESSAGE);
	}
	//-------------------------------------------------------------------------
	// ● 写入4字节unsigned
	//-------------------------------------------------------------------------
	void VBinaryFileWriter::write_u32(uint32_t value) {
		write_i32((int32_t) value);
	}
	//-------------------------------------------------------------------------
	// ● 写入8字节unsigned
	//-------------------------------------------------------------------------
	void VBinaryFileWriter::write_u64(uint64_t value) {
		write_i64((int64_t) value);
	}
	//-------------------------------------------------------------------------
	// ● 写入4字节float
	//-------------------------------------------------------------------------
	void VBinaryFileWriter::write_float(float value) {
		write_directly<float>(value);
	}
	//-------------------------------------------------------------------------
	// ● 写入8字节double
	//-------------------------------------------------------------------------
	void VBinaryFileWriter::write_double(double value) {
		write_directly<double>(value);
	}
}
