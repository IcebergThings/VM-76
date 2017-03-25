//=============================================================================
// ■ DemoView.cpp
//-----------------------------------------------------------------------------
//   Game view of a simple camera turnning, for demo
//=============================================================================

#include "control.hpp"

#ifndef INCLUDE_CONTROL_DEMO_VIEW_
#define INCLUDE_CONTROL_DEMO_VIEW_

namespace VM76 {

class DemoView : public Control {
public:
	void update_control();
	void init_control();

};

}

#endif
