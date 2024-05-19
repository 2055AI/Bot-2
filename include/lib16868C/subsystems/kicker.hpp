#pragma once
#include "lib16868C/devices/motor.hpp"

namespace lib16868C {
enum class KickerCmd {
	MOVE,
	FIRE,
	HOLD,
	STOP
};

class Kicker {
	public:
	Kicker(lib16868C::Motor& mtr);

	void move(double vel);
	void fireCount(double vel, int count);
	void holdAt(double pos, double vel);
	void stop();

	int getCount() const;

	bool isSettled() const;
	void waitUntilSettled() const;

	double getTemperature() const;

	private:
	lib16868C::Motor& mtr;
	int cnt = 0, fireCnt = 0;
	double tgtVel = 0;

	KickerCmd cmd = KickerCmd::STOP;

	pros::task_t kickerTask;
	static void kickerControl(void*);
};
} // namespace lib16868C