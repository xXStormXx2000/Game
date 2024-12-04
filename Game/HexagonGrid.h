#pragma once
#include "Vector3D.h"
class HexagonGrid {
	Vector3D origin;
	double size;
	const double halfSqrt3 = 0.866025;
public:
	HexagonGrid(Vector3D origin = { 0, 0, 0 }, double size = 1);
	Vector3D getHexCordAtPoint(Vector3D);
	Vector3D hexCordToWorldCord(Vector3D);
};

