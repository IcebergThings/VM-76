//=============================================================================
// ■ SkyBox.cpp
//-----------------------------------------------------------------------------
//   3D模型：天空盒
//=============================================================================

#include "SkyBox.hpp"

namespace VM76 {
	SkyBox::SkyBox(const char* skyTexture) {
		tex = new Res::Texture(skyTexture);

		gbuffers_sky = new Shaders();
		gbuffers_sky->add_file(GL_VERTEX_SHADER, "../Media/shaders/gbuffers_skytextured.vsh");
		gbuffers_sky->add_file(GL_FRAGMENT_SHADER, "../Media/shaders/gbuffers_skytextured.fsh");
		gbuffers_sky->link_program();

		float xs = 0.25 * 4.0;
		float ys = 1.0 / 3.0;

		float yT = ys;
		float xT = 0.25;

		vtx[0] = new Vertex[4] {
			{{0.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + 0.0, ys + yT},{0.0, 0.0, -1.0}},
			{{0.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + 0.0, ys + 0.0},{0.0, 0.0, -1.0}},
			{{1.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + xT, ys + 0.0},{0.0, 0.0, -1.0}},
			{{1.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + xT, ys + yT},{0.0, 0.0, -1.0}},
		};
		xs = 0.25;
		vtx[1] = new Vertex[4] {
			{{0.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + 0.0, ys + yT},{0.0, 0.0, 1.0}},
			{{0.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + 0.0, ys + 0.0},{0.0, 0.0, 1.0}},
			{{1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + xT, ys + 0.0},{0.0, 0.0, 1.0}},
			{{1.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + xT, ys + yT},{0.0, 0.0, 1.0}},
		};
		xs = 0.25 * 1.0;
		ys = 0.0;
		vtx[2] = new Vertex[4] {
			{{0.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + 0.0, ys + 0.0},{0.0, 1.0, 0.0}},
			{{0.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + 0.0, ys + yT},{0.0, 1.0, 0.0}},
			{{1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + xT, ys + yT},{0.0, 1.0, 0.0}},
			{{1.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + xT, ys + 0.0},{0.0, 1.0, 0.0}},
		};
		ys = 2.0 / 3.0;
		vtx[3] = new Vertex[4] {
			{{0.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + 0.0, ys + yT},{0.0, -1.0, 0.0}},
			{{0.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + xT, ys + yT},{0.0, -1.0, 0.0}},
			{{1.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + xT, ys + 0.0},{0.0, -1.0, 0.0}},
			{{1.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + 0.0, ys + 0.0},{0.0, -1.0, 0.0}},
		};
		xs = 0.25 * 0.0;
		ys = 1.0 / 3.0;
		vtx[4] = new Vertex[4] {
			{{0.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + 0.0, ys + yT},{-1.0, 0.0, 0.0}},
			{{0.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + xT, ys + yT},{-1.0, 0.0, 0.0}},
			{{0.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + xT, ys + 0.0},{-1.0, 0.0, 0.0}},
			{{0.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + 0.0, ys + 0.0},{-1.0, 0.0, 0.0}},
		};
		xs = 0.25 * -3.0;
		vtx[5] = new Vertex[4] {
			{{1.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + 0.0, ys + yT},{1.0, 0.0, 0.0}},
			{{1.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + xT, ys + yT},{1.0, 0.0, 0.0}},
			{{1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + xT, ys + 0.0},{1.0, 0.0, 0.0}},
			{{1.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + 0.0, ys + 0.0},{1.0, 0.0, 0.0}},
		};
		itx[0] = new GLuint[6] {3,1,0,  3,2,1};
		itx[1] = new GLuint[6] {0,1,3,  1,2,3};
		itx[2] = new GLuint[6] {3,1,0,  3,2,1};
		itx[3] = new GLuint[6] {0,1,3,  1,2,3};
		itx[4] = new GLuint[6] {3,1,0,  3,2,1};
		itx[5] = new GLuint[6] {0,1,3,  1,2,3};

		mat = glm::scale(glm::translate(glm::mat4(1.0), glm::vec3(-64.0, -128.0, -64.0)), glm::vec3(512.0));

		// Prepare an empty space
		for (int i = 0; i < 6; i++) {
			obj[i] = new GDrawable();
			obj[i]->data.vtx_c = 4;
			obj[i]->data.ind_c = 2 * 3;
			obj[i]->data.vertices = vtx[i];
			obj[i]->data.indices = itx[i];
			obj[i]->data.mat_c = 1;
			obj[i]->data.mat =(GLuint*) &mat;
			obj[i]->fbind();
		}
	}

	void SkyBox::render() {
		gbuffers_sky->use();
		gbuffers_sky->ProjectionView(projection, view);
		gbuffers_sky->set_texture("colortex0", tex, 0);

		obj[0]->render();
		obj[1]->render();
		obj[2]->render();
		obj[3]->render();
		obj[4]->render();
		obj[5]->render();
	}

	SkyBox::~SkyBox() {
		for (int i = 0; i < 6; i++) {
			VMDE_Dispose(delete, obj[i]);
			XE(delete[], vtx[i]);
			XE(delete[], itx[i]);
		}
		XE(delete, gbuffers_sky);
	}
}
