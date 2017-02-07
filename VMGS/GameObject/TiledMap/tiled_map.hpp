//=============================================================================
// ■ tiled_map.hpp
//=============================================================================

#include <global.hpp>

#ifndef _INCLUDE_TILED_MAP_H
#define _INCLUDE_TILED_MAP_H

#include "tile.hpp"

namespace VM76 {

	struct TileData {
		unsigned int tid;
		unsigned char data_flag;
	};

	class TiledMap {
	public:
		TileData* tiles;
		int width, length, height;
		glm::vec3 mount_point = glm::vec3(0.0);

	private:
		Tiles* cinstance[16];

	public:
		int calcTileIndex(int x, int y, int z);
		int calcTileIndex(glm::vec3 pos);
		TileData tileQuery(int x, int y, int z);

		TiledMap(int x, int y, int z, glm::vec3 wp);
		void render();
		void generate_flat();
		void bake_tiles();
		void dispose();
	};
}

#endif
