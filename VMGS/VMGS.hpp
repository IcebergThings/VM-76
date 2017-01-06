//=============================================================================
// ■ VMGS/global.hpp
//=============================================================================

#include <global.hpp>

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
		enum TileList {
			GRASS,
			STONE,
			DIRT,
			GLASS,
			WOOD,
			STONE_BRICK,
			WALL,
			TNT,
			COBBLE_STONE,
		};

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
	}

#endif
