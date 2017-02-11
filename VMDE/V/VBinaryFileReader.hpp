//=============================================================================
// ■ VBinaryFileReader.hpp
//=============================================================================

namespace V {
	class VBinaryFileReader {
	private:
		static const char* ERROR_MESSAGE;
		FILE* f;
	public:
		VBinaryFileReader(const char* filename);
		~VBinaryFileReader();
		template <class T> T read();
		template <> int32_t read<int32_t>();
		template <> int64_t read<int64_t>();
		template <> uint32_t read<uint32_t>();
		template <> uint64_t read<uint64_t>();
		#define read_i8 read<int8_t>
		#define read_i32 read<int32_t>
		#define read_i64 read<int64_t>
		#define read_u8 read<uint8_t>
		#define read_u32 read<uint32_t>
		#define read_u64 read<uint64_t>
		#define read_float read<float>
		#define read_double read<double>
	};
	//-------------------------------------------------------------------------
	// ● 直接读取
	//   慎用！它对数据的可移植性没有保证！
	//-------------------------------------------------------------------------
	template <class T> T VBinaryFileReader::read() {
		T r;
		if (fread(&r, sizeof(T), 1, f) < 1) error(ERROR_MESSAGE);
		return r;
	}
}
