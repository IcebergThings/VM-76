//=============================================================================
// ■ FirstPersonView.hpp
//=============================================================================

#include "control.hpp"

#ifndef INCLUDE_CONTROL_FP_VIEW_
#define INCLUDE_CONTROL_FP_VIEW_

namespace VM76 {

class FirstPersonView : public Control {
public:

	struct Game_control {
		int
		key_quit,
		key_forward, key_left, key_right, key_back,
		key_up, key_down;
	} game;

	class Player {
	public:
		bool in_gui = false;
		float horizontal_angle = 0.0;
		float vertical_angle = 0.0;
		float speed = 0.0;
		glm::vec3 wpos = glm::vec3(0.0, 1.0, 0.0);
	} game_player;

public:
	void update_control();
	void init_control();

};

}

#endif
