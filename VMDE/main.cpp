//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   处理主渲染循环等主要过程。
//=============================================================================

#include "VMDE.hpp"
#include "GDrawable.hpp"

//-----------------------------------------------------------------------------
// ● 为GLFW提供的通用错误回调函数
//-----------------------------------------------------------------------------
void glfw_error_callback(int error, const char* description) {
	log("GLFW error %d: %s", error, description);
}

//-----------------------------------------------------------------------------
// ● TODO: 这是啥
//-----------------------------------------------------------------------------
glm::mat4 view;

time_t fps_since = time(NULL);
int fps_counter = 0;
time_t accumulated_frame_time = 0;

//-----------------------------------------------------------------------------
// ● 渲染
//-----------------------------------------------------------------------------
void main_draw_loop() {
	VMDE->frame_count++;
	fps_counter++;
	time_t now = time(NULL);
	if (difftime(now, fps_since) >= 1.0d) {
		VMDE->fps = fps_counter;
		fps_counter = 0;
		fps_since = now;
		VMDE->frame_time = accumulated_frame_time / double((VMDE->fps)) * 1000.0;
		accumulated_frame_time -= 1.0d;
	}
	if (!VMDE->state.frozen) {
		double cr = 0.2f * VMDE->state.brightness,
			cg = 0.3f * VMDE->state.brightness,
			cb = 0.3f * VMDE->state.brightness;
		glClearColor(cr, cg, cb, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/* Render */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Activate Shader
		glUseProgram(main_shader->shaderProgram);

		// Setup uniforms
		GLint model_location;
		model_location = glGetUniformLocation(main_shader->shaderProgram, "viewMatrix");
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(view));
		model_location = glGetUniformLocation(main_shader->shaderProgram, "brightness");
		glUniform1f(model_location, VMDE->state.brightness);

		// Setup textures
		for (int index = 0; index < 16; index++) if (Res::tex_unit[index]) {
			char* uniform_name = new char[16];
			sprintf(uniform_name, "colortex%d", index);
			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(GL_TEXTURE_2D, Res::tex_unit[index]->texture);
			glUniform1i(glGetUniformLocation(main_shader->shaderProgram, (GLchar*) uniform_name), index);
			xefree(uniform_name);
		}


		struct RenderChainNode* chain = render_chain;
		while (chain) {
			VALUE v UNUSED = chain->n; // TODO: use it!

			GDrawable::draw(chain->gd);

			chain = chain->next;
		}
		glDisable(GL_BLEND);
		glFlush();

		accumulated_frame_time += difftime(time(NULL), now);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

//-----------------------------------------------------------------------------
// ● Matrix for 2D (GUI), depth from -100 to 100, X = 0 ~ Width, Y = 0 ~ Height
//-----------------------------------------------------------------------------
void matrix2D() {
	view = glm::ortho(0.0f, float(VMDE->width), 0.0f, float(VMDE->height), -100.0f, 100.0f);
}
