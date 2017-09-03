//=============================================================================
// ■ VMGS/GameObject/Light/Light.hpp
//-----------------------------------------------------------------------------
//   VMGS光源类
//=============================================================================

#pragma once

namespace VM76 {

	class DirectionalLight : public RenderObject {
	public:
		static Shaders* shader;

	public:
		glm::vec3 direction;
		glm::vec3 lightColor;
		glm::vec3 ambientColor;

		bool use_shadow;

		void render();
		DirectionalLight(bool use_shadow = false, glm::vec3 lightColor = glm::vec3(1.0), glm::vec3 ambientColor = glm::vec3(0.1));
	};

}
