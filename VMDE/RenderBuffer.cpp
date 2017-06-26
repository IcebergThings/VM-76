//=============================================================================
// ■ RenderBuffer.cpp
//-----------------------------------------------------------------------------
//   VMDE中渲染对象缓冲
//=============================================================================

#include "global.hpp"

RenderBuffer::RenderBuffer (int w, int h, int mrt, const GLuint* type) {
	log("Creating Render Buffer of %d x %d", w, h);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	mrtcount = mrt;
	texture_buffer = new Res::Texture*[mrt];
	for (int i = 0; i < mrt; i++) {
		texture_buffer[i] = new Res::Texture();
		glGenTextures(1, &(texture_buffer[i]->texture));
		glBindTexture(GL_TEXTURE_2D, texture_buffer[i]->texture);
		glTexImage2D(GL_TEXTURE_2D, 0, type[i], w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		texture_buffer[i]->width = w; texture_buffer[i]->height = h;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture_buffer[i]->texture, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) error("Framebuffer creation failed. Framebuffer is not complete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderBuffer::set_draw_buffers() {
	GLuint attachments[mrtcount];
	for (int i = 0; i < mrtcount; i++) attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	glDrawBuffers(mrtcount, attachments);
}

RenderBuffer::~RenderBuffer () {
	log("Delete Buffer %d", framebuffer);
	
	for (int i = 0; i < mrtcount; i++) XE(delete, texture_buffer[i]);
	XE(delete, texture_buffer);

	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &framebuffer);
}

GDrawable* PostProcessingManager::QuadScreen;

void PostProcessingManager::init() {
	Vertex* vtx = new Vertex[4] {
		{{-1.0, -1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{0.0,0.0},{0.0, 0.0, 0.0}},
		{{-1.0,  1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{0.0,1.0},{0.0, 0.0, 0.0}},
		{{ 1.0,  1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{1.0,1.0},{0.0, 0.0, 0.0}},
		{{ 1.0, -1.0, 0.0},{1.0, 1.0, 1.0, 1.0},{1.0,0.0},{0.0, 0.0, 0.0}},
	};
	GLuint* itx = new GLuint[6] { 0,1,3, 1,2,3 };

	QuadScreen = new GDrawable();
	QuadScreen->data.vtx_c = 4;
	QuadScreen->data.ind_c = 6;
	QuadScreen->data.vertices = vtx;
	QuadScreen->data.indices = itx;
	QuadScreen->data.mat_c = 0;
	QuadScreen->data.mat = NULL;
	QuadScreen->fbind();
}

void PostProcessingManager::Blit2D() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	QuadScreen->renderOnce();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}
