#include <VMDE/VMDE.hpp>

#ifndef _INCLUDE_TILES_H
#define _INCLUDE_TILES_H

namespace VM76 {
	class Tile {
	public:
		GDrawable* obj = NULL;

	private:
		GLfloat *vtx = NULL;
		GLuint *itx = NULL;
		glm::mat4 *mat = NULL;

	public:
		Tile(int tid);
	};
}

#endif
