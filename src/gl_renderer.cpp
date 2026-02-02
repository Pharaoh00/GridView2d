#include "gl_renderer.h"
#include <cstdio>

static unsigned compileShader(QOpenGLFunctions_3_3_Core* gl, unsigned type, const char* src) {
	unsigned s = gl->glCreateShader(type);
	gl->glShaderSource(s, 1, &src, nullptr);
	gl->glCompileShader(s);

	int ok = 0;
	gl->glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		char log[4096];
		gl->glGetShaderInfoLog(s, (int)sizeof(log), nullptr, log);
		std::fprintf(stderr, "Shader compile error:\n%s\n", log);
	}
	return s;
}

static unsigned linkProgram(QOpenGLFunctions_3_3_Core* gl, unsigned vs, unsigned fs) {
	unsigned p = gl->glCreateProgram();
	gl->glAttachShader(p, vs);
	gl->glAttachShader(p, fs);
	gl->glLinkProgram(p);

	int ok = 0;
	gl->glGetProgramiv(p, GL_LINK_STATUS, &ok);
	if (!ok) {
		char log[4096];
		gl->glGetProgramInfoLog(p, (int)sizeof(log), nullptr, log);
		std::fprintf(stderr, "Program link error:\n%s\n", log);
	}

	gl->glDetachShader(p, vs);
	gl->glDetachShader(p, fs);
	return p;
}

void GLRenderer::init(QOpenGLFunctions_3_3_Core* gl) {
	if (inited) return;

	const char* vsSrc = R"(
        #version 330 core
        layout(location=0) in vec2 aPos;
        uniform mat4 uMVP;
        void main() { gl_Position = uMVP * vec4(aPos, 0.0, 1.0); }
    )";

	const char* fsSrc = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec4 uColor;
        void main() { FragColor = uColor; }
    )";

	unsigned vs = compileShader(gl, GL_VERTEX_SHADER, vsSrc);
	unsigned fs = compileShader(gl, GL_FRAGMENT_SHADER, fsSrc);
	prog = linkProgram(gl, vs, fs);
	gl->glDeleteShader(vs);
	gl->glDeleteShader(fs);

	uMVP = gl->glGetUniformLocation(prog, "uMVP");
	uColor = gl->glGetUniformLocation(prog, "uColor");

	gl->glGenVertexArrays(1, &vao);
	gl->glBindVertexArray(vao);

	gl->glGenBuffers(1, &vbo);
	gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);

	gl->glEnableVertexAttribArray(0);
	gl->glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * (int)sizeof(float), (void*)0);

	gl->glBindVertexArray(0);

	inited = true;
}

void GLRenderer::destroy(QOpenGLFunctions_3_3_Core* gl) {
	if (!inited) return;

	if (vbo) {
		gl->glDeleteBuffers(1, &vbo);
		vbo = 0;
	}
	if (vao) {
		gl->glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
	if (prog) {
		gl->glDeleteProgram(prog);
		prog = 0;
	}

	uMVP = -1;
	uColor = -1;
	inited = false;
}

void GLRenderer::drawLines(QOpenGLFunctions_3_3_Core* gl,
	const float* mvp,
	float r, float g, float b, float a,
	const float* xy, int vertexCount) {
	if (!inited || vertexCount <= 0) return;

	gl->glUseProgram(prog);
	gl->glUniformMatrix4fv(uMVP, 1, false, mvp);
	gl->glUniform4f(uColor, r, g, b, a);

	gl->glBindVertexArray(vao);

	const int bytes = vertexCount * 2 * (int)sizeof(float);
	gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);

	gl->glBufferData(GL_ARRAY_BUFFER, bytes, nullptr, GL_DYNAMIC_DRAW);
	gl->glBufferSubData(GL_ARRAY_BUFFER, 0, bytes, xy);

	gl->glDrawArrays(GL_LINES, 0, vertexCount);

	gl->glBindVertexArray(0);
	gl->glUseProgram(0);
}