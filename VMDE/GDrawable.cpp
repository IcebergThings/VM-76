//=============================================================================
// ■ GDrawable.cpp
//-----------------------------------------------------------------------------
//   VMDE中GDrawable的控制与类。
//=============================================================================

#include "global.hpp"

void GDrawable::draw(GLuint start, GLuint end) {
	glBindVertexArray(data.VAO);
	glDrawRangeElements(GL_TRIANGLES, start, end, end + 1, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GDrawable::draw() {
	glBindVertexArray(data.VAO);
	glDrawElementsInstanced(
		GL_TRIANGLES, data.ind_c, GL_UNSIGNED_INT, 0, data.mat_c);
	glBindVertexArray(0);
}

void GDrawable::fbind() {
	glBindVertexArray(data.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
	glBufferData(GL_ARRAY_BUFFER, data.vtx_c * sizeof(GLfloat), data.vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.ind_c * sizeof(GLuint), data.indices, GL_STATIC_DRAW);

	size_t vertex_size = (3 + 4 + 2) * sizeof(GLfloat); // X,Y,Z,  R,G,B,A,  S,T
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*) (7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, data.MBO);
	glBufferData(GL_ARRAY_BUFFER, data.mat_c * sizeof(glm::mat4), data.mat, GL_DYNAMIC_DRAW);
	size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*) 0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*) (vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*) (2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*) (3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GDrawable::update() {
	glBindVertexArray(data.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
	void *bufv = glMapBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	memcpy(bufv, data.vertices, data.vtx_c * sizeof(GLfloat));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.EBO);
	void *bufi = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
	memcpy(bufi, data.indices, data.ind_c * sizeof(GLuint));
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, data.MBO);
	void *bufm = glMapBuffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	memcpy(bufm, data.mat, data.mat_c * sizeof(glm::mat4));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void GDrawable::update_instance() {
	glBindVertexArray(data.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, data.MBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.mat_c * sizeof(glm::mat4), data.mat);
	//glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void GDrawable::dispose() {
	glDeleteVertexArrays(1, &data.VAO);
	glDeleteBuffers(1, &data.VBO);
	glDeleteBuffers(1, &data.EBO);
	glDeleteBuffers(1, &data.MBO);
	// 都在类里面了，一个free就完事了
}

GDrawable::GDrawable() {
	glGenVertexArrays(1, &data.VAO);
	glGenBuffers(1, &data.VBO);
	glGenBuffers(1, &data.EBO);
	glGenBuffers(1, &data.MBO);
	data.vertices = NULL;
	data.indices = NULL;
	data.mat = NULL;
}
