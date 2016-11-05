#include <VMDE/VMDE.hpp>

#ifndef _INCLUDE_TILES_H
#define _INCLUDE_TILES_H

namespace VM76 {
	class Tile {
	public:
		GDrawable* obj[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

	private:
		GLfloat *vtx[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
		GLuint *itx[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
		glm::mat4 *mat = NULL;

	public:
		Tile(int tid);
		void render(Shaders* shader, glm::mat4 projection, glm::mat4 view);
		void dispose();
	};
}

#endif
