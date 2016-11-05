#include "tiles.hpp"

namespace VM76 {
	Tile::Tile(int tid) {
		int x = tid % 16;
		int y = tid / 16;
		float T = 1.0f / 16.0f;
		float S = 0.0f;
		float xs = x * T;
		float ys = y * T;
		vtx[0] = new GLfloat[9 * 4] {
			0.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
			0.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
			1.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,
		};
		vtx[1] = new GLfloat[9 * 4] {
			0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
			0.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
			1.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,
		};
		vtx[2] = new GLfloat[9 * 4] {
			0.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
			0.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
			1.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,
		};
		vtx[3] = new GLfloat[9 * 4] {
			0.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
			0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,
			1.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
		};
		vtx[4] = new GLfloat[9 * 4] {
			0.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
			0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,
			0.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			0.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
		};
		vtx[5] = new GLfloat[9 * 4] {
			1.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
			1.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,
			1.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
		};
		itx[0] = new GLuint[6] {0,1,3,  1,2,3};
		itx[1] = new GLuint[6] {3,1,0,  3,2,1};
		itx[2] = new GLuint[6] {0,1,3,  1,2,3};
		itx[3] = new GLuint[6] {3,1,0,  3,2,1};
		itx[4] = new GLuint[6] {0,1,3,  1,2,3};
		itx[5] = new GLuint[6] {3,1,0,  3,2,1};

		mat = new glm::mat4[256 * 256];
		for (int x = 0; x < 256; x++) {
			for (int y = 0; y < 256; y++) {
				mat[x * 256 + y] = glm::translate(glm::mat4(1.0), glm::vec3(float(x) - 128.0f, 0.0, float(y) - 128.0f));
			}
		}

		for (int x = 0; x < 6; x++) {
			obj[x] = new GDrawable();
			obj[x]->data.vtx_c = 9 * 4;
			obj[x]->data.ind_c = 2 * 3;
			obj[x]->data.vertices = vtx[x];
			obj[x]->data.indices = itx[x];
			obj[x]->data.tri_mesh_count = 2;
			obj[x]->data.mat_c = 256 * 256;
			obj[x]->data.mat = (GLuint*) &mat[0];
			obj[x]->fbind();
		}
	}

	void Tile::render(Shaders* shader, glm::mat4 projection, glm::mat4 view) {
		obj[2]->prepare(shader, projection, view);
		obj[2]->draw();
	}

	void Tile::dispose() {
		obj[0]->dispose();
		obj[1]->dispose();
		obj[2]->dispose();
		obj[3]->dispose();
		obj[4]->dispose();
		obj[5]->dispose();
		xefree(vtx[0]); xefree(itx[0]);
		xefree(vtx[1]); xefree(itx[1]);
		xefree(vtx[2]); xefree(itx[2]);
		xefree(vtx[3]); xefree(itx[3]);
		xefree(vtx[4]); xefree(itx[4]);
		xefree(vtx[5]); xefree(itx[5]);
	}
}
