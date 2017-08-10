//=============================================================================
// ■ V.hpp
//-----------------------------------------------------------------------------
//   注意：这个目录里的很多文件都是模板类，它们有单独的头文件。
//=============================================================================

#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>
#ifndef _INCLUDE_V_HPP
#define _INCLUDE_V_HPP

#include "VError.hpp"
#include "VExplosion.hpp"
#include "VEndian.hpp"
namespace V {
	#include "VMath.hpp"
	#include "VVector.hpp"
	#include "VRingBuffer.hpp"
	#include "VBinaryFileReader.hpp"
	#include "VBinaryFileWriter.hpp"
}

#endif
