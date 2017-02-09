//=============================================================================
// ■ map.hpp
//-----------------------------------------------------------------------------
//   地图类
//=============================================================================

#ifndef _INCLUDE_MAP_H
#define _INCLUDE_MAP_H

#include "tiled_map.hpp"

namespace VM76 {

	class Map : public RenderObject {
	public:
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
