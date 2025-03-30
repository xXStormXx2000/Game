#pragma once
#ifdef _DEBUG
#include <assert.h>
#include <iostream>
#define debugMessage(x) std::cout << x << '\n'
#else
#define debugMessage(x)
#define assert(x)
#endif