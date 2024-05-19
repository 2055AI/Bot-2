#pragma once
#include "api.h"
#include <functional>
#include <queue>
#include <vector>

namespace lib16868C {
namespace Util {
template<typename T> inline int sgn(T n) {
	return (n > T(0)) - (n < T(0));
}

double degToRad(double deg);
double radToDeg(double rad);

double mToIn(double m);
double inToM(double in);

template<typename T> inline T avg(std::vector<T> v) {
	T sum = T(0);
	for (size_t i = 0; i < v.size(); i++) sum += v[i];
	return sum / v.size();
}
template<typename T> inline T avg(std::initializer_list<T> l) {
	return avg(std::vector<T>(l));
}

template<typename T> inline std::vector<T> queueToVector(const std::queue<T>& q) {
	std::vector<T> v;
	std::queue<T> tmp = q;
	while (!tmp.empty()) {
		v.push_back(tmp.front());
		tmp.pop();
	}
	return v;
}
template<typename T> inline std::queue<T> vectorToQueue(const std::vector<T>& v) {
	std::queue<T> q;
	for (size_t i = 0; i < v.size(); i++) {
		q.push(v[i]);
	}
	return q;
}

void runAsBlocking(std::function<void()> fn, std::function<bool()> endCond, int timeout = -1, int pollRate = 10, int paddingDelay = 5);
void blocking(std::function<bool()> endCond, int timeout = -1, int pollRate = 10, int paddingDelay = 5);
pros::Task runAsync(std::function<void()> fn);
} // namespace lib16868C::Util
} // namespace lib16868C