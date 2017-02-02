//=============================================================================
// ■ axis.hpp
//=============================================================================

#include <global.hpp>

#ifndef _INCLUDE_GLAXIS_H
#define _INCLUDE_GLAXIS_H

namespace VM76 {
	class Axis {
	public:
		GDrawable* obj = NULL;
		glm::mat4 *mat = NULL;

	private:
		GLfloat *vtx = NULL;
		GLuint *itx = NULL;

	public:
		Axis();
		void render();
		void dispose();
	};
}

#endif
