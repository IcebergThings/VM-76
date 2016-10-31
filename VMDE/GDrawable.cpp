//=============================================================================
// ■ GDrawable.cpp
//-----------------------------------------------------------------------------
//   VMDE中GDrawable的控制与类。
//=============================================================================
#include "GDrawable.hpp"

void GDrawable::prepare(glm::mat4 projection, glm::mat4 view) {
	GLuint loc = glGetUniformLocation(main_shader->shaderProgram, "Model");
	GLfloat* ptrM = glm::value_ptr(data.model);
	glUniformMatrix4fv(loc, 1, GL_FALSE, ptrM);

	loc = glGetUniformLocation(main_shader->shaderProgram, "View");
	GLfloat* ptrV = glm::value_ptr(view);
	glUniformMatrix4fv(loc, 1, GL_FALSE, ptrV);

	loc = glGetUniformLocation(main_shader->shaderProgram, "Projection");
	GLfloat* ptrP = glm::value_ptr(projection);
	glUniformMatrix4fv(loc, 1, GL_FALSE, ptrP);
}

void GDrawable::draw(GLuint start, GLuint end) {
	glBindVertexArray(data.VAO);
	glDrawRangeElements(GL_TRIANGLES, start, end, end + 1, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GDrawable::draw() {
	glBindVertexArray(data.VAO);
	glDrawElements(GL_TRIANGLES, data.ind_c, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GDrawable::fbind() {
	glBindVertexArray(data.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
	glBufferData(GL_ARRAY_BUFFER, data.vtx_c * sizeof(GLfloat), data.vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.ind_c * sizeof(GLuint), data.indices, GL_DYNAMIC_DRAW);

	size_t vertex_size = (3 + 4 + 2) * sizeof(GLfloat); // X,Y,Z,  R,G,B,A,  S,T
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*) (7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GDrawable::update() {
	glBindVertexArray(data.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
	void *bufv = glMapBuffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	memcpy(bufv, data.vertices, data.vtx_c * sizeof(GLfloat));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.EBO);
	void *bufi = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	memcpy(bufi, data.indices, data.ind_c * sizeof(GLuint));
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void GDrawable::dispose() {
	glDeleteVertexArrays(1, &data.VAO);
	glDeleteBuffers(1, &data.VBO);
	glDeleteBuffers(1, &data.EBO);
	// 都在类里面了，一个free就完事了
}

GDrawable::GDrawable() {
	glGenVertexArrays(1, &data.VAO);
	glGenBuffers(1, &data.VBO);
	glGenBuffers(1, &data.EBO);
	data.vertices = NULL;
	data.indices = NULL;
	data.model = glm::mat4(1.0);
}
