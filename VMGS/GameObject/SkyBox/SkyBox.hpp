//=============================================================================
// ■ SkyBox.hpp
//=============================================================================

#include <global.hpp>

#ifndef _INCLUDE_SKYBOX_H
#define _INCLUDE_SKYBOX_H

namespace VM76 {
	class SkyBox : public RenderObject {
	public:
		GDrawable* obj[6];
		Res::Texture* tex;
		glm::mat4 mat;
		Shaders* gbuffers_sky;

	private:
		Vertex* vtx[6];
		GLuint* itx[6];

	public:
		SkyBox(const char* skyTexture);
		void render();
		~SkyBox();
	};
}

#endif
