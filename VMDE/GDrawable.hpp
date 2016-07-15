//=============================================================================
// ■ GDrawable.h
//-----------------------------------------------------------------------------
//   VMDE中GDrawable的控制与类。
//=============================================================================
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef _INCLUDE_GDRAWABLE_H
#define _INCLUDE_GDRAWABLE_H

namespace GDrawableNS {

	struct GDrawable {
		GLfloat* vertices;
		GLuint VAO;
		GLuint VBO;
		size_t size_of_VBO;
	};

	void draw(struct GDrawable* s);

	void update(struct GDrawable* s);

	struct GDrawable* create();

}

#endif
