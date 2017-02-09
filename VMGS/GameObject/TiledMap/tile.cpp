//=============================================================================
// ■ tile.cpp
//-----------------------------------------------------------------------------
//   3D模型：Tile
//=============================================================================

#include "tile.hpp"

namespace VM76 {
	Tiles::Tiles(int tid) {
		int x = tid % 16;
		int y = tid / 16;
		float T = 1.0f / 16.0f;
		float S = 0.0f;
		float xs = x * T;
		float ys = y * T;
		vtx[0] = new Vertex[4] {
			{{0.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + T},{0.0, 0.0, -1.0}},
			{{0.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + S},{0.0, 0.0, -1.0}},
			{{1.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + S},{0.0, 0.0, -1.0}},
			{{1.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + T},{0.0, 0.0, -1.0}},
		};
		vtx[1] = new Vertex[4] {
			{{0.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + T},{0.0, 0.0, 1.0}},
			{{0.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + S},{0.0, 0.0, 1.0}},
			{{1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + S},{0.0, 0.0, 1.0}},
			{{1.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + T},{0.0, 0.0, 1.0}},
		};
		vtx[2] = new Vertex[4] {
			{{0.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + S},{0.0, 1.0, 0.0}},
			{{0.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + T},{0.0, 1.0, 0.0}},
			{{1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + T},{0.0, 1.0, 0.0}},
			{{1.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + S},{0.0, 1.0, 0.0}},
		};
		vtx[3] = new Vertex[4] {
			{{0.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + T},{0.0, -1.0, 0.0}},
			{{0.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + T},{0.0, -1.0, 0.0}},
			{{1.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + S},{0.0, -1.0, 0.0}},
			{{1.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + S},{0.0, -1.0, 0.0}},
		};
		vtx[4] = new Vertex[4] {
			{{0.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + T},{-1.0, 0.0, 0.0}},
			{{0.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + T},{-1.0, 0.0, 0.0}},
			{{0.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + S},{-1.0, 0.0, 0.0}},
			{{0.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + S},{-1.0, 0.0, 0.0}},
		};
		vtx[5] = new Vertex[4] {
			{{1.0, 0.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + T},{1.0, 0.0, 0.0}},
			{{1.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + T},{1.0, 0.0, 0.0}},
			{{1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{xs + T, ys + S},{1.0, 0.0, 0.0}},
			{{1.0, 1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{xs + S, ys + S},{1.0, 0.0, 0.0}},
		};
		itx[0] = new GLuint[6] {0,1,3,  1,2,3};
		itx[1] = new GLuint[6] {3,1,0,  3,2,1};
		itx[2] = new GLuint[6] {0,1,3,  1,2,3};
		itx[3] = new GLuint[6] {3,1,0,  3,2,1};
		itx[4] = new GLuint[6] {0,1,3,  1,2,3};
		itx[5] = new GLuint[6] {3,1,0,  3,2,1};

		// Prepare an empty space
		for (int i = 0; i < 6; i++) {
			mat[i] = new glm::mat4[4096];
			for (int x = 0; x < 4096; x++) mat[i][x] = glm::mat4(1.0);

			obj[i] = new GDrawable();
			obj[i]->data.vtx_c = 4;
			obj[i]->data.ind_c = 2 * 3;
			obj[i]->data.vertices = vtx[i];
			obj[i]->data.indices = itx[i];
			// Reserve 512 spaces
			obj[i]->data.mat_c = 4096;
			obj[i]->data.mat = (GLuint*) &mat[i][0];
			obj[i]->fbind();
			// Draw none for default
			obj[i]->data.mat_c = 0;
		}
	}

	void Tiles::update_instance(int c1, int c2, int c3, int c4, int c5, int c6) {
		obj[0]->data.mat_c = c1;
		obj[0]->data.mat = (GLuint*) mat[0];
		obj[0]->update_instance();

		obj[1]->data.mat_c = c2;
		obj[1]->data.mat = (GLuint*) mat[1];
		obj[1]->update_instance();

		obj[2]->data.mat_c = c3;
		obj[2]->data.mat = (GLuint*) mat[2];
		obj[2]->update_instance();

		obj[3]->data.mat_c = c4;
		obj[3]->data.mat = (GLuint*) mat[3];
		obj[3]->update_instance();

		obj[4]->data.mat_c = c5;
		obj[4]->data.mat = (GLuint*) mat[4];
		obj[4]->update_instance();

		obj[5]->data.mat_c = c6;
		obj[5]->data.mat = (GLuint*) mat[5];
		obj[5]->update_instance();
	}

	void Tiles::render() {
		for (int i = 0; i < 6; i++)
			if (obj[i]->data.mat_c) obj[i]->render();
	}

	void Tiles::dispose() {
		for (int i = 0; i < 6; i++) {
			VMDE_Dispose(obj[i]);
			xefree(vtx[i]); xefree(itx[i]);
		}
	}
}
