//=============================================================================
// ■ VMGS/global.hpp
//=============================================================================

#include <global.hpp>
#include <config.hpp>

#include "GameObject/GameObject.hpp"
#include "Control/control.hpp"

#ifndef _INCLUDE_VMGS_GLOBAL_H
	#define _INCLUDE_VMGS_GLOBAL_H

	#undef DEBUG_ENVIRONMENT
	#define DEBUG_ENVIRONMENT "VMGS"

	//-------------------------------------------------------------------------
	// ● TODO
	//-------------------------------------------------------------------------
	namespace VM76 {
		const GLfloat tile_size = 1.0f / 16.0f;

		class Block {
		public:
			GLuint tid;
			Block(int id);
		};

		void loop();
		void start_game();
		void terminate();
		void init_tiles();

	//-------------------------------------------------------------------------
	// ● Game Logic
	//-------------------------------------------------------------------------
		void init_logic();
		void update_logic();
		void render();
	}

#endif
