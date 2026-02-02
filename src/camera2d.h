#pragma once

#include <algorithm>

struct Camera2D {
	float panX = 0.0f;
	float panY = 0.0f;

	float zoom = 80.0f;

	float minZoom = 0.05f;
	float maxZoom = 300.0f;

	int viewPortW = 1;
	int viewPortpH = 1;

	void setViewport(int w, int h) {
		viewPortW = (w > 0) ? w : 1;
		viewPortpH = (h > 0) ? h : 1;
	}

	void getViewBounds(float& minX, float& maxX, float& minY, float& maxY) const {
		const float halfW = (viewPortW * 0.5f) / zoom;
		const float halfH = (viewPortpH * 0.5f) / zoom;
		minX = panX - halfW;
		maxX = panX + halfW;
		minY = panY - halfH;
		maxY = panY + halfH;
	}

	// column-major
	void getOrthoMVP(float out16[16]) const {
		float l, r, b, t;
		getViewBounds(l, r, b, t);
		const float n = -1.0f;
		const float f = 1.0f;

		out16[0] = 2.0f / (r - l); out16[4] = 0.0f;       out16[8] = 0.0f;        out16[12] = -(r + l) / (r - l);
		out16[1] = 0.0f;       out16[5] = 2.0f / (t - b); out16[9] = 0.0f;        out16[13] = -(t + b) / (t - b);
		out16[2] = 0.0f;       out16[6] = 0.0f;       out16[10] = -2.0f / (f - n);  out16[14] = -(f + n) / (f - n);
		out16[3] = 0.0f;       out16[7] = 0.0f;       out16[11] = 0.0f;        out16[15] = 1.0f;
	}

	void screenToWorld(float sx, float sy, float& wx, float& wy) const {
		const float halfW = (viewPortW * 0.5f) / zoom;
		const float halfH = (viewPortpH * 0.5f) / zoom;

		const float minX = panX - halfW;
		const float maxY = panY + halfH;

		wx = minX + (sx / zoom);
		wy = maxY - (sy / zoom);
	}

	void worldToScreen(float wx, float wy, float& sx, float& sy) const {
		const float halfW = (viewPortW * 0.5f) / zoom;
		const float halfH = (viewPortpH * 0.5f) / zoom;

		const float minX = panX - halfW;
		const float maxY = panY + halfH;

		sx = (wx - minX) * zoom;
		sy = (maxY - wy) * zoom;
	}

	void panByPixels(float dxPixels, float dyPixels) {
		panX -= dxPixels / zoom;
		panY += dyPixels / zoom;
	}

	void zoomAtScreenPoint(float factor, float sx, float sy) {
		const float oldZoom = zoom;
		float newZoom = zoom * factor;
		newZoom = std::clamp(newZoom, minZoom, maxZoom);

		float wxBefore, wyBefore;
		screenToWorld(sx, sy, wxBefore, wyBefore);

		zoom = newZoom;

		float wxAfter, wyAfter;
		screenToWorld(sx, sy, wxAfter, wyAfter);

		panX += (wxBefore - wxAfter);
		panY += (wyBefore - wyAfter);
	}
};