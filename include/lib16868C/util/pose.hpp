#pragma once
#include "okapi/api.hpp"
#include "lib16868C/util/math.hpp"
#include <string>

namespace lib16868C {
class Pose {
	public:
		Pose();
		Pose(okapi::QLength x, okapi::QLength y);
		Pose(okapi::QLength x, okapi::QLength y, okapi::QAngle theta, uint time);
		Pose(const Pose& p);

		okapi::QLength distTo(Point p);
		okapi::QLength distTo(Pose p);
		okapi::QAngle angleTo(Point p);
		okapi::QAngle angleTo(Pose p);

		const Point* pos();
		const double* x() const;
		const double* y() const;
		const double* theta() const;
		const uint* time() const;

		void setPos(Point pos);
		void setX(double x);
		void setY(double y);
		void setTheta(double theta);
		void setTime(double time);

		std::string toStr();

		Pose operator+(const Pose& p) const;
		Pose operator-(const Pose& p) const;
		double operator*(const Pose& p) const;
		Pose operator*(const double& o) const;
		Pose operator/(const double& o) const;
		Pose& operator=(const Pose& p);

	private:
		Point _pos;
		double _theta;
		uint _time;
		
		Pose(Point pos, double theta, uint time);
		Pose(double x, double y, double theta, uint time);
};
} // namespace lib16868C