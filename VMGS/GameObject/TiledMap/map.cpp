//=============================================================================
// ■ map.cpp
//-----------------------------------------------------------------------------
//   地图类
//=============================================================================

#include "map.hpp"

namespace VM76 {

	Map::Map(int w, int h, int l, int csize) {
		log("Init map with size %d, %d, %d in chunk size %d", w, l, h, csize);
		CHUNK_SIZE = csize;
		width = w; length = l; height = h;
		bw = CHUNK_SIZE * w; bl = CHUNK_SIZE * l; bh = CHUNK_SIZE * h;

		chunks = new TiledMap*[w * l * h];
		for (int x = 0; x < width; x++)
			for (int z = 0; z < length; z++) {
				log("Load Chunks group X:%d Z:%d", x, z);

				int y = 0, ind = calcChunkIndex(x,0,z);
				chunks[ind] = new TiledMap(
					CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,
					glm::vec3(CHUNK_SIZE * x, CHUNK_SIZE * y, CHUNK_SIZE * z)
				);
				chunks[ind]->generate_flat();
				chunks[ind]->bake_tiles();

				for (y = y + 1; y < height; y++) {
					ind = calcChunkIndex(x,y,z);
					chunks[ind] = new TiledMap(
						CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,
						glm::vec3(CHUNK_SIZE * x, CHUNK_SIZE * y, CHUNK_SIZE * z)
					);
					chunks[ind]->generate_void();
					chunks[ind]->bake_tiles();
				}
			}

		log("Map initialized");
	}

	void Map::place_block(glm::vec3 dir, int tid) {
		int cx = (int) dir.x / CHUNK_SIZE;
		int cy = (int) dir.y / CHUNK_SIZE;
		int cz = (int) dir.z / CHUNK_SIZE;

		int bx = (int) dir.x - cx * CHUNK_SIZE;
		int by = (int) dir.y - cy * CHUNK_SIZE;
		int bz = (int) dir.z - cz * CHUNK_SIZE;

		int ind = calcChunkIndex(cx,cy,cz);
		chunks[ind]->tiles[chunks[ind]->calcTileIndex(bx, by, bz)].tid = tid;
		chunks[ind]->bake_tiles();
	}

	void Map::render() {
		for (int x = 0; x < width * length * height; x++)
			chunks[x]->render();
	}

	void Map::dispose() {
		for (int x = 0; x < width * length * height; x++) VMDE_Dispose(chunks[x]);
		xefree(chunks);
	}

	int Map::calcChunkIndex(int x, int y, int z) {
		return (width * length) * y + (length) * z + x;
	}
}
