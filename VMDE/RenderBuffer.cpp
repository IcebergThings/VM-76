//=============================================================================
// ■ RenderBuffer.cpp
//-----------------------------------------------------------------------------
//   VMDE中渲染对象缓冲
//=============================================================================

#include "global.hpp"

RenderBuffer::RenderBuffer(int w, int h, int mrt, const GLuint* type) {
	log("Creating Render Buffer of %d x %d", w, h);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	mrtcount = mrt;
	texture_buffer = new Res::Texture*[mrt + 1];
	// Always reserve the last space for depth
	for (int i = 0; i < mrt; i++) {
		texture_buffer[i] = new Res::Texture();
		glGenTextures(1, &(texture_buffer[i]->texture));
		VMSC::ChangeTexture2D(texture_buffer[i]->texture);
		glTexImage2D(GL_TEXTURE_2D, 0, type[i], w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		texture_buffer[i]->width = w; texture_buffer[i]->height = h;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture_buffer[i]->texture, 0);

		texture_buffer[i]->parameter = &Res::LinearTextureParameters;
	}
	//glBindTexture(GL_TEXTURE_2D, 0);

	// Generate depth tex
	{
		texture_buffer[mrt] = new Res::Texture();
		glGenTextures(1, &(texture_buffer[mrt]->texture));
		VMSC::ChangeTexture2D(texture_buffer[mrt]->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w, h, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture_buffer[mrt]->texture, 0);
		texture_buffer[mrt]->width = w; texture_buffer[mrt]->height = h;
		texture_buffer[mrt]->parameter = &Res::LinearTextureParameters;
	}

	// We use stencil 24 depth 8 internal format
	//glGenRenderbuffers(1, &rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, w, h);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Check errors
	log("glCheckFramebufferStatus() = 0x%x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		error("Framebuffer creation failed. Framebuffer is not complete");
	check_gl_error;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void RenderBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderBuffer::set_draw_buffers() {
	GLuint attachments[mrtcount];
	for (int i = 0; i < mrtcount; i++) attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	glDrawBuffers(mrtcount, attachments);
}

void RenderBuffer::clearBuffer(glm::vec4 c, bool color, bool depth, bool stencil) {
	glClearColor(c.r, c.g, c.b, c.a);
	GLuint bits = 0x0;
	if (color) bits |= GL_COLOR_BUFFER_BIT;
	if (depth) {
		bits |= GL_DEPTH_BUFFER_BIT;
		VMStateControl::enable_depth_test();
	}
	if (stencil) {
		bits |= GL_STENCIL_BUFFER_BIT;
		glStencilMask(~0);
		VMStateControl::enable_stencil_test();
	}
	glClear(bits);
}

RenderBuffer::~RenderBuffer() {
	log("Delete Buffer %d", framebuffer);

	for (int i = 0; i < mrtcount + 1; i++) XE(delete, texture_buffer[i]);
	XE(delete, texture_buffer);

	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &framebuffer);
}

GDrawable* PostProcessingManager::QuadScreen;

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

void PostProcessingManager::init() {
	glm::vec3* vtx = new glm::vec3[4] {
		{-1.0, -1.0, 0.0},
		{-1.0,  1.0, 0.0},
		{ 1.0,  1.0, 0.0},
		{ 1.0, -1.0, 0.0},
	};
	GLuint* itx = new GLuint[6] { 0,1,3, 1,2,3 };

	QuadScreen = new GDrawable(&Simple_VBO, NULL);
	QuadScreen->data.vtx_c = 4;
	QuadScreen->data.ind_c = 6;
	QuadScreen->data.vertices = (GLuint*)vtx;
	QuadScreen->data.indices = itx;
	QuadScreen->data.mat_c = 0;
	QuadScreen->data.mat = NULL;
	QuadScreen->fbind();
}

void PostProcessingManager::Blit2D() {
	VMSC::disable_depth_test();
	VMSC::disable_cullface();
	QuadScreen->renderOnce();
	VMSC::enable_depth_test();
	VMSC::enable_cullface();
}
