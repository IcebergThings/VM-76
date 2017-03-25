//=============================================================================
// ■ GodView.hpp
//=============================================================================

#include "control.hpp"

#ifndef INCLUDE_CONTROL_GOD_VIEW_
#define INCLUDE_CONTROL_GOD_VIEW_

namespace VM76 {

class GodView : public Control {
public:

	struct Game_control {
		int key_quit;
		int key_forward, key_left, key_right, key_back;
		int key_zoomin, key_zoomout;
	} game;

	class Camera {
	public:
		glm::vec3 wpos = glm::vec3(0.0, 0.0, 0.0);
	} cam;

public:
	void update_control();
	void init_control();

};

}

#endif
