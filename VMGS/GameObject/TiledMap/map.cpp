//=============================================================================
// ■ map.cpp
//-----------------------------------------------------------------------------
//   地图类
//=============================================================================

#include "../../VMGS.hpp"
#include "glm/gtc/noise.hpp"

namespace VM76 {

	DataMap::DataMap(int w, int h, int l) {
		constVoid = {255, 0};

		map = (TileData*) malloc(sizeof(TileData) * w * h * l);
		width = w; length = l; height = h;

		srand(time(NULL));

		if (!read_map("../userdata/map.dat")) generate_flat();
	}

	void DataMap::generate_flat() {
		for (int x = 0; x < width; x++)
			for (int z = 0; z < length; z++)
				for (int y = 0; y < height; y++) {
					map[index(x,y,z)].tid = (y == 0) ? Grass : Air;
				}
	}

	bool DataMap::read_map(const char* filename) {
		log("Reading map");
		V::BinaryFileReader fr(filename);
		if (!fr.is_valid()) {
			log("map.dat doesn't exist");
			return false;
		}

		int map_version = fr.read_i32();
		if (fr.read_u8() == 'V' && fr.read_u8() == 'M'
			&& fr.read_u8() == '7' && fr.read_u8() == '6') {
				log("Map version : %d", map_version);
				for (int x = 0; x < width * length * height; x++) {
					if (x % (width * length * height / 7) == 0)
						log("Map %d%% loaded", 100 * x / (width * length * height));
					map[x].tid = fr.read_u8();
					map[x].data_flag = fr.read_u8();
				}
				return true;
			} else {
				log("Invalid map.dat");
				return false;
			}
	}

	void DataMap::save_map(const char* filename) {
		log("Saving map to %s", filename);
		V::BinaryFileWriter fw(filename);
		// 版本号
		fw.write_i32(100);
		// 文件头标识
		fw.write_u8('V');
		fw.write_u8('M');
		fw.write_u8('7');
		fw.write_u8('6');

		for (int x = 0; x < width * length * height; x++) {
			fw.write_u8(map[x].tid);
			fw.write_u8(map[x].data_flag);
		}

		if (map_save_worker) {
			map_save_worker = NULL;
		}
		log("Map saved to %s", filename);
	}

	void DataMap::save_map_async(const char* filename) {
		if (!map_save_worker) {
			map_save_worker = new thread([] (DataMap* t, const char* filename) {
				t->save_map(filename);
			}, this, filename);
		}
	}

	DataMap::~DataMap() {
		if (map_save_worker) map_save_worker->join();
		XE(delete, map_save_worker);
	}

	void DataMap::generate_V1() {
		log("Start generating maps, %d x %d x %d", width, length, height);

		float seed = rand() * 0.00001f;
		for (int i = 0; i < width; i++) {
			if (i % (width / 12) == 0)
				log("Generated %d%% (%d / %d)",
					(int) (100.0 / width * i),
					i * length * height, width * length * height
				);

			for (int j = 0; j < length; j++) {
				glm::vec2 coord = glm::vec2(i, j) * 0.04f;

				const glm::mat2 rotate = glm::mat2(0.8387, -0.5446, 0.5446, 0.8387);

				glm::vec2 dir = glm::vec2(1.0, 0.1);
				float n = glm::sin(glm::perlin(coord * dir + seed)) * 0.5f; dir = rotate * dir;
				coord += n * dir * 0.6f;
				n += glm::perlin(coord * dir + seed) * 0.45f; dir = rotate * dir;
				coord += n * dir * 0.5f;
				n += glm::perlin(coord * dir + seed) * 0.35f; dir = rotate * dir;
				coord += n * dir * 0.4f;
				n += glm::perlin(coord * dir + seed) * 0.25f; dir = rotate * dir;
				coord += n * dir * 0.3f;
				n += glm::perlin(coord + seed) * 0.125f;

				n = glm::clamp(n * 0.4f + 0.4f, 1.0f / (float) TERRIAN_MAX_HEIGHT, 1.0f);
				int h = n * TERRIAN_MAX_HEIGHT;
				int ho = h;
				h = glm::clamp(h, 0, height);

				float dirt_layers = glm::perlin(coord * 5.0f + seed) * 2.0f;
				for (int y = 0; y < h; y++) {
					if (y == ho - 1)
						map[index(i,y,j)].tid = Grass;
					else if (y >= ho - 2 - dirt_layers && y < ho - 1)
						map[index(i,y,j)].tid = Dirt;
					else
						map[index(i,y,j)].tid = Stone;
				}
				for (int y = h; y < height; y++) {
					map[index(i,y,j)].tid = Air;
				}
			}
		}
		log("Generated 100%%, Complete");
	}

