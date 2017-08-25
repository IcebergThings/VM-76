//=============================================================================
// ■ GDrawable.cpp
//-----------------------------------------------------------------------------
//   VMDE中GDrawable的控制与类。
//=============================================================================

#include <VMDE.hpp>

GLuint Vertex_entry_size[] = {3, 4, 2, 3};
GLuint Vertex_entry_type[] = {GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT};

struct VBO_Entry_Descriptor default_Vertex {
	12 * sizeof(GLfloat),
	4,
	Vertex_entry_size,
	Vertex_entry_type,
	false,
	NULL
};

GLuint MBO_entry_size[] = {4, 4, 4, 4};
GLuint MBO_entry_type[] = {GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT};
GLuint MBO_entry_divisor[] = {1, 1, 1, 1};

struct VBO_Entry_Descriptor default_MBO {
	16 * sizeof(GLfloat),
	4,
	MBO_entry_size,
	MBO_entry_type,
	true,
	MBO_entry_divisor
};

void GDrawable::renderOnce() {
	VMSC::ChangeVertexArray(data.VAO);
	glDrawElements(GL_TRIANGLES, data.ind_c, GL_UNSIGNED_INT, 0);
}

void GDrawable::render() {
	VMSC::ChangeVertexArray(data.VAO);
	glDrawElementsInstanced(
		GL_TRIANGLES, data.ind_c, GL_UNSIGNED_INT, 0, data.mat_c);
}

size_t _internal_attribute_size(GLuint type) {
	if (type == GL_FLOAT) {
		return sizeof(GLfloat);
	} else if (type == GL_INT) {
		return sizeof(GLint);
	} else if (type == GL_UNSIGNED_INT) {
		return sizeof(GLuint);
	} else
		return sizeof(GLvoid*);
}

void GDrawable::fbind() {
	VMSC::ChangeVertexArray(data.VAO);
	VMSC::ChangeElementArrayBuffer(data.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.ind_c * sizeof(GLuint), data.indices, GL_STATIC_DRAW);

	//const size_t vec4Size = sizeof(glm::vec4);
	GLuint attr_index = 0;
	GLuint stream = 0;

	VMSC::ChangeArrayBuffer(data.VBO);
	glBufferData(GL_ARRAY_BUFFER, data.vtx_c * vbo->entry_length, data.vertices, GL_STATIC_DRAW);

	if (VMDE->gl_ver == GL_43) {
		// GL43 Vertex Attribute Format & Binding

		size_t offset = 0;
		for (int i = 0; i < vbo->attribute_count; i++) {
			GLuint type = vbo->attribute_type[i];
			GLuint size = vbo->attribute_size[i];
			glVertexAttribFormat(attr_index, size, type, false, offset);
			offset += size * _internal_attribute_size(type);

			glVertexAttribBinding(attr_index, stream);
			glEnableVertexAttribArray(attr_index);
			if (vbo->is_instanced) glVertexAttribDivisor(attr_index, vbo->divisors[i]);
			attr_index++;
		}

		glBindVertexBuffer(stream, data.VBO, 0, offset);
	} else {
		// GL33 Vertex Attribute Pointer
		size_t offset = 0;
		for (int i = 0; i < vbo->attribute_count; i++) {
			GLuint type = vbo->attribute_type[i];
			GLuint size = vbo->attribute_size[i];
			glVertexAttribPointer(attr_index, size, type, GL_FALSE, vbo->entry_length, (GLvoid*) offset);
			offset += size * _internal_attribute_size(type);

			glEnableVertexAttribArray(attr_index);
			if (vbo->is_instanced) glVertexAttribDivisor(attr_index, vbo->divisors[i]);
			attr_index++;
		}
	}
	// GL33 Vertex Attribute Pointer Instanced
	VMSC::ChangeArrayBuffer(data.MBO);
	glBufferData(GL_ARRAY_BUFFER, data.mat_c * sizeof(glm::mat4), data.mat, GL_DYNAMIC_DRAW);

	size_t offset = 0;
	for (int i = 0; i < mbo->attribute_count; i++) {
		GLuint type = mbo->attribute_type[i];
		GLuint size = mbo->attribute_size[i];
		glVertexAttribPointer(attr_index, size, type, GL_FALSE, mbo->entry_length, (GLvoid*) offset);
		offset += size * _internal_attribute_size(type);

		glEnableVertexAttribArray(attr_index);
		if (mbo->is_instanced) glVertexAttribDivisor(attr_index, mbo->divisors[i]);
		attr_index++;
	}

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//VMSC::ChangeVertexArray(0);
}

void GDrawable::update() {
	VMSC::ChangeVertexArray(data.VAO);

	VMSC::ChangeArrayBuffer(data.VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.vtx_c * sizeof(Vertex), data.vertices);

	VMSC::ChangeElementArrayBuffer(data.EBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data.ind_c * sizeof(GLuint), data.indices);
}

void GDrawable::update_instance() {
	VMSC::ChangeVertexArray(data.VAO);
	VMSC::ChangeArrayBuffer(data.MBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.mat_c * sizeof(glm::mat4), data.mat);
	//glBindBuffer(GL_ARRAY_BUFFER,0);
}

void GDrawable::update_instance_alien_size() {
	VMSC::ChangeVertexArray(data.VAO);
	VMSC::ChangeArrayBuffer(data.MBO);
	glBufferData(GL_ARRAY_BUFFER, data.mat_c * sizeof(glm::mat4), data.mat, GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER,0);
}

GDrawable::~GDrawable() {
	glDeleteVertexArrays(1, &data.VAO);
	glDeleteBuffers(1, &data.VBO);
	glDeleteBuffers(1, &data.EBO);
	glDeleteBuffers(1, &data.MBO);
}

GDrawable::GDrawable(struct VBO_Entry_Descriptor* vbo, struct VBO_Entry_Descriptor* mbo) {
	this->vbo = vbo ? vbo : &default_Vertex;
	this->mbo = mbo ? mbo : &default_MBO;

	glGenVertexArrays(1, &data.VAO);
	glGenBuffers(1, &data.VBO);
	glGenBuffers(1, &data.EBO);
	glGenBuffers(1, &data.MBO);
	data.vertices = (GLuint*)NULL;
	data.indices = NULL;
	data.mat = NULL;
}
