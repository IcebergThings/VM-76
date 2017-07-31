//=============================================================================
// ■ SkyBox.hpp
//=============================================================================

#include <global.hpp>

#ifndef _INCLUDE_SKYBOX_H
#define _INCLUDE_SKYBOX_H

namespace VM76 {

	class SkyBox : public RenderObject {
	public:
		GDrawable* obj;
		Res::CubeMap* tex;
		glm::mat4 mat;
		Shaders* gbuffers_sky;

	private:
		Vertex* vtx;
		GLuint* itx;

	public:
		SkyBox(const struct Res::CubeMapFiles files);
		void render();
		~SkyBox();
	};
}

#endif
