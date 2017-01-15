#include <global.hpp>
#include <config.hpp>

namespace VM76 {
	struct Game_control {
		int
		key_quit,
		key_forward, key_left, key_right, key_back,
		key_up, key_down;
	};
	extern Game_control game;

	class Player {
	public:
		bool in_gui = false;
		float horizontal_angle = 0.0;
		float vertical_angle = 0.0;
		float speed = 0.0;
		glm::vec3 wpos = glm::vec3(0.0, 1.0, 0.0);
	};
	extern Player game_player;
}
