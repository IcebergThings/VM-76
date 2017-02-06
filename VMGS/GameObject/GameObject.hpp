//=============================================================================
// ■ GameObject.hpp
//=============================================================================

#ifndef _INCLUDE_GAME_OBJECT_H
#define _INCLUDE_GAME_OBJECT_H

#include "Cube/cube.hpp"
#include "GLAxis/axis.hpp"

namespace VM76 {
	class Object {
	public:
		glm::vec3 pos;
		glm::vec3 rotation;
		glm::vec3 scale;

	public:
		glm::mat4 transform();
		void move(glm::vec3 vector);
		void move_to(glm::vec3 vector);

		Object(
			glm::vec3 position = glm::vec3(0.0),
			glm::vec3 rotation = glm::vec3(0.0),
			glm::vec3 scale = glm::vec3(1.0)
		);
	};
}

#endif
