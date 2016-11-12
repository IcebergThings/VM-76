#include "global.hpp"

namespace VM76 {
	Game game = {
		.key_quit = GLFW_KEY_ESCAPE,
		.key_forward = GLFW_KEY_W,
		.key_left = GLFW_KEY_A,
		.key_right = GLFW_KEY_D,
		.key_back = GLFW_KEY_S,
		.key_up = GLFW_KEY_SPACE,
		.key_down = GLFW_KEY_LEFT_SHIFT,
	};
	Player game_player;
}
