//=============================================================================
// ■ text.cpp
//-----------------------------------------------------------------------------
//   VMDE中与文字渲染有关的部分。
//=============================================================================

#include "global.hpp"

GLuint Text_VBO_size[] = {3, 4, 2};
GLuint Text_VBO_type[] = {GL_FLOAT, GL_FLOAT, GL_FLOAT};

struct VBO_Entry_Descriptor Text_VBO {
	9 * sizeof(GLfloat),
	3,
	Text_VBO_size,
	Text_VBO_type,
	false,
	NULL
};

typedef struct {
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 uv;
} TextVertex;

TextRenderer::TextRenderer() {
	obj = new GDrawable(&Text_VBO, NULL);
	texshader.add_file(GL_VERTEX_SHADER, "../Media/shaders/text.vsh");
	texshader.add_file(GL_FRAGMENT_SHADER, "../Media/shaders/text.fsh");
	texshader.link_program();
	check_gl_error;
}

TextRenderer::~TextRenderer() {
	XE(delete, obj);
}

void TextRenderer::BakeText(
	const char* text,
	float width, float height,
	TextDecorationType decoration
) {
	int length = strlen(text);
	int vtx_stride, itx_stride;
	switch (decoration) {
	case NONE:
		vtx_stride = 4;
		itx_stride = 6;
		break;
	case SHADOW:
		vtx_stride = 8;
		itx_stride = 12;
		break;
	case OUTLINE:
		vtx_stride = 20;
		itx_stride = 30;
		break;
	}
	TextVertex* vtx = (TextVertex*) malloc(sizeof(TextVertex) * vtx_stride * length);
	GLuint* itx = (GLuint*) malloc(sizeof(GLuint) * itx_stride * length);

	float lbx = 0.0f;

	for (int i = 0; i < length; i++) {
		char c = text[i];
		const float w = 1 / 32.0f;
		const float h = 1 / 8.0f;

		float stx = (float) (c & 0b11111) * w;
		float sty = (float) (c >> 5) * h;

		lbx = (float) (i) * width;

		TextVertex* vtxi = vtx + i * vtx_stride;
		GLuint* itxi = itx + i * itx_stride;
		#define ADD_VERTICES(j, ox, oy, r, g, b, a) do { \
			vtxi[(j) + 0] = {{lbx         + (ox),        + (oy), .0}, {r, g, b, a}, {stx    , sty + h}}; \
			vtxi[(j) + 1] = {{lbx         + (ox), height + (oy), .0}, {r, g, b, a}, {stx    , sty    }}; \
			vtxi[(j) + 2] = {{lbx + width + (ox), height + (oy), .0}, {r, g, b, a}, {stx + w, sty    }}; \
			vtxi[(j) + 3] = {{lbx + width + (ox),        + (oy), .0}, {r, g, b, a}, {stx + w, sty + h}}; \
		} while (false);
		#define ADD_INDEICES(j, a, b, c) do { \
			itxi[(j) + 0] = i * vtx_stride + (a); \
			itxi[(j) + 1] = i * vtx_stride + (b); \
			itxi[(j) + 2] = i * vtx_stride + (c); \
		} while (false);
		ADD_VERTICES(0, 0, 0, 1.0, 1.0, 1.0, 1.0);
		// sd = shadow distance
		float sd = 0.0016 * (float) VMDE->width / (float) VMDE->height;
		switch (decoration) {
		case NONE:
			break;
		case SHADOW:
			ADD_VERTICES(4, sd, -sd, 0.0, 0.0, 0.0, 0.8);
			break;
		case OUTLINE:
			ADD_VERTICES( 4, +sd, +sd, 0.0, 0.0, 0.0, 0.3);
			ADD_VERTICES( 8, +sd, -sd, 0.0, 0.0, 0.0, 0.3);
			ADD_VERTICES(12, -sd, +sd, 0.0, 0.0, 0.0, 0.3);
			ADD_VERTICES(16, -sd, -sd, 0.0, 0.0, 0.0, 0.3);
			break;
		}
		switch (decoration) {
		case NONE:
			ADD_INDEICES(0, 0, 1, 3);
			ADD_INDEICES(3, 1, 2, 3);
			break;
		case SHADOW:
			ADD_INDEICES(0, 4, 5, 7);
			ADD_INDEICES(3, 5, 6, 7);
			ADD_INDEICES(6, 0, 1, 3);
			ADD_INDEICES(9, 1, 2, 3);
			break;
		case OUTLINE:
			ADD_INDEICES(0, 4, 5, 7);
			ADD_INDEICES(3, 5, 6, 7);
			ADD_INDEICES(6, 8, 9, 11);
			ADD_INDEICES(9, 9, 10, 11);
			ADD_INDEICES(12, 12, 13, 15);
			ADD_INDEICES(15, 13, 14, 15);
			ADD_INDEICES(18, 16, 17, 19);
			ADD_INDEICES(21, 17, 18, 19);
			ADD_INDEICES(24, 0, 1, 3);
			ADD_INDEICES(27, 1, 2, 3);
			break;
		}
	}

	obj->data.vtx_c = length * vtx_stride;
	obj->data.ind_c = length * itx_stride;
	obj->data.vertices = (GLuint*) vtx;
	obj->data.indices = itx;

	obj->fbind();
	XE(free, vtx);
	XE(free, itx);
}

void TextRenderer::render() {
	VMSC::disable_cullface();
	texshader.use();
	texshader.set_texture("fontmap", &tex, 0);
	obj->render();
	//GDrawable::close_draw_node();
	VMSC::enable_cullface();
}

void TextRenderer::instanceRenderText(
	const char* text,
	glm::mat4 projection, glm::mat4 view, glm::mat4 transform,
	float width, float height, TextDecorationType decoration
) {
	texshader.ProjectionView(projection, view);

	glm::mat4 foo[1] = {transform};
	obj->data.mat_c = 1;
	obj->data.mat = (GLuint*) &foo[0];
	BakeText(text, width, height, decoration);

	render();
}
