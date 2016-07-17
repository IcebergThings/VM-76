//=============================================================================
// ■ GDrawable.cpp
//-----------------------------------------------------------------------------
//   VMDE中GDrawable的控制与类。
//=============================================================================
#include "global.hpp"

namespace GDrawable {

	void draw(struct GDrawable* s) {
		glBindVertexArray(s->VAO);
		glDrawArrays(GL_TRIANGLES, 0, s->tri_mesh_count);
		glBindVertexArray(0);
	}

	void fbind(struct GDrawable* s) {
		glBindVertexArray(s->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, s->VBO);
		glBufferData(GL_ARRAY_BUFFER, s->size_of_VBO, s->vertices, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void update(struct GDrawable* s) {
		glBindVertexArray(s->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, s->VBO);
		void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(buf, s->vertices, s->size_of_VBO);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER,0);
	}

	struct GDrawable* create() {
		struct GDrawable* s = (struct GDrawable*) malloc(sizeof(struct GDrawable));

		glGenVertexArrays(1, &s->VAO);
		glGenBuffers(1, &s->VBO);
		s->vertices = NULL;

		return s;
	}

}
