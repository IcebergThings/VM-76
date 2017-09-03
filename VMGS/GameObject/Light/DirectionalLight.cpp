//=============================================================================
// ■ DirectionalLight.cpp
//-----------------------------------------------------------------------------
//   定向平行光源
//=============================================================================

#include "../../VMGS.hpp"

namespace VM76 {

	Shaders* DirectionalLight::shader = NULL;

	void DirectionalLight::render() {
		shader->use();
		shader->set_vec3("sunVec", direction);
		shader->set_vec3("lightColor", lightColor);
		shader->set_vec3("ambientColor", ambientColor);

		PostProcessingManager::Blit2D();
	}

	DirectionalLight::DirectionalLight(bool use_shadow, glm::vec3 lightColor, glm::vec3 ambientColor) {
		this->use_shadow = use_shadow;
		this->lightColor = lightColor;
		this->ambientColor = ambientColor;
	};

}
