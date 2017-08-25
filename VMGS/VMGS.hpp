//=============================================================================
// ■ VMGS/VMGS.hpp
//=============================================================================

#pragma once

#include <VMDE.hpp>
#undef DEBUG_ENVIRONMENT
#define DEBUG_ENVIRONMENT "VMGS"

#include "Game/config.hpp"

namespace VM76 {
	//-------------------------------------------------------------------------
	// ● TODO
	//-------------------------------------------------------------------------
	void loop();
	void start_game();
	void terminate();
	void init_tiles();

	extern TextRenderer* trex;
	extern glm::mat4 gui_2d_projection;
	extern float aspect_ratio;

	//-------------------------------------------------------------------------
	// ● Game Logic
	//-------------------------------------------------------------------------
	void init_logic();
	void update_logic();
	void render();
}

#include "Physics/Physics.hpp"
#include "GameObject/GameObject.hpp"
#include "Control/control.hpp"
#include "Scene/scene.hpp"
