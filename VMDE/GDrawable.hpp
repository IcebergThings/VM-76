//=============================================================================
// ■ GDrawable.hpp
//-----------------------------------------------------------------------------
//   GDrawable头文件。
//=============================================================================

#ifndef _INCLUDE_GDRAWBLE_H
#define _INCLUDE_GDRAWBLE_H

#include "global.hpp"

namespace GDrawable {
	struct GDrawable {
		GLfloat* vertices;
		int vtx_c;
		GLuint* indices;
		int ind_c;
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		GLuint tri_mesh_count;
	};
	void draw(GDrawable* s);
	void fbind(GDrawable* s);
	void update(GDrawable* s);
	void dispose(GDrawable* s);
	GDrawable* create();
}

#endif
