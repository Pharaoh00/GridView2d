#include "overlay.h"
#include <QString>
#include <cmath>

static float nice_1_2_5_10(float x) {
	if (x <= 0.0f) return 1.0f;

	const float e10 = std::pow(10.0f, std::floor(std::log10(x)));
	const float f = x / e10; // 1..10

	float nf = 1.0f;
	if (f < 1.5f) {
		nf = 1.0f;
	}
	else if (f < 3.0f) {
		nf = 2.0f;
	}
	else if (f < 7.0f) {
		nf = 5.0f;
	}
	else {
		nf = 10.0f;
	}

	return nf * e10;
}

void Overlay::drawHUD(QPainter& p, const QFontMetrics& fm,
	float zoomPxPerM, float gridStep, float gridMajorStep,
	float cursorWX, float cursorWY, bool hasCursor) {
	const int x = 12;
	int y = 22;
	const int line = fm.lineSpacing();

	p.setPen(QColor(230, 230, 230));

	const float worldPerPixel = 1.0f / zoomPxPerM;
	const float worldPer100px = worldPerPixel * 100.0f;

	QString line1 = QString("zoom: %1 px/m").arg(zoomPxPerM, 0, 'f', 2);
	QString line2 = QString("grid: minor %1 m | major %2 m")
		.arg(gridStep, 0, 'f', 3)
		.arg(gridMajorStep, 0, 'f', 3);
	QString line3 = QString("100 px = %1 m").arg(worldPer100px, 0, 'f', 3);

	p.drawText(x, y, line1); y += line;
	p.drawText(x, y, line2); y += line;
	p.drawText(x, y, line3); y += line;

	if (hasCursor) {
		QString line4 = QString("cursor: X %1 m | Y %2 m")
			.arg(cursorWX, 0, 'f', 3)
			.arg(cursorWY, 0, 'f', 3);
		p.drawText(x, y, line4); y += line;
	}
}

void Overlay::drawScaleBar(QPainter& p, const QFontMetrics& fm,
	int viewW, int viewH,
	float worldPerPixel, float targetPixels) {
	const float targetWorld = targetPixels * worldPerPixel;
	const float barWorld = nice_1_2_5_10(targetWorld);
	const int barPx = (int)std::round(barWorld / worldPerPixel);

	const int margin = 14;
	const int baseX = margin;
	const int baseY = viewH - margin;

	const int labelH = fm.height();
	const int pad = 6;
	const int tick = 6;

	const int barY = baseY - pad;
	const int labelTop = barY - tick - pad - labelH;

	// fundo
	QRect bg(baseX - pad, labelTop - pad, barPx + pad * 2, labelH + tick + pad * 3);
	p.setPen(Qt::NoPen);
	p.setBrush(QColor(0, 0, 0, 120));
	p.drawRoundedRect(bg, 4, 4);

	// label
	p.setPen(QColor(230, 230, 230));
	QString label = QString("%1 m").arg(barWorld, 0, 'g', 3);
	p.drawText(QRect(baseX, labelTop, barPx, labelH), Qt::AlignCenter, label);

	// barra + ticks
	p.setPen(QPen(QColor(230, 230, 230), 2));
	p.drawLine(baseX, barY, baseX + barPx, barY);
	p.drawLine(baseX, barY - tick, baseX, barY + tick);
	p.drawLine(baseX + barPx, barY - tick, baseX + barPx, barY + tick);
}

void Overlay::drawMeasure(QPainter& p,
	const Camera2D& cam,
	const QFontMetrics& fm,
	int measureStage,
	float ax, float ay,
	float bx, float by,
	float curScreenX, float curScreenY)
{
	if (measureStage <= 0) return;

	QString mline;
	if (measureStage == 0) {
		mline = "measure: Shift+Click set A | Shift+Right clear";
	}
	else if (measureStage == 1) {
		mline = "measure: Shift+Click set B | Shift+Right cancel";
	}
	else {
		mline = "measure: Shift+Click new | Shift+Right clear";
	}

	p.drawText(12, 22 + fm.lineSpacing() * 4, mline);

	// "ghost" line
	float bwx = bx, bwy = by;
	if (measureStage == 1) {
		cam.screenToWorld(curScreenX, curScreenY, bwx, bwy);
	}

	float asx, asy, bsx, bsy;
	cam.worldToScreen(ax, ay, asx, asy);
	cam.worldToScreen(bwx, bwy, bsx, bsy);

	const float sdx = bsx - asx;
	const float sdy = bsy - asy;
	const float sdist2 = sdx * sdx + sdy * sdy;

	p.setRenderHint(QPainter::Antialiasing, true);

	const QColor c(255, 255, 80, 220);

	// marker A
	p.setPen(Qt::NoPen);
	p.setBrush(c);
	p.drawEllipse(QPointF(asx, asy), 4.0, 4.0);

	// se muito perto, não desenha linha nem label
	if (sdist2 < (6.0f * 6.0f)) return;

	// linha
	QPen pen(c, 2);
	if (measureStage == 1) pen.setStyle(Qt::DashLine); // ghost
	p.setPen(pen);
	p.setBrush(Qt::NoBrush);
	p.drawLine(QPointF(asx, asy), QPointF(bsx, bsy));

	// marker B
	p.setPen(Qt::NoPen);
	p.setBrush(c);
	p.drawEllipse(QPointF(bsx, bsy), 4.0, 4.0);

	// texto dx/dy/dist no mundo
	const float dx = bwx - ax;
	const float dy = bwy - ay;
	const float dist = std::sqrt(dx * dx + dy * dy);

	QString msg = QString("dx %1 m | dy %2 m | d %3 m")
		.arg(dx, 0, 'f', 3)
		.arg(dy, 0, 'f', 3)
		.arg(dist, 0, 'f', 3);

	const int pad = 6;
	const int w = fm.horizontalAdvance(msg) + pad * 2;
	const int h = fm.height() + pad * 2;

	int x = (int)bsx + 12;
	int y = (int)bsy + 12;

	// Cap da tela
	x = std::max(8, std::min(x, cam.viewPortpH - w - 8));
	y = std::max(8, std::min(y, cam.viewPortpH - h - 8));

	QRect r(x, y, w, h);

	p.setPen(Qt::NoPen);
	p.setBrush(QColor(0, 0, 0, 160));
	p.drawRoundedRect(r, 4, 4);

	p.setPen(QColor(255, 255, 80, 240));
	p.drawText(r.adjusted(pad, pad, -pad, -pad), Qt::AlignLeft | Qt::AlignVCenter, msg);
}