#include <global.hpp>
#include <config.hpp>

namespace VM76 {
	struct Game_control {
		int
		key_quit,
		key_forward, key_left, key_right, key_back,
		key_zoomin, key_zoomout;
	};
	extern Game_control game;

	class Camera {
	public:
		glm::vec3 wpos = glm::vec3(0.0, 0.0, 0.0);
	};
	extern Camera cam;
}
