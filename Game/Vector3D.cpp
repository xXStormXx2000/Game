#include "Vector3D.h"

Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z) {};

Vector3D Vector3D::operator*(float num) const {
	return Vector3D(x * num, y * num, z * num);
}

Vector3D& Vector3D::operator*=(float num) {
	*this = *this * num;
	return *this;
}

Vector3D Vector3D::operator/(float num) const {
	return Vector3D(x / num, y / num, z / num);
}

Vector3D& Vector3D::operator/=(float num) {
	*this = *this / num;
	return *this;
}

float Vector3D::abs() const {
	return (float)sqrt(x * x + y * y + z * z);
}

float Vector3D::norm() {
	return abs();
}

float Vector3D::distance(const Vector3D& other) const {
	return (*this - other).abs();
}

Vector3D& Vector3D::rotateZ(float rad) {
	this->x = this->x * cos(rad) - this->y * sin(rad);
	this->y = this->x * sin(rad) + this->y * cos(rad);
	return *this;
}

bool Vector3D::operator<(const Vector3D& other) const {
	return abs() < other.abs();
}

bool Vector3D::operator>(const Vector3D& other) const {
	return abs() > other.abs();
}

bool Vector3D::operator==(const Vector3D& other) const {
	return x == other.x && y == other.y && z == other.z;
}

Vector3D Vector3D::operator+(Vector3D other) const {
	other.x += x;
	other.y += y;
	other.z += z;
	return other;
}

Vector3D& Vector3D::operator+=(const Vector3D& other) {
	*this = *this + other;
	return *this;
}
Vector3D Vector3D::operator-(Vector3D other) const {
	other.x = x - other.x;
	other.y = y - other.y;
	other.z = z - other.z;
	return other;
}

Vector3D Vector3D::operator-() const {
	return {-x, -y, -z};
}

Vector3D& Vector3D::operator-=(const Vector3D& other) {
	*this = *this - other;
	return *this;
}

Vector3D Vector3D::unitVector() const {
	return (*this / abs());
}

Vector3D& Vector3D::normalize() {
	*this = this->unitVector();
	return *this;
}

Vector3D Vector3D::hadamardProduct(const Vector3D& other) const {
	return { this->x * other.x, this->y * other.y, this->z * other.z };
}

float Vector3D::operator*(const Vector3D& other) const {
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

Vector3D Vector3D::crossProduct(const Vector3D& other) const {
	Vector3D out;
	out.x = y * other.z - z * other.y;
	out.y = z * other.x - x * other.z;
	out.z = x * other.y - y * other.x;
	return out;
}

Vector3D& Vector3D::crossProductEquals(const Vector3D& other) {
	x = y * other.z - z * other.y;
	y = z * other.x - x * other.z;
	z = x * other.y - y * other.x;
	return *this;
}

std::string Vector3D::toString() const {
	return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")";
}

Vector3D operator*(float num, Vector3D vec) {
	vec *= num;
	return vec;
}

Vector3D operator/(float num, Vector3D vec) {
	vec /= num;
	return vec;
}
