#pragma once
#include <math.h>
#include <string>

#define PI 3.14159265

class Vector3D {
public:
	float x, y, z;

	Vector3D(float x = 0, float y = 0, float z = 0);

	Vector3D operator*(float num) const;
	Vector3D& operator*=(float num);

	Vector3D operator/(float num) const;
	Vector3D& operator/=(float num);

	Vector3D operator+(Vector3D other) const;
	Vector3D& operator+=(const Vector3D& other);

	Vector3D operator-(Vector3D other) const;
	Vector3D& operator-=(const Vector3D& other);

	Vector3D crossProduct(const Vector3D& other) const;
	Vector3D& crossProductEquals(const Vector3D& other);

	Vector3D unitVector() const;
	Vector3D& normalize();

	Vector3D hadamardProduct(const Vector3D& other) const;

	float operator * (const Vector3D& other) const;

	float abs() const;
	float norm();
	float distance(const Vector3D& other) const;

	Vector3D& rotateZ(float);

	bool operator<(const Vector3D& other) const;
	bool operator>(const Vector3D& other) const;
	bool operator==(const Vector3D& other) const;

	std::string toString() const;
};

Vector3D operator*(float num, Vector3D vec);

Vector3D operator/(float num, Vector3D vec);