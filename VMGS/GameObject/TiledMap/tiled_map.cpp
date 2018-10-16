//=============================================================================
// ■ tiled_map.cpp
//-----------------------------------------------------------------------------
//   Tile地图
//=============================================================================

#include "tiled_map.hpp"

namespace VM76 {

	void TiledMap::init_cinstances(Tiles* cinstance[]) {
		for (int i = 0; i < 16; i++)
			cinstance[i] = get_instances(i + 1);
	}

	Tiles* TiledMap::get_instances(int id) {
		switch (id) {
			case Grass:
				return new MultiFaceCubeTile(49, 49, 0, 2, 49, 49);
			case Stone:
				return new SimpleCubeTile(1);
			case Dirt:
				return new SimpleCubeTile(2);
			case Glass:
				return new SimpleCubeTile(3);
			case WoodPlank:
				return new SimpleCubeTile(4);
			case HalfBrick:
				return new MultiFaceCubeTile(5, 5, 6, 6, 5, 5);
			case Brick:
				return new SimpleCubeTile(7);
			case TNT:
				return new MultiFaceCubeTile(8, 8, 9, 10, 8, 8);
			case CobbleStone:
				return new SimpleCubeTile(16);
			default:
				return new SimpleCubeTile(31);
		}
	}

	TiledMap::TiledMap(int x, int y, int z, glm::vec3 wp, DataMap* m) {
		width = x; length = z; height = y;

		//init_cinstances(cinstance);
		memset(cinstance, 0, sizeof(cinstance));
		init_cinstances(cinstance);
		map = m;

		mount_point = wp;

		obj = NULL;
	}

	#define CHECK_EXPAND \
	if (vtx_c + 4 > vtx_max) {vtx_max *= 2; vtx = (Vertex*) realloc(vtx, sizeof(Vertex) * vtx_max);}\
	if (ind_c + 6 > ind_max) {ind_max *= 2; ind = (GLuint*) realloc(ind, sizeof(GLuint) * ind_max);}\

	void TiledMap::bake_tiles() {
		int count = 0;

		// Pre alloc
		Vertex* vtx = (Vertex*)malloc(sizeof(Vertex) * 512);
		GLuint* ind = (GLuint*)malloc(sizeof(GLuint) * 512);
		int vtx_max = 512, ind_max = 512;
		int vtx_c = 0, ind_c = 0;

		for (int x = mount_point.x; x < mount_point.x + width; x++) {
			for (int z = mount_point.z; z < length + mount_point.z; z++) {
				for (int y = mount_point.y; y < height + mount_point.y; y++) {
					int id = map->tidQuery(x, y, z);
					if (id > 0) {
						id--;
						float x0 = x - mount_point.x;
						float y0 = y - mount_point.y;
						float z0 = z - mount_point.z;

						if (Tiles::is_transperant(map->tidQuery(x, y, z - 1))) {
							CHECK_EXPAND
							cinstance[id]->bake(x0, y0, z0, vtx, ind, &vtx_c, &ind_c, 0);
							count++;
						}
						if (Tiles::is_transperant(map->tidQuery(x, y, z + 1))) {
							CHECK_EXPAND
							cinstance[id]->bake(x0, y0, z0, vtx, ind, &vtx_c, &ind_c, 1);
							count++;
						}
						if (Tiles::is_transperant(map->tidQuery(x, y + 1, z))) {
							CHECK_EXPAND
							cinstance[id]->bake(x0, y0, z0, vtx, ind, &vtx_c, &ind_c, 2);
							count++;
						}
						if (Tiles::is_transperant(map->tidQuery(x, y - 1, z))) {
							CHECK_EXPAND
							cinstance[id]->bake(x0, y0, z0, vtx, ind, &vtx_c, &ind_c, 3);
							count++;
						}
						if (Tiles::is_transperant(map->tidQuery(x - 1, y, z))) {
							CHECK_EXPAND
							cinstance[id]->bake(x0, y0, z0, vtx, ind, &vtx_c, &ind_c, 4);
							count++;
						}
						if (Tiles::is_transperant(map->tidQuery(x + 1, y, z))) {
							CHECK_EXPAND
							cinstance[id]->bake(x0, y0, z0, vtx, ind, &vtx_c, &ind_c, 5);
							count++;
						}

					}
				}
			}
		}
		//log("%d < %d : %d < %d", vtx_c, vtx_max, ind_c, ind_max);
		translate = glm::translate(glm::mat4(1.0), mount_point);

		is_valid = count > 0;
		if (is_valid) {
			if (!obj) obj = new GDrawable(NULL, NULL);
			obj->data.vtx_c = vtx_c;
			obj->data.ind_c = ind_c;
			obj->data.vertices = (GLuint*)vtx;
			obj->data.indices = ind;
			obj->data.mat_c = 1;
			obj->data.mat = (GLuint*) &translate;
			obj->fbind();
		} else {
			if (obj) XE(delete, obj);
		}
	}

	void TiledMap::render() {
		if (is_valid) obj->renderOnce();
	}

	GDrawable* TiledMap::getBatch() {
		if (is_valid) return obj;
		return NULL;
	}

	TiledMap::~TiledMap() {
		for (int i = 0; i < 16; i++)
			if (cinstance[i]) XE(delete, cinstance[i]);
	}
}
