//=============================================================================
// ■ text.cpp
//-----------------------------------------------------------------------------
//   VMDE中与文字渲染的部分。
//=============================================================================

#include "global.hpp"

TextRenderer::TextRenderer() {
	obj = new GDrawable();

	obj->data.vtx_c = 0;
	obj->data.ind_c = 0;
	obj->data.vertices = NULL;
	obj->data.indices = NULL;
	obj->data.mat_c = 1;
	obj->data.mat = NULL;
	obj->fbind();

	tex = new Res::Texture("../Media/Font.bmp");
	texshader = new Shaders("../Media/shaders/font.fsh", "../Media/shaders/font.vsh");
}

void TextRenderer::BakeText(const char* text, float width, float height) {
	int length = strlen(text);

	Vertex* vtx = new Vertex[length * 4];
	GLuint* itx = new GLuint[length * 6];

	float lbx = 0.0;
	for (int i = 0; i < length; i++) {
		char c = text[i];
		const float w = 1.0 / 64.0f;
		const float h = 1.0 / 32.0f;

		float stx = (float) (c % 64) * w;
		float sty = (float) (c / 64) * h;

		vtx[i * 4 + 0] = {{lbx, 0.0, 0.0},     {1.0,1.0,1.0,1.0}, {stx,sty + h}, {0.0,0.0,0.0}};
		vtx[i * 4 + 1] = {{lbx, h, 0.0},       {1.0,1.0,1.0,1.0}, {stx,sty    }, {0.0,0.0,0.0}};
		vtx[i * 4 + 2] = {{lbx + w, h, 0.0},   {1.0,1.0,1.0,1.0}, {stx + w,sty}, {0.0,0.0,0.0}};
		vtx[i * 4 + 3] = {{lbx + w, 0.0, 0.0}, {1.0,1.0,1.0,1.0}, {stx+w,sty+h}, {0.0,0.0,0.0}};

		itx[i * 6 + 0] = i * 4 + 0;
		itx[i * 6 + 1] = i * 4 + 1;
		itx[i * 6 + 2] = i * 4 + 3;
		itx[i * 6 + 3] = i * 4 + 1;
		itx[i * 6 + 4] = i * 4 + 2;
		itx[i * 6 + 5] = i * 4 + 3;
	}

	obj->data.vtx_c = length * 4;
	obj->data.ind_c = length * 6;
	obj->data.vertices = vtx;
	obj->data.indices = itx;
	obj->data.mat_c = 1;
	obj->data.mat = NULL;

	obj->update();

	free(vtx); free(itx);
}

void TextRenderer::render() {
	glDisable(GL_CULL_FACE);
	texshader->use();
	texshader->set_texture("fontmap", tex, 0);
	obj->draw();
	glEnable(GL_CULL_FACE);
}

void TextRenderer::instanceRenderText(const char* text, glm::mat4 projection, glm::mat4 view, float width, float height) {
	BakeText(text, width, height);
	texshader->ProjectionView(projection, view);
	render();
}

void TextRenderer::dispose() {
	VMDE_Dispose(obj);
	VMDE_Dispose(tex);
	//VMDE_Dispose(texshader);
}
