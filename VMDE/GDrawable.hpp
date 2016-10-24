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
		glm::mat4 model;
	};
	void prepare(GDrawable* s, glm::mat4 projection, glm::mat4 view);
	void draw(struct GDrawable* s, GLuint start, GLuint end);
	void draw(struct GDrawable* s);
	void fbind(GDrawable* s);
	void update(GDrawable* s);
	void dispose(GDrawable* s);
	GDrawable* create();
}

#endif
