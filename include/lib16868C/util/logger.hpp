#pragma once
#include <string>
#include <cstdarg>

#define DEBUG
#define ERROR

namespace lib16868C {
template<typename T> T Argument(T value) {
	return value;
}
template<typename T> T const* Argument(std::basic_string<T> const& value) {
	return value.c_str();
}

template<typename ... Args> void printDebug(const char* fmt, Args ... args) {
	#ifdef DEBUG
	printf(fmt, Argument(args) ...);
	#endif
}
template<typename ... Args> void printError(const char* fmt, Args ... args) {
	#ifdef DEBUG
	fprintf(stderr, fmt, Argument(args) ...);
	#endif
}
} // namespace lib16868C