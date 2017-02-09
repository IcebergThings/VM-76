//=============================================================================
// ■ tile.hpp
//=============================================================================

#include <global.hpp>

#ifndef _INCLUDE_TILES_H
#define _INCLUDE_TILES_H

namespace VM76 {
	class Tiles : public RenderObject {
	public:
		GDrawable* obj[6];
		glm::mat4 *mat[6];

	private:
		Vertex *vtx[6];
		GLuint *itx[6];

	public:
		Tiles(int tid);
		void render();
		void update_instance(int c1, int c2, int c3, int c4, int c5, int c6);
		void dispose();
	};
}

#endif
