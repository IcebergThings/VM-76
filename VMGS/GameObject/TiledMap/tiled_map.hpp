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

		glm::mat4 transform;
	};

	#define TERRIAN_MAX_HEIGHT 60

	class DataMap : public Object {
	public:
		TileData* map = NULL;
		int width, length, height;

	public:
		DataMap(int w, int h, int l);
		TileData tileQuery(int x, int y, int z);

		inline int calcIndex(int x, int y, int z) { return (width * length) * y + (length) * z + x; }
		inline int calcIndex(glm::vec3 v) { return (width * length) * v.y + (length) * v.z + v.x; }

		void generate_flat();
		void generate_V1();
	};

	class TiledMap : public RenderObject {
	public:
		DataMap* map = NULL;
		int width, length, height;
		glm::vec3 mount_point = glm::vec3(0.0);

	private:
		Tiles* cinstance[16];

	public:
		inline int calcTileIndex(int x, int y, int z) { return (width * length) * y + (length) * z + x; }
		inline int calcTileIndex(glm::vec3 pos) { return (width * length) * pos.y + (length) * pos.z + pos.x; }
		TileData tileQuery(int x, int y, int z);

		static void init_cinstances(Tiles* cinstance[]);

		TiledMap(int x, int y, int z, glm::vec3 wp, DataMap* m);
		void render();
		void bake_tiles();
		void dispose();
	};

	class Map : public RenderObject {
	public:
		DataMap* map = NULL;
		TiledMap** chunks = NULL;
		int width, length, height; // chunk as unit
		int bw, bl, bh; // block as unit
		int CHUNK_SIZE;

	public:
		Map(int w, int h, int l, int csize);
		void place_block(glm::vec3 dir, int tid);
		void render();
		void dispose();

		int calcChunkIndex(int x, int y, int z);
	};
}

#endif
