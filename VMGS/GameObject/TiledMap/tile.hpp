//=============================================================================
// ■ tile.hpp
//=============================================================================

#include <global.hpp>

#ifndef _INCLUDE_TILES_H
#define _INCLUDE_TILES_H

namespace VM76 {
	class Tiles : public Object {
	public:
		Vertex* vtx[6];
		GLuint* itx[6];

	public:
		virtual ~Tiles();
		virtual void bake(
			float x, float y, float z, 
			Vertex* v, GLuint* ix, 
			int* vcount, int* icount, int ind);
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
