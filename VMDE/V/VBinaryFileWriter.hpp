//=============================================================================
// ■ VBinaryFileWriter.hpp
//=============================================================================

namespace V {
	class VBinaryFileWriter {
	private:
		static const char* ERROR_MESSAGE;
		FILE* f;
	public:
		VBinaryFileWriter(const char* filename);
		~VBinaryFileWriter();
		template <class T> void write(T value);
		template <> void write(int32_t value);
		template <> void write(int64_t value);
		template <> void write(uint32_t value);
		template <> void write(uint64_t value);
	};
	//-------------------------------------------------------------------------
	// ● 直接写入
	//   慎用！它对数据的可移植性没有保证！
	//-------------------------------------------------------------------------
	template <class T> void VBinaryFileWriter::write(T value) {
		if (fwrite(&value, sizeof(T), 1, f) < 1) error(ERROR_MESSAGE);
	};
	#define write_i8 write<int8_t>
	#define write_i32 write<int32_t>
	#define write_i64 write<int64_t>
	#define write_u8 write<uint8_t>
	#define write_u32 write<uint32_t>
	#define write_u64 write<uint64_t>
	#define write_float write<float>
	#define write_double write<double>
}
