//=============================================================================
// ■ VMGS/VMGS.hpp
//=============================================================================

#include <VMDE.hpp>

#include <config.hpp>

#include "GameObject/GameObject.hpp"
#include "Control/control.hpp"
#include "Physics/Physics.hpp"
#include "Scene/scene.hpp"

#ifndef _INCLUDE_VMGS_GLOBAL_H
namespace VM76 {
	#define _INCLUDE_VMGS_GLOBAL_H

	#undef DEBUG_ENVIRONMENT
	#define DEBUG_ENVIRONMENT "VMGS"

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
#endif
