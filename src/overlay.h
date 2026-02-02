#pragma once

#include "camera2d.h"

#include <QPainter>
#include <QFontMetrics>

struct Overlay {
	static void drawHUD(QPainter& p, const QFontMetrics& fm,
		float zoomPxPerM, float gridStep, float gridMajorStep,
		float cursorWX, float cursorWY, bool hasCursor);

	static void drawScaleBar(QPainter& p, const QFontMetrics& fm,
		int viewW, int viewH,
		float worldPerPixel, float targetPixels = 120.0f);

	static void drawMeasure(QPainter& p,
		const Camera2D& cam,
		const QFontMetrics& fm,
		int measureStage,
		float ax, float ay,
		float bx, float by,
		float curScreenX, float curScreenY);
};