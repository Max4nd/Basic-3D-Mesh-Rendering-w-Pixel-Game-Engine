#pragma once
#include <iostream>
#include <math.h>
#include <fstream>
#include <strstream>
#include <vector>

struct vecf3D {
	float x, y, z;

	vecf3D() : x(0), y(0), z(0) {}
	vecf3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	vecf3D operator + (const vecf3D& rhs) const { return vecf3D(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z); }
};

struct mat4x4 {
	float m[4][4] = { 0 };

	vecf3D multiplyMatrixVector(vecf3D i);
};

struct triangle {
	vecf3D a, b, c;

	triangle() : a(), b(), c() {}
	triangle(vecf3D _a, vecf3D _b, vecf3D _c) : a(_a), b(_b), c(_c) {}
	triangle operator + (const triangle& rhs) const { return triangle(this->a + rhs.a, this->b + rhs.b, this->c + rhs.c); }

	triangle multiplyTriangle(mat4x4 m);
	triangle zOffset(float offset);
};

struct mesh {
	std::vector<triangle> tris;
};
