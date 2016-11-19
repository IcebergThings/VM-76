//=============================================================================
// ■ physics.hpp
//=============================================================================

#include <global.hpp>

#ifndef _INCLUDE_PHYS_H
#define _INCLUDE_PHYS_H

namespace Physics {
	using namespace glm;

	class Object {
	public:
		float mass = 0.0f;
		vec3 center_of_geometry = vec3(0.0);
		float geometry_radius = 0.0f;
		vec3 center_of_gravity = vec3(0.0);
		vec3 velocity = vec3(0.0);

		GLuint vtx_c = 0;
		vec3* vertices;

		void dispose();
		void update();
		void accel();
		Object(GLuint vtx_c, vec3* vertices);
	};

	extern vec3 gravity;
}

#endif
