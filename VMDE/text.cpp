//=============================================================================
// ■ text.cpp
//-----------------------------------------------------------------------------
//   VMDE中与文字渲染的部分。
//=============================================================================

#include "global.hpp"

TextRenderer::TextRenderer() {
	obj = new GDrawable();
	/*obj->data.vtx_c = 2000;
	obj->data.ind_c = 2000;
	obj->data.vertices = NULL;
	obj->data.indices = NULL;
	obj->data.mat_c = 10;
	obj->data.mat = NULL;
	obj->fbind();
	obj->data.mat_c = 0;*/

	tex = new Res::Texture("../Media/Font.bmp", true);
	texshader = Shaders::CreateFromFile("../Media/shaders/text.vsh", "../Media/shaders/text.fsh");
}

void TextRenderer::BakeText(const char* text, float width, float height, bool shadow) {
	int length = strlen(text);

	Vertex* vtx = new Vertex[shadow ? length * 8 : length * 4];
	GLuint* itx = new GLuint[shadow ? length * 12 : length * 6];

	float lbx = 0.0;
	int vtx_stride = shadow ? 8 : 4;
	int itx_stride = shadow ? 12 : 6;

	for (int i = 0; i < length; i++) {
		char c = text[i];
		const float w = 1.0 / 32.0f;
		const float h = 1.0 / 8.0f;

		float stx = (float) (c % 32) * w;
		float sty = (float) (c >> 5) * h;

		lbx = (float) (i) * width;

		vtx[i * vtx_stride + 0] = {{lbx, 0.0, 0.0},         {1.0,1.0,1.0,1.0}, {stx,sty + h}, {0.0,0.0,0.0}};
		vtx[i * vtx_stride + 1] = {{lbx, height, 0.0},      {1.0,1.0,1.0,1.0}, {stx,sty    }, {0.0,0.0,0.0}};
		vtx[i * vtx_stride + 2] = {{lbx+width, height, 0.0},{1.0,1.0,1.0,1.0}, {stx + w,sty}, {0.0,0.0,0.0}};
		vtx[i * vtx_stride + 3] = {{lbx+width, 0.0, 0.0},   {1.0,1.0,1.0,1.0}, {stx+w,sty+h}, {0.0,0.0,0.0}};

		float sd = 0.0016 * (float) VMDE->width / (float) VMDE->height;
		if (shadow) {
			vtx[i * vtx_stride + 4] = {{lbx + sd, - sd, 0.0},         {.0,.0,.0,.8}, {stx,sty + h}, {0.0,0.0,0.0}};
			vtx[i * vtx_stride + 5] = {{lbx + sd, height - sd, 0.0},  {.0,.0,.0,.8}, {stx,sty    }, {0.0,0.0,0.0}};
			vtx[i * vtx_stride + 6] = {{lbx+width+sd, height-sd, 0.0},{.0,.0,.0,.8}, {stx + w,sty}, {0.0,0.0,0.0}};
			vtx[i * vtx_stride + 7] = {{lbx+width+sd, -sd, 0.0},      {.0,.0,.0,.8}, {stx+w,sty+h}, {0.0,0.0,0.0}};
		}

		if (shadow) {
			itx[i * itx_stride + 0] = i * vtx_stride + 4;
			itx[i * itx_stride + 1] = i * vtx_stride + 5;
			itx[i * itx_stride + 2] = i * vtx_stride + 7;
			itx[i * itx_stride + 3] = i * vtx_stride + 5;
			itx[i * itx_stride + 4] = i * vtx_stride + 6;
			itx[i * itx_stride + 5] = i * vtx_stride + 7;
			itx[i * itx_stride + 6] = i * vtx_stride + 0;
			itx[i * itx_stride + 7] = i * vtx_stride + 1;
			itx[i * itx_stride + 8] = i * vtx_stride + 3;
			itx[i * itx_stride + 9] = i * vtx_stride + 1;
			itx[i * itx_stride + 10] = i * vtx_stride + 2;
			itx[i * itx_stride + 11] = i * vtx_stride + 3;
		} else {
			itx[i * itx_stride + 0] = i * vtx_stride + 0;
			itx[i * itx_stride + 1] = i * vtx_stride + 1;
			itx[i * itx_stride + 2] = i * vtx_stride + 3;
			itx[i * itx_stride + 3] = i * vtx_stride + 1;
			itx[i * itx_stride + 4] = i * vtx_stride + 2;
			itx[i * itx_stride + 5] = i * vtx_stride + 3;
		}
	}

	obj->data.vtx_c = length * vtx_stride;
	obj->data.ind_c = length * itx_stride;
	obj->data.vertices = vtx;
	obj->data.indices = itx;

	obj->fbind();
	free(vtx); free(itx);
}

void TextRenderer::render() {
	glDisable(GL_CULL_FACE);
	texshader->use();
	texshader->set_texture("fontmap", tex, 0);
	obj->draw();
	glEnable(GL_CULL_FACE);
}

void TextRenderer::instanceRenderText(
		const char* text,
		glm::mat4 projection, glm::mat4 view, glm::mat4 transform,
		float width, float height, bool shadow
) {
	texshader->ProjectionView(projection, view);

	glm::mat4 foo[1] = {transform};
	obj->data.mat_c = 1;
	obj->data.mat = (GLuint*) &foo[0];
	BakeText(text, width, height, shadow);

	render();
}

void TextRenderer::dispose() {
	VMDE_Dispose(obj);
	VMDE_Dispose(tex);
	VMDE_Dispose(texshader);
}
