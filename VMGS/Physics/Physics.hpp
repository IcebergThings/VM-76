//=============================================================================
// ■ VMGS/Physics/Physics.hpp
//-----------------------------------------------------------------------------
// Take over the Physics world!
//=============================================================================

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/fast_square_root.hpp>

#ifndef _INCLUDE_PHYSICS_H
#define _INCLUDE_PHYSICS_H

typedef struct StructOBB {
	glm::vec3 position;
	glm::vec3 size;
} OBB;

class PhyEngine {
public:
	static bool AABB(OBB* a, OBB* b);

	PhyEngine();
};

class BoxCollider {
public:
	glm::vec3 X, Y, Z;
	glm::vec3 anchor;
	glm::mat4 w2local;

public:
	bool is_collide(BoxCollider* b);
	void move(glm::vec3 o);

	BoxCollider(glm::vec3 o, glm::vec3 a, glm::vec3 b, glm::vec3 c);
};

#endif
