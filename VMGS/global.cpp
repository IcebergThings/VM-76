#include "global.hpp"

namespace VM76 {
	Game game = {
		.forward = GLFW_KEY_W,
		.left = GLFW_KEY_A,
		.right = GLFW_KEY_D,
		.back = GLFW_KEY_S,
	};
	Player game_player;
}
