//=============================================================================
// ■ control.hpp
//=============================================================================

#ifndef INCLUDE_CONTROL_
#define INCLUDE_CONTROL_

#include <config.hpp>
#include <global.hpp>

namespace VM76 {

	class Control : public Object {
	public:
		void update_control();
		void init_control();
	};

}

#endif
