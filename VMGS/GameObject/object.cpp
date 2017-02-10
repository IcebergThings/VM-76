//=============================================================================
// ■ object.cpp
//-----------------------------------------------------------------------------
//
//=============================================================================

#include "GameObject.hpp"

namespace VM76 {
	glm::mat4 GObject::transform() {
		glm::mat4 t;
		t = glm::translate(t, pos);
		t = glm::scale(t, scale);
		t = glm::rotate(t, rotation.x, glm::vec3(1.0, 0.0, 0.0));
		t = glm::rotate(t, rotation.y, glm::vec3(0.0, 1.0, 0.0));
		t = glm::rotate(t, rotation.z, glm::vec3(0.0, 0.0, 1.0));
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
}
