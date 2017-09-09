//=============================================================================
// ■ DirectionalLight.cpp
//-----------------------------------------------------------------------------
//   定向平行光源
//=============================================================================

#include "../../VMGS.hpp"

namespace VM76 {

	void DirectionalLight::render() {
		material->set_texture("normal", Res::TextureList["GBuffers/Normals"], 1);

		material->set_vec3("sunVec", direction);
		material->set_vec3("lightColor", lightColor);
		material->set_vec3("ambientColor", ambientColor);

		PostProcessingManager::Blit2D();
	}

	DirectionalLight::DirectionalLight(bool use_shadow, glm::vec3 lightColor, glm::vec3 ambientColor) {
		this->use_shadow = use_shadow;
		this->lightColor = lightColor;
		this->ambientColor = ambientColor;

		this->material = Res::ShadersList["Internal/DirectionalLight"];
	};

}
