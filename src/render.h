#pragma once

#include "gl_renderer.h"
#include "camera2d.h"
#include "scene.h"

#include <vector>

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QPoint>

class Render final : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
	Q_OBJECT
public:
	explicit Render(QWidget* parent = nullptr);
	~Render() override;

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void mousePressEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void wheelEvent(QWheelEvent* e) override;

private:
	GLRenderer gl;
	Camera2D cam;
	Scene scene;

	bool leftDown = false;
	bool dragging = false;
	QPoint pressPos;
	QPoint lastPos;
	QPoint curPos;

	std::vector<float> minor;
	std::vector<float> major;
	float gridStep = 1.0f;
	float gridMajorStep = 5.0f;

	int   tl_measureStage = 0;
	float tl_ax = 0.0f, tl_ay = 0.0f;
	float tl_bx = 0.0f, tl_by = 0.0f;
};