#include "lib16868C/devices/pneumatic.hpp"

using namespace lib16868C;

Pneumatic::Pneumatic(uint port, bool initState) : pneumatic(port, initState) {
	state = initState;
}
Pneumatic::Pneumatic(uint smartPort, uint port, bool initState) : pneumatic({smartPort, port}, initState) {
	state = initState;
}

void Pneumatic::extend() {
	setState(true);
}
void Pneumatic::retract() {
	setState(false);
}
void Pneumatic::toggle() {
	setState(!state);
}

bool Pneumatic::getState() {
	return state;
}

void Pneumatic::setState(bool state) {
	pneumatic.set_value(state);
	this->state = state;
}