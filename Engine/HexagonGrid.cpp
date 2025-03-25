#include "HexagonGrid.h"

HexagonGrid::HexagonGrid(Vector3D origin, float size): origin(origin), size(size) {}
Vector3D HexagonGrid::getHexCordAtPoint(Vector3D point) {
	point -= this->origin;
	const Vector3D i(1, 0, 0);
	const Vector3D j(0.5, halfSqrt3, 0);
	const Vector3D k(-0.5, halfSqrt3, 0);

	float x_ = floor(i * (point/this->size));
	float y_ = floor(j * (point/this->size));
	float z_ = floor(k * (point/this->size));

	float x = x_ + (int)(floor((y_ + z_ + 2)/3) + x_)%2;
	float z = z_ + (int)(floor((y_ + x_ + 2)/3) + z_)%2;

	return Vector3D(x, z, 0);
}
Vector3D HexagonGrid::hexCordToWorldCord(Vector3D hexCord) {
	Vector3D worldCord;
	worldCord.x = hexCord.x * this->size;
	worldCord.y = halfSqrt3 * (hexCord.y + (hexCord.y / 2 + hexCord.x) * 2 / 3) *this->size;
	worldCord += this->origin;
	return worldCord;
};
