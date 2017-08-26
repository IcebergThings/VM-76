//=============================================================================
// ■ SkyBox.cpp
//-----------------------------------------------------------------------------
//   3D模型：天空盒
//=============================================================================

#include "../../VMGS.hpp"

namespace VM76 {

	GLuint Simple_VBO_size[] = {3};
	GLuint Simple_VBO_type[] = {GL_FLOAT};

	struct VBO_Entry_Descriptor Simple_VBO {
		3 * sizeof(GLfloat),
		1,
		Simple_VBO_size,
		Simple_VBO_type,
		false,
		NULL
	};

	SkyBox::SkyBox(const struct Res::CubeMapFiles files, Camera* cam) {
		tex = new Res::CubeMap(files,NULL);

		gbuffers_sky = new Shaders();
		gbuffers_sky->add_file(GL_VERTEX_SHADER, "../Media/shaders/gbuffers_skytextured.vsh");
		gbuffers_sky->add_file(GL_FRAGMENT_SHADER, "../Media/shaders/gbuffers_skytextured.fsh");
		gbuffers_sky->link_program();

		vtx = new SimpleVertex[4 * 6] {
			{{-1.0, -1.0, -1.0}},
			{{-1.0,  1.0, -1.0}},
			{{ 1.0,  1.0, -1.0}},
			{{ 1.0, -1.0, -1.0}},

			{{-1.0, -1.0,  1.0}},
			{{-1.0,  1.0,  1.0}},
			{{ 1.0,  1.0,  1.0}},
			{{ 1.0, -1.0,  1.0}},
		};
		itx = new GLuint[6 * 6] {
			3,1,0,  3,2,1,
			4,5,7,  5,6,7,
			2,5,1,  2,6,5,
			0,4,3,  4,7,3,
			1,4,0,  1,5,4,
			3,7,2,  7,6,2,
		};

		mat = glm::scale(glm::mat4(1.0), glm::vec3(512.0));

		obj = new GDrawable(&Simple_VBO, NULL);
		obj->data.vtx_c = 8;
		obj->data.ind_c = 6 * 6;
		obj->data.vertices = (GLuint*) vtx;
		obj->data.indices = itx;
		obj->data.mat_c = 1;
		obj->data.mat =(GLuint*) &mat;
		obj->fbind();

		this->cam = cam;
	}

	void SkyBox::render() {
		gbuffers_sky->use();
		gbuffers_sky->ProjectionView(cam->Projection, cam->view_camera);
		gbuffers_sky->set_texture_cube("cubemap0", tex, 0);

		obj->render();
	}

	SkyBox::~SkyBox() {
		XE(delete, obj);
		XE(delete[], vtx);
		XE(delete[], itx);
		XE(delete, gbuffers_sky);
	}
}
