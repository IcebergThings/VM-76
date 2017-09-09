//=============================================================================
// ■ object.cpp
//-----------------------------------------------------------------------------
//
//=============================================================================

#include "../VMGS.hpp"

namespace VM76 {
	glm::mat4 GObject::transform() {
		glm::mat4 t;
		t = glm::scale(t, scale);
		t = glm::rotate(t, rotation.x, glm::vec3(1.0, 0.0, 0.0));
		t = glm::rotate(t, rotation.y, glm::vec3(0.0, 1.0, 0.0));
		t = glm::rotate(t, rotation.z, glm::vec3(0.0, 0.0, 1.0));
		t = glm::translate(t, pos);
		return t;
	}

	void GObject::move(glm::vec3 vector) {
		this->pos += vector;
	}

	GObject::GObject(
		glm::vec3 position,
		glm::vec3 rotation,
		glm::vec3 scale
	) {
		this->pos = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	void RenderHierarchy::render(RenderStage currentStage) {
		for (RenderHierarchy* r : children) {
			r->render(currentStage);
		}

		if (currentStage == this->stage && this->robj) {
			this->robj->material->use();
			this->robj->render();
		}
	}

	void RenderHierarchy::sort_child() {
		std::sort(children.begin(), children.end(),
			[] (RenderHierarchy* a, RenderHierarchy* b) {
				return (a->robj->material && b->robj->material) && a->robj->material->program < b->robj->material->program;
			});

		for (RenderHierarchy* r : children) {
			r->sort_child();
		}
	}

	void RenderHierarchy::push_back(RenderHierarchy* robj) {
		robj->father = this;
		this->children.push_back(robj);
	}

	void RenderHierarchy::push_back(RenderStage stage, RenderObject* robj) {
		RenderHierarchy* n = new RenderHierarchy(this, stage, robj);
		this->children.push_back(n);
	}

	RenderHierarchy::RenderHierarchy(RenderHierarchy* father, RenderStage stage, RenderObject* obj) {
		this->father = father;
		this->stage = stage;
		this->robj = obj;
	}
}
