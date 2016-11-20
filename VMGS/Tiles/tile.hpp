//=============================================================================
// ■ tile.hpp
//=============================================================================

#include <global.hpp>

#ifndef _INCLUDE_TILES_H
#define _INCLUDE_TILES_H

namespace VM76 {
	class Tile {
	public:
		GDrawable* obj[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
		glm::mat4 *mat = NULL;

	private:
		GLfloat *vtx[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
		GLuint *itx[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

	public:
		Tile(int tid);
		void render(bool r1, bool r2, bool r3, bool r4, bool r5, bool r6);
		void update_instance(int mat_c, glm::mat4* mat);
		void dispose();
	};
}

#endif
