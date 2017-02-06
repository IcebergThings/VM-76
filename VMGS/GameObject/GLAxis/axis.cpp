//=============================================================================
// ■ axis.cpp
//-----------------------------------------------------------------------------
//   3D模型：3维座标系
//=============================================================================

#include "axis.hpp"

namespace VM76 {
	Axis::Axis() {
		vtx = new Vertex[3 * 3] {
			// X Axis
			{{0.0, 0.0, 0.0},{1.0, 0.0, 0.0, 1.0},{0.0,0.0},{0.0, 0.0, 0.0}},
			{{0.0, 0.0, 0.0},{1.0, 0.0, 0.0, 1.0},{0.0,0.0},{0.0, 0.0, 0.0}},
			{{0.3, 0.0, 0.0},{1.0, 0.0, 0.0, 1.0},{0.0,0.0},{0.0, 0.0, 0.0}},

			// Y Axis
			{{0.0, 0.0, 0.0},{0.0, 1.0, 0.0, 1.0},{0.0,0.0},{0.0, 0.0, 0.0}},
			{{0.0, 0.0, 0.0},{0.0, 1.0, 0.0, 1.0},{0.0,0.0},{0.0, 0.0, 0.0}},
			{{0.0, 0.3, 0.0},{0.0, 1.0, 0.0, 1.0},{0.0,0.0},{0.0, 0.0, 0.0}},

			// Z Axis
			{{0.0, 0.0, 0.0},{0.0, 0.0, 1.0, 1.0},{0.0,0.0},{0.0, 1.0, 0.0}},
			{{0.0, 0.0, 0.0},{0.0, 0.0, 1.0, 1.0},{0.0,0.0},{0.0, 1.0, 0.0}},
			{{0.0, 0.0, 0.3},{0.0, 0.0, 1.0, 1.0},{0.0,0.0},{0.0, 1.0, 0.0}},
		};
		itx = new GLuint[3 * 3] {
			0,1,2, 3,4,5, 6,7,8
		};

		// Prepare an empty space
		mat = new glm::mat4[1] {glm::mat4(1.0)};

		obj = new GDrawable();
		obj->data.vtx_c = 3 * 3;
		obj->data.ind_c = 3 * 3;
		obj->data.vertices = vtx;
		obj->data.indices = itx;
		obj->data.tri_mesh_count = 3;
		obj->data.mat_c = 1;
		obj->data.mat = (GLuint*) &mat[0];
		obj->fbind();
	}

	void Axis::render() {
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		obj->draw();
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void Axis::dispose() {
		VMDE_Dispose(obj);
		xefree(vtx); xefree(itx);
	}
}
