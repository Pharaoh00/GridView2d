#pragma once

#include <QOpenGLFunctions_3_3_Core>

class GLRenderer {
public:
	void init(QOpenGLFunctions_3_3_Core* gl);
	void destroy(QOpenGLFunctions_3_3_Core* gl);

	void drawLines(QOpenGLFunctions_3_3_Core* gl, const float* mvp,
		float r, float g, float b, float a, const float* xy, int vertexCount);

private:
	unsigned prog = 0;
	unsigned vao = 0;
	unsigned vbo = 0;
	int uMVP = -1;
	int uColor = -1;

	bool inited = false;
};