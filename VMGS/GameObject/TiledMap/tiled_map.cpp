//=============================================================================
// ■ tiled_map.cpp
//-----------------------------------------------------------------------------
//   Tile地图
//=============================================================================

#include "tiled_map.hpp"

namespace VM76 {

	void TiledMap::init_cinstances (Tiles* cinstance[]) {
		for (int i = 0; i < 16; i ++)
			cinstance[i] = get_instances(i + 1);
	}

	Tiles* TiledMap::get_instances (int id) {
		switch (id) {
			case Grass:
				return new MultiFaceCubeTile(49,49,0,2,49,49);
				break;
			case Stone:
				return new SimpleCubeTile(1);
				break;
			case Dirt:
				return new SimpleCubeTile(2);
				break;
			case Glass:
				return new SimpleCubeTile(3);
				break;
			case WoodPlank:
				return new SimpleCubeTile(4);
				break;
			case HalfBrick:
				return new MultiFaceCubeTile(5,5,6,6,5,5);
				break;
			case Brick:
				return new SimpleCubeTile(7);
				break;
			case TNT:
				return new MultiFaceCubeTile(8,8,9,10,8,8);
				break;
			case CobbleStone:
				return new SimpleCubeTile(16);
				break;
			default:
				return new SimpleCubeTile(31);
				break;
		}
	}

	TiledMap::TiledMap(int x, int y, int z, glm::vec3 wp, DataMap* m) {
		width = x; length = z; height = y;

		//init_cinstances(cinstance);
		memset(cinstance, 0, sizeof(cinstance));
		init_cinstances(cinstance);
		map = m;

		mount_point = wp;
		
		obj = new GDrawable();
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
						id --;
						float x0 = x - mount_point.x;
						float y0 = y - mount_point.y;
						float z0 = z - mount_point.z;

						if (map->tidQuery(x, y, z - 1) == 0) {
							CHECK_EXPAND
							cinstance[id]->bake(x0, y0, z0, vtx, ind, &vtx_c, &ind_c, 0);
							count ++;
						}
						if (map->tidQuery(x, y, z + 1) == 0) {
							CHECK_EXPAND
							cinstance[id]->bake(x0, y0, z0, vtx, ind, &vtx_c, &ind_c, 1);
							count ++;
						}
						if (map->tidQuery(x, y + 1, z) == 0) {
							CHECK_EXPAND
							cinstance[id]->bake(x0, y0, z0, vtx, ind, &vtx_c, &ind_c, 2);
							count ++;
						}
						if (map->tidQuery(x, y - 1, z) == 0) {
							CHECK_EXPAND
							cinstance[id]->bake(x0, y0, z0, vtx, ind, &vtx_c, &ind_c, 3);
							count ++;
						}
						if (map->tidQuery(x - 1, y, z) == 0) {
							CHECK_EXPAND
							cinstance[id]->bake(x0, y0, z0, vtx, ind, &vtx_c, &ind_c, 4);
							count ++;
						}
						if (map->tidQuery(x + 1, y, z) == 0) {
							CHECK_EXPAND
							cinstance[id]->bake(x0, y0, z0, vtx, ind, &vtx_c, &ind_c, 5);
							count ++;
						}
						
					}
				}
			}
		}
		//log("%d < %d : %d < %d", vtx_c, vtx_max, ind_c, ind_max);
		translate = glm::translate(glm::mat4(1.0), mount_point);

		obj->data.vtx_c = vtx_c;
		obj->data.ind_c = ind_c;
		obj->data.vertices = vtx;
		obj->data.indices = ind;
		obj->data.mat_c = 1;
		obj->data.mat = (GLuint*) &translate;
		obj->fbind();
		
		is_valid = count > 0;
	}

	void TiledMap::render() {
		if (is_valid) obj->renderOnce();
	}

	TiledMap::~TiledMap() {
		for (int i = 0; i < 16; i++)
			if (cinstance[i]) VMDE_Dispose(delete, cinstance[i]);
	}
}
