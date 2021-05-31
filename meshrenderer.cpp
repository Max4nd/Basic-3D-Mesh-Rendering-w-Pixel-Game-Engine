#include "meshrenderer.h"

vecf3D mat4x4::multiplyMatrixVector(vecf3D i) {
	vecf3D o;
	o.x = i.x * this->m[0][0] + i.y * this->m[1][0] + i.z * this->m[2][0] + this->m[3][0];
	o.y = i.x * this->m[0][1] + i.y * this->m[1][1] + i.z * this->m[2][1] + this->m[3][1];
	o.z = i.x * this->m[0][2] + i.y * this->m[1][2] + i.z * this->m[2][2] + this->m[3][2];
	float w = i.x * this->m[0][3] + i.y * this->m[1][3] + i.z * this->m[2][3] + this->m[3][3];

	//Revert from 4d to 3d so long as w isn't 0
	if (w != 0.0f) {
		o.x /= w; o.y /= w; o.z /= w;
	}
	return o;
}

triangle triangle::multiplyTriangle(mat4x4 m) {
	triangle o;
	o.a = m.multiplyMatrixVector(this->a);
	o.b = m.multiplyMatrixVector(this->b);
	o.c = m.multiplyMatrixVector(this->c);
	return o;
}

triangle triangle::zOffset(float offset) {
	triangle o;
	this->a.z = o.a.z + offset;
	this->b.z = o.b.z + offset;
	this->c.z = o.c.z + offset;
	return o;
}