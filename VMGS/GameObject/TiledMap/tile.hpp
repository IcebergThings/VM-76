//=============================================================================
// ■ tile.hpp
//=============================================================================

#include <global.hpp>

#ifndef _INCLUDE_TILES_H
#define _INCLUDE_TILES_H

namespace VM76 {
	class Tiles : public RenderObject {
	public:
		GDrawable* obj[6];
		glm::mat4* mat[6];

		Vertex* vtx[6];
		GLuint* itx[6];

	public:
		virtual ~Tiles();
		virtual void render();
		virtual void update_instance(int c1, int c2, int c3, int c4, int c5, int c6);
		virtual void dispose();
	};

	enum TileID {
		Air,           // 0
		Grass,         // 1
		Stone,         // 2
		Dirt,          // 3
		Glass,         // 4
		WoodPlank,     // 5
		HalfBrick,     // 6 : 0 = whole, 1 = bottom, 2 = top
		Brick,         // 7
		TNT,           // 8
		CobbleStone,   // 9
	};

	class SimpleCubeTile : public Tiles {
	public:
		SimpleCubeTile(int tid);
	};

	class MultiFaceCubeTile : public Tiles {
	public:
		MultiFaceCubeTile(int t1, int t2, int t3, int t4, int t5, int t6);
	};
}

#endif
