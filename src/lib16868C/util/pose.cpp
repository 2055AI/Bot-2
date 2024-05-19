#include "lib16868C/util/pose.hpp"
#include <ctime>

using namespace lib16868C;

lib16868C::Pose::Pose() : Pose(Point(), 0, 0) {}
lib16868C::Pose::Pose(Point pos, double theta, uint time) : _pos(pos), _theta(theta), _time(time) {}
lib16868C::Pose::Pose(double x, double y, double theta, uint time) : Pose(Point(x, y), theta, time) {}
lib16868C::Pose::Pose(okapi::QLength x, okapi::QLength y) : Pose(x.convert(okapi::inch), y.convert(okapi::inch), 0, 0) {}
lib16868C::Pose::Pose(okapi::QLength x, okapi::QLength y, okapi::QAngle theta, uint time)
	: Pose(x.convert(okapi::inch), y.convert(okapi::inch), theta.convert(okapi::radian), time) {}
lib16868C::Pose::Pose(const Pose& p) : Pose(p._pos, p._theta, p._time) {}

okapi::QLength lib16868C::Pose::distTo(Point p) {
	return _pos.distTo(p) * okapi::inch;
}
okapi::QLength lib16868C::Pose::distTo(Pose p) {
	return distTo(p._pos);
}
okapi::QAngle lib16868C::Pose::angleTo(Point p) {
	return _pos.angleTo(p) * okapi::radian;
}
okapi::QAngle lib16868C::Pose::angleTo(Pose p) {
	return angleTo(p._pos);
}

const Point* Pose::pos() {
	return &_pos;
}
const double* Pose::x() const {
	return &_pos.x;
}
const double* Pose::y() const {
	return &_pos.y;
}
const double* Pose::theta() const {
	return &_theta;
}
const uint* Pose::time() const {
	return &_time;
}

void Pose::setPos(Point pos) {
	this->_pos = pos;
}
void Pose::setX(double x) {
	this->_pos.x = x;
}
void Pose::setY(double y) {
	this->_pos.y = y;
}
void Pose::setTheta(double theta) {
	this->_theta = theta;
}
void Pose::setTime(double time) {
	this->_time = time;
}

std::string lib16868C::Pose::toStr() {
	return "{" + _pos.toStr() + ", theta: " + std::to_string(_theta) + ", time: " + std::to_string(_time) + "}";
}

Pose lib16868C::Pose::operator+(const Pose& p) const {
	return {*x() + *p.x(), *y() + *p.y(), *theta(), *time()};
}
Pose lib16868C::Pose::operator-(const Pose& p) const {
	return {*x() - *p.x(), *y() - *p.y(), *theta(), *time()};
}
double lib16868C::Pose::operator*(const Pose& p) const {
	return *x() * *p.x() + *y() * *p.y();
}
Pose lib16868C::Pose::operator*(const double& o) const {
	return {*x() * o, *y() * o, *theta(), *time()};
}
Pose lib16868C::Pose::operator/(const double& o) const {
	return {*x() / o, *y() / o, *theta(), *time()};
}
Pose& lib16868C::Pose::operator=(const Pose& p) {
	_pos = p._pos;
	_theta = p._theta;
	_time = p._time;
	return *this;
}