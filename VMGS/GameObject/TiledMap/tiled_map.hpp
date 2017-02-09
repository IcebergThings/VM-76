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

	class TiledMap : public RenderObject {
	public:
		TileData* tiles;
		int width, length, height;
		glm::vec3 mount_point = glm::vec3(0.0);

	private:
		Tiles* cinstance[16];

	public:
		inline int calcTileIndex(int x, int y, int z) { return (width * length) * y + (length) * z + x; }
		inline int calcTileIndex(glm::vec3 pos) { return (width * length) * pos.y + (length) * pos.z + pos.x; }
		TileData tileQuery(int x, int y, int z);

		static void init_cinstances(Tiles* cinstance[]);

		TiledMap(int x, int y, int z, glm::vec3 wp);
		void render();
		void generate_flat();
		void generate_void();
		void bake_tiles();
		void dispose();
	};
}

#endif
