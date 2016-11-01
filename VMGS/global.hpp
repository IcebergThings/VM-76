//=============================================================================
// ■ global.hpp
//-----------------------------------------------------------------------------
//   关于为什么用global.hpp而非一个个的小头，请参阅/VMDE/global.hpp。
//=============================================================================

#include "VMDE/VMDE.hpp"

#ifndef _INCLUDE_VMGS_GLOBAL_H
#define _INCLUDE_VMGS_GLOBAL_H

namespace VM76 {
	class Tile {
	public:
		GDrawable::GDrawable* obj = NULL;

	private:
		GLfloat *vtx = NULL;
		GLuint *itx = NULL;

	public:
		Tile(int tid);
	};

	extern Tile* t[16];

	const GLfloat tile_size = 1.0f / 16.0f;

	void loop();
	void start_game();
	void terminate();
}
#endif
