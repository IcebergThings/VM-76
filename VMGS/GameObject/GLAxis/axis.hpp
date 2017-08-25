//=============================================================================
// ■ axis.hpp
//=============================================================================

#include <VMDE.hpp>

#ifndef _INCLUDE_GLAXIS_H
#define _INCLUDE_GLAXIS_H

namespace VM76 {
	class Axis : public RenderObject {
	public:
		GDrawable* obj = NULL;
		glm::mat4* mat = NULL;

	private:
		Vertex* vtx = NULL;
		GLuint* itx = NULL;

	public:
		Axis();
		void render();
		~Axis();
	};
}

#endif
