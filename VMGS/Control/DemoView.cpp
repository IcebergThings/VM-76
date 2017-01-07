// Game view of a simple camera turnning, for demo

#include "control.hpp"

#ifdef Control_DemoView
#include <config.hpp>
#include <global.hpp>

namespace VM76 {

// Yes, does nothing in init
	void init_control() {
	}

// That simple and does not need a header
	void update_control() {
		float stime = float(VMDE->frame_count);

		float x = 5.0f * cos(stime * 0.04f);
		float z = 5.0f * sin(stime * 0.04f);
		view = glm::lookAt(glm::vec3(x, 3.5f, z), glm::vec3(0.74f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

}

#endif
