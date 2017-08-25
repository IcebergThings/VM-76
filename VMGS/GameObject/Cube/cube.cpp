//=============================================================================
// ■ cube.cpp
//-----------------------------------------------------------------------------
//   3D模型：方块
//=============================================================================

#include "../../VMGS.hpp"

namespace VM76 {
	Cube::Cube(int tid) {
		int x = tid % 16;
		int y = tid / 16;
		float T = 1.0f / 16.0f;
		float S = 0.0f;
		float xs = x * T;
		float ys = y * T;
		vtx = new Vertex[4 * 6] {
			{{0.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + T},{0.0, 0.0, -1.0}},
			{{0.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + S},{0.0, 0.0, -1.0}},
			{{1.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + S},{0.0, 0.0, -1.0}},
			{{1.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + T},{0.0, 0.0, -1.0}},

			{{0.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + T},{0.0, 0.0, 1.0}},
			{{0.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + S},{0.0, 0.0, 1.0}},
			{{1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + S},{0.0, 0.0, 1.0}},
			{{1.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + T},{0.0, 0.0, 1.0}},

			{{0.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + S},{0.0, 1.0, 0.0}},
			{{0.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + T},{0.0, 1.0, 0.0}},
			{{1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + T},{0.0, 1.0, 0.0}},
			{{1.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + S},{0.0, 1.0, 0.0}},

			{{0.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + T},{0.0, -1.0, 0.0}},
			{{0.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + T},{0.0, -1.0, 0.0}},
			{{1.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + S},{0.0, -1.0, 0.0}},
			{{1.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + S},{0.0, -1.0, 0.0}},

			{{0.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + T},{-1.0, 0.0, 0.0}},
			{{0.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + T},{-1.0, 0.0, 0.0}},
			{{0.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + S},{-1.0, 0.0, 0.0}},
			{{0.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + S},{-1.0, 0.0, 0.0}},

			{{1.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + T},{1.0, 0.0, 0.0}},
			{{1.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + T},{1.0, 0.0, 0.0}},
			{{1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + S},{1.0, 0.0, 0.0}},
			{{1.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + S},{1.0, 0.0, 0.0}},
		};
		itx = new GLuint[6 * 6] {
			0 + 0,  0 + 1,  0 + 3,   0 + 1,  0 + 2,  0 + 3,
			4 + 3,  4 + 1,  4 + 0,   4 + 3,  4 + 2,  4 + 1,
			8 + 0,  8 + 1,  8 + 3,   8 + 1,  8 + 2,  8 + 3,
			12 + 3, 12 + 1, 12 + 0,  12 + 3, 12 + 2, 12 + 1,
			16 + 0, 16 + 1, 16 + 3,  16 + 1, 16 + 2, 16 + 3,
			20 + 3, 20 + 1, 20 + 0,  20 + 3, 20 + 2, 20 + 1,
		};

		// Prepare an empty space
		mat = new glm::mat4[1];
		mat[0] = glm::mat4(1.0);

		obj = new GDrawable(NULL, NULL);
		obj->data.vtx_c = 4 * 6;
		obj->data.ind_c = 2 * 3 * 6;
		obj->data.vertices = (GLuint*)vtx;
		obj->data.indices = itx;
		// Reserve 1 space
		obj->data.mat_c = 1;
		obj->data.mat = (GLuint*) &mat[0];
		obj->fbind();
		// Draw none for default
		obj->data.mat_c = 0;

		//XE(delete[], mat);
	}

	void Cube::update_instance(int mat_c, glm::mat4* mat) {
		obj->data.mat_c = mat_c;
		obj->data.mat = (GLuint*) mat;
		obj->update_instance_alien_size();
	}

	void Cube::update_instance(int mat_c) {
		obj->data.mat_c = mat_c;
		obj->data.mat = (GLuint*) mat;
		obj->update_instance_alien_size();
	}

	void Cube::render() {
		obj->render();
	}

	Cube::~Cube() {
		VMDE_Dispose(delete, obj);
		XE(delete[], vtx);
		XE(delete[], itx);
	}
}
