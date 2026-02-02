#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>

#include "render.h"

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	// OpenGL 3.3 Core
	QSurfaceFormat fmt;
	fmt.setVersion(3, 3);
	fmt.setProfile(QSurfaceFormat::CoreProfile);
	fmt.setDepthBufferSize(24);
	fmt.setStencilBufferSize(8);
	fmt.setSamples(0);
	QSurfaceFormat::setDefaultFormat(fmt);

	QMainWindow win;
	auto* view = new Render();
	win.setCentralWidget(view);
	win.resize(900, 650);
	win.show();

	return app.exec();
}