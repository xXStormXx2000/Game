#pragma once
#include <chrono>

class Timer {
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
public:
	void start();
	double stop();
};