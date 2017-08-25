//=============================================================================
// ■ VMGS/Physics/Physics.hpp
//-----------------------------------------------------------------------------
// Take over the Physics world!
//=============================================================================

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/fast_square_root.hpp>

typedef struct StructOBB {
	glm::vec3 position;
	glm::vec3 size;
} OBB;

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

class PhyObject {
public:
	OBB clipping_shell;
	bool is_stable = true;

public:
	virtual void tick();
	virtual void collide_callback(BoxCollider* aC, PhyObject* b, BoxCollider* bC);
	virtual BoxCollider** get_collide_iterator(OBB* b);

	PhyObject();
	virtual ~PhyObject();
};


typedef struct StructPhyObjNode {
	struct StructPhyObjNode *prev, *next;
	PhyObject* obj;
} PhyObjNode;

class PhyEngine {
private:
	PhyObjNode *first, *last;

public:
	static bool AABB(OBB* a, OBB* b);
	void collide(PhyObject* obj, PhyObjNode* node);
	void tick();
	PhyObjNode* add_obj(PhyObject* phy);
	void remove_obj(PhyObjNode* node);

	PhyEngine();
};
