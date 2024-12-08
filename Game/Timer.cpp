#include "Timer.h"

void Timer::start() {
	startTime = std::chrono::high_resolution_clock::now();
}

float Timer::stop() {
	auto endTime = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTime).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

	auto duration = end - start;
	float msTime = duration;
	return msTime;
}
