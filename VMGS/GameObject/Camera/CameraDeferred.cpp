//=============================================================================
// ■ VMGS/Camera/CameraDeferred.cpp
//-----------------------------------------------------------------------------
// VMGS摄像机-Deferred
//=============================================================================

#include "../../VMGS.hpp"

namespace VM76 {

	RenderBuffer* CameraDeferred::create_gbuffers() {
		const GLuint gbuffers_type[4] = {GL_RGB8, GL_RGB8, GL_RGB8, GL_RGB16F};
		// Albedo, Normal, Lighting, Composite
		return new RenderBuffer(VMDE->width, VMDE->height, 4, gbuffers_type);
	}

	void CameraDeferred::render(RenderHierarchy* scene_root) {
		// ================ STAGE 1 ================
		//  GBuffers_Solid & GBuffers_Cutout
		//  Bind Post buffer & use stencil
		gbuffers->bind();
		RenderBuffer::clearBuffer(glm::vec4(0.0, 0.0, 0.0, 0.0), true, true, true);
		gbuffers->set_draw_buffers();

		// setup mask
		glStencilFunc(GL_ALWAYS, 1, 0x01);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);

		// Textured blocks rendering
		//shader_textured.ProjectionView(cam->Projection, cam->View);
		scene_root->render(GBuffers_Cutout);

		// ================ STAGE 2 ================
		//  Deferred_Lighting_Opaque
		//  Read Post buffer & stencil mask
		glStencilFunc(GL_EQUAL, 1, 0x01);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilMask(0x00);

		// Blend mode : ADD for light accumulation
		glBlendFunc(GL_ONE, GL_ONE);

		scene_root->render(Deferred_Lighting_Opaque);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Combine lighting with albedo
		Shaders* deferred_composite = Res::ShadersList["Internal/DeferredComposite"];
		deferred_composite->use();
		deferred_composite->set_texture("albedo", gbuffers->texture_buffer[BufferAlbedo], 0);
		deferred_composite->set_texture("lighting", gbuffers->texture_buffer[BufferLighting], 1);
		deferred_composite->set_texture("depthtex", gbuffers->texture_buffer[4], 14);
		deferred_composite->set_vec2("clipping", glm::vec2(0.1, 1000.0));
		deferred_composite->set_vec3("fogColor", glm::vec3(0.5, 0.7, 1.0));
		PostProcessingManager::Blit2D();

		// ================ STAGE 3 ================
		//  Skybox (Unfilled area) shading
		//  Reverse stencil mask

		glStencilFunc(GL_NOTEQUAL, 1, 0x01);
		VMStateControl::disable_depth_test();
		scene_root->render(Render_Skybox);

		// ================ STAGE 4 ================
		//  GBuffers_NoLighting_Opaque
		//  No stencil
		VMStateControl::disable_stencil_test();

		// Non textured rendering
		scene_root->render(GBuffers_NoLighting_Opaque);

		// ================ STAGE 5 ================
		//  Final composite & Full screen shading
		gbuffers->unbind();
		Shaders* final_composite = Res::ShadersList["Internal/FinalComposite"];
		final_composite->use();
		final_composite->set_texture("composite", gbuffers->texture_buffer[BufferComposite], 15);
		PostProcessingManager::Blit2D();
	}

	CameraDeferred::CameraDeferred(glm::vec3 wpos, glm::vec3 rotate, glm::mat4 proj) {
		this->pos = wpos;
		this->rotation = rotate;
		this->Projection = proj;

		update();
	}
}
