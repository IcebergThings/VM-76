//=============================================================================
// ■ tiled_map.hpp
//=============================================================================

#include <global.hpp>

#ifndef _INCLUDE_TILED_MAP_H
#define _INCLUDE_TILED_MAP_H

#include "../Cube/cube.hpp"

namespace VM76 {

	struct TileData {
		unsigned int tid;
		unsigned char data_flag;
	};

	class TiledMap {
	public:
		TileData* tiles;
		int width, length, height;

	private:
		Cube* cinstance[16];

	public:
		int calcTileIndex(int x, int y, int z);

		TiledMap(int x, int y, int z);
		void render();
		void generate_flat();
		void bake_tiles();
		void dispose();
	};
}

#endif
