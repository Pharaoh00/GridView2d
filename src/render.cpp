#include "render.h"
#include "grid.h"
#include "overlay.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QFontMetrics>

Render::Render(QWidget* parent) : QOpenGLWidget(parent) {
	setMouseTracking(true);
}

Render::~Render() {
	makeCurrent();
	gl.destroy(this);
	doneCurrent();
}

void Render::initializeGL() {
	initializeOpenGLFunctions();

	qDebug() << "GL_VERSION:" << (const char*)glGetString(GL_VERSION);
	qDebug() << "GL_RENDERER:" << (const char*)glGetString(GL_RENDERER);

	gl.init(this);
	scene.buildTestScene();

	glDisable(GL_DEPTH_TEST);
	glClearColor(0.08f, 0.08f, 0.09f, 1.0f);

	cam.zoom = 80.f;
}

void Render::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
	cam.setViewport(w, h);
}

void Render::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);

	float mvp[16];
	cam.getOrthoMVP(mvp);

	float minX, maxX, minY, maxY;
	cam.getViewBounds(minX, maxX, minY, maxY);

	// GRID
	const int majorEvery = 5;
	auto g = grid::build(minX, maxX, minY, maxY, cam.zoom, majorEvery);
	minor = std::move(g.minor);
	major = std::move(g.major);
	gridStep = g.step;
	gridMajorStep = g.majorStep;

	if (!minor.empty()) {
		gl.drawLines(this, mvp, 0.16f, 0.16f, 0.18f, 1.0f, minor.data(), (int)(minor.size() / 2));
	}

	if (!major.empty()) {
		gl.drawLines(this, mvp, 0.22f, 0.22f, 0.25f, 1.0f, major.data(), (int)(major.size() / 2));
	}

	// SCENE
	if (!scene.verts().empty()) {
		gl.drawLines(this, mvp, 0.60f, 0.60f, 0.65f, 1.0f,
			scene.verts().data(),
			(int)(scene.verts().size() / 2));
	}

	// HIGHLIGHT selecionado
	const int sel = scene.selected();
	if (sel >= 0) {
		const auto& s = scene.segments()[sel];
		const float hi[] = { s.x0, s.y0, s.x1, s.y1 };
		gl.drawLines(this, mvp, 1.0f, 0.85f, 0.25f, 1.0f, hi, 2);
	}

	// EIXOS
	const float axes[] = {
		minX, 0.0f, maxX, 0.0f,
		0.0f, minY, 0.0f, maxY
	};
	gl.drawLines(this, mvp, 0.85f, 0.30f, 0.30f, 1.0f, axes, 4);

	QPainter p(this);
	p.setRenderHint(QPainter::TextAntialiasing, true);

	QFontMetrics fm(p.font());

	Overlay::drawMeasure(p, cam, fm,
		tl_measureStage,
		tl_ax, tl_ay,
		tl_bx, tl_by,
		(float)curPos.x(), (float)curPos.y());

	float cursorWX = 0.0f, cursorWY = 0.0f;
	const bool hasCursor = true; // por enquanto sempre true
	cam.screenToWorld((float)curPos.x(), (float)curPos.y(), cursorWX, cursorWY);

	Overlay::drawHUD(p, fm, cam.zoom, gridStep, gridMajorStep,
		cursorWX, cursorWY, hasCursor);

	const float worldPerPixel = 1.0f / cam.zoom;
	Overlay::drawScaleBar(p, fm, width(), height(), worldPerPixel, 120.0f);
}

void Render::mousePressEvent(QMouseEvent* e) {
	if (e->button() == Qt::RightButton && (e->modifiers() & Qt::ShiftModifier)) {
		tl_measureStage = 0;
		update();
		return;
	}

	if (e->button() == Qt::LeftButton) {
		leftDown = true;
		dragging = false;
		pressPos = e->pos();
		lastPos = e->pos();
		curPos = e->pos();
	}
}

void Render::mouseReleaseEvent(QMouseEvent* e) {
	if (e->button() == Qt::LeftButton) {
		const bool wasClick = !dragging;
		leftDown = false;
		dragging = false;

		if (wasClick) {
			if (!(e->modifiers() & Qt::ShiftModifier)) {
				float wx, wy;
				cam.screenToWorld((float)curPos.x(), (float)curPos.y(), wx, wy);

				const float pickRadiusPx = 10.0f;
				const float thresholdWorld = pickRadiusPx * (1.0f / cam.zoom);

				const int idx = scene.pick(wx, wy, thresholdWorld);
				scene.setSelected(idx);
				update();
				return;
			}
			else {
				float wx, wy;
				cam.screenToWorld((float)curPos.x(), (float)curPos.y(), wx, wy);

				if (tl_measureStage == 0 || tl_measureStage == 2) {
					// novo A
					tl_ax = wx;
					tl_ay = wy;
					tl_measureStage = 1;
				}
				else if (tl_measureStage == 1) {
					// fecha B
					tl_bx = wx;
					tl_by = wy;
					tl_measureStage = 2;
				}

				update();
				return;
			}
		}
	}

}

void Render::mouseMoveEvent(QMouseEvent* e) {
	curPos = e->pos();

	if (leftDown) {
		if (!dragging) {
			if ((curPos - pressPos).manhattanLength() >= QApplication::startDragDistance()) {
				dragging = true;
				lastPos = curPos;
			}
		}

		if (dragging) {
			const QPoint d = curPos - lastPos;
			cam.panByPixels((float)d.x(), (float)d.y());
			lastPos = curPos;
		}
	}

	update();
}

void Render::wheelEvent(QWheelEvent* e) {
	const float delta = (float)e->angleDelta().y();
	const float factor = (delta > 0) ? 1.15f : (1.0f / 1.15f);

	const QPoint p = e->position().toPoint();
	cam.zoomAtScreenPoint(factor, (float)p.x(), (float)p.y());

	update();
}