	Map::Map(int w, int h, int l, int csize) {
		log("Init map with size %d, %d, %d in chunk size %d", w, l, h, csize);
		CHUNK_SIZE = csize;
		width = w; length = l; height = h;
		bw = CHUNK_SIZE * w; bl = CHUNK_SIZE * l; bh = CHUNK_SIZE * h;

		chunks = new TiledMap*[w * l * h];

		map = new DataMap(bw, bh, bl);

		for (int x = 0; x < length; x++) {
			log("Baking Chunks: %d%% (%d / %d)",
				(int) (100.0 / length * x),
				x * width * height, width * length * height
			);
			for (int z = 0; z < width; z++)
				for (int y = 0; y < height; y++) {
					int ind = calcChunkIndex(x,y,z);
					chunks[ind] = new TiledMap(
						CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,
						glm::vec3(CHUNK_SIZE * x, CHUNK_SIZE * y, CHUNK_SIZE * z),
						map
					);
					chunks[ind]->bake_tiles();
				}
		}

		log("Baking Chunks: 100%%, map initialized");
		log("Preparing render command buffer");

		cmd_buffer = NULL;
		update_render_buffer();
	}

	void Map::update_render_buffer() {
		if (cmd_buffer) XE(delete, cmd_buffer);

		int max_count = width * length * height;
		size_t size = 0x10 + (max_count + 1) * sizeof(GDrawable*);
		ASM76::Instruct* cmd_buf = (ASM76::Instruct*) malloc(size);
		memset(cmd_buf, 0x0, size);
		cmd_buf[0] = {ASM76::INTX, CLEnum_GDrawable_batchOnce, 0x10};
		cmd_buf[1] = {ASM76::HALT, 0, 0};
		int real_count = 0;
		GDrawable** list = (GDrawable**) (((uint8_t*) cmd_buf) + 0x10);
		for (int i = 0; i < max_count; i++) {
			GDrawable* obj = chunks[i]->getBatch();
			if (obj) {
				list[real_count] = obj;
				real_count++;
			}
		}
		size = 0x10 + (real_count + 1) * sizeof(GDrawable*);
		cmd_buf = (ASM76::Instruct*) realloc(cmd_buf, size);
		cmd_buffer = new CmdList({{.size = size, .instruct = cmd_buf}});
	}

	void Map::place_block(glm::vec3 dir, int tid) {
		int cx = (int) dir.x / CHUNK_SIZE;
		int cy = (int) dir.y / CHUNK_SIZE;
		int cz = (int) dir.z / CHUNK_SIZE;

		map->map[map->index(dir)].tid = tid;
		chunks[calcChunkIndex(cx,cy,cz)]->bake_tiles();

		if (cx > 0      && (dir.x - 1.0) / CHUNK_SIZE != cx) chunks[calcChunkIndex(cx-1,cy,cz)]->bake_tiles();
		if (cx < width  && (dir.x + 1.0) / CHUNK_SIZE != cx) chunks[calcChunkIndex(cx+1,cy,cz)]->bake_tiles();
		if (cy > 0      && (dir.y - 1.0) / CHUNK_SIZE != cy) chunks[calcChunkIndex(cx,cy-1,cz)]->bake_tiles();
		if (cy < height && (dir.y + 1.0) / CHUNK_SIZE != cy) chunks[calcChunkIndex(cx,cy+1,cz)]->bake_tiles();
		if (cz > 0      && (dir.z - 1.0) / CHUNK_SIZE != cz) chunks[calcChunkIndex(cx,cy,cz-1)]->bake_tiles();
		if (cz < length && (dir.z + 1.0) / CHUNK_SIZE != cz) chunks[calcChunkIndex(cx,cy,cz+1)]->bake_tiles();

		update_render_buffer();
	}

	void Map::render() {
		material->ProjectionView(ActiveCamera->Projection, ActiveCamera->View);
		material->set_texture("colortex0", Res::TextureList["Game/TileAtlas"], 0);
		cmd_buffer->call();
	}

	Map::~Map() {
		for (int x = 0; x < width * length * height; x++) {
			XE(delete, chunks[x]);
		}
		XE(delete[], chunks);
		XE(delete, cmd_buffer);

		XE(delete, map);
	}

	PhysicsMap::PhysicsMap(Map* m) {
		this->robj = m;

		this->clipping_shell = {glm::vec3(0.0), glm::vec3(128.0)};
	}

	BoxCollider** PhysicsMap::get_collide_iterator(OBB* b) {
		int total = (1.0 + b->size.x) * (1.0 + b->size.y) * (1.0 + b->size.z);
		XE(free, buf);
		buf = (BoxCollider**) malloc(total * sizeof(BoxCollider*));
		int count = 0;
		// Get target chunk
		for (int x0 = b->position.x; x0 <= b->position.x + b->size.x; x0++) {
			for (int y0 = b->position.y; y0 <= b->position.y + b->size.y; y0++) {
				for (int z0 = b->position.z; z0 <= b->position.z + b->size.z; z0++) {
					if (robj->map->map[robj->map->index(x0, y0, z0)].tid != 0) {
						BoxCollider* collider = new BoxCollider(
							glm::vec3(x0, y0, z0),
							glm::vec3(1.0, 0.0, 0.0),
							glm::vec3(0.0, 1.0, 0.0),
							glm::vec3(0.0, 0.0, 1.0)
						);
						buf[count] = collider;
						count ++;
					}
				}
			}
		}

		return buf;
	}

	PhysicsMap::~PhysicsMap() {
		XE(free, buf);
	}

}
