#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "meshrenderer.h"

class Example : public olc::PixelGameEngine {
public:
	Example() {
		sAppName = "Basic 3D Mesh Rendering";
	}

private:
	mesh object;
	mat4x4 matProj;

	float fTheta;

	bool LoadFromObjectFile(std::string sFilename, mesh& toMesh) {
		std::ifstream f(sFilename);
		if (!f.is_open())
			return false;

		//temp cache
		std::vector<vecf3D> verts;

		while (!f.eof()) {
			char line[128];
			f.getline(line, 128);

			std::strstream s;
			s << line;

			char junk;

			if (line[0] == 'v') {
				vecf3D v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f') {
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				toMesh.tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}
		return true;
	}

public:
	bool OnUserCreate() override {

		LoadFromObjectFile("teapot.obj", object);

		//Projection Matrix taken from OLC source
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {

		Clear(olc::BLACK);

		//Rotation
		mat4x4 matRotZ, matRotX;
		fTheta += 1.0f * fElapsedTime;

		//Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		//Rotation X (Must have different rotation then the Z axis to avoid gimble lock)
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;

		//Draw Triangles
		for (auto tri : object.tris) {
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			//Rotate in Z-Axis
			triRotatedZ = tri.multiplyTriangle(matRotZ);

			//Rotate in X-Axis
			triRotatedZX = triRotatedZ.multiplyTriangle(matRotX);

			//Offset into the screen
			triTranslated = triRotatedZX;
			triRotatedZX = triTranslated.zOffset(6.0f);

			//Project triangles from 3D to 2D
			triProjected = triTranslated.multiplyTriangle(matProj);

			//Scale into view, need to refactor this
			triProjected.a.x += 1.0f; triProjected.a.y += 1.0f;
			triProjected.b.x += 1.0f; triProjected.b.y += 1.0f;
			triProjected.c.x += 1.0f; triProjected.c.y += 1.0f;
			triProjected.a.x *= 0.5f * (float)ScreenWidth();
			triProjected.a.y *= 0.5f * (float)ScreenHeight();
			triProjected.b.x *= 0.5f * (float)ScreenWidth();
			triProjected.b.y *= 0.5f * (float)ScreenHeight();
			triProjected.c.x *= 0.5f * (float)ScreenWidth();
			triProjected.c.y *= 0.5f * (float)ScreenHeight();

			//Draw all lines in triangle
			//DrawTriangle(triProjected.a.x, triProjected.a.y,
			//	triProjected.b.x, triProjected.b.y,
			//	triProjected.c.x, triProjected.c.y, olc::WHITE);

			//Draw side a and b no c (looks more like mesh)
			DrawLine(triProjected.a.x, triProjected.a.y, triProjected.b.x, triProjected.b.y, olc::WHITE);
			DrawLine(triProjected.b.x, triProjected.b.y, triProjected.c.x, triProjected.c.y, olc::WHITE);
		}
		return true;
	}
};

int main() {
	Example demo;
	if (demo.Construct(1280, 720, 1, 1))
		demo.Start();

	return 0;
}