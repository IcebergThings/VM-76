//=============================================================================
// ■ SkyBox.hpp
//=============================================================================

#include <global.hpp>

#ifndef _INCLUDE_SKYBOX_H
#define _INCLUDE_SKYBOX_H

namespace VM76 {

	typedef struct struct_SimpleVertex { glm::vec3 pos; } SimpleVertex;

	class SkyBox : public RenderObject {
	public:
		GDrawable* obj;
		Res::CubeMap* tex;
		glm::mat4 mat;
		Shaders* gbuffers_sky;

	private:
		SimpleVertex* vtx;
		GLuint* itx;

	public:
		SkyBox(const struct Res::CubeMapFiles files);
		void render();
		~SkyBox();
	};
}

#endif
