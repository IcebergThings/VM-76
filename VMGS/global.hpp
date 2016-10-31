#include <VMDE/VMDE.hpp>

#ifndef _INCLUDE_VMGS_GLOBAL_H
#define _INCLUDE_VMGS_GLOBAL_H

namespace VM76 {
	class Tile {
	public:
		GDrawable* obj = NULL;

	private:
		GLfloat *vtx = NULL;
		GLuint *itx = NULL;

	public:
		Tile(int tid);
	};

	extern Tile* t[16];

	const GLfloat tile_size = 1.0f / 16.0f;

	void loop();
	void start_game();
	void terminate();

}

extern "C" {
	void client_terminate();

	void i_have_a_key(GLFWwindow* window UNUSED,
		int key UNUSED, int scancode UNUSED, int action UNUSED, int mode UNUSED);
}

#endif
