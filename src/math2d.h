#pragma once
#include <cmath>
#include <algorithm>

namespace math2d {

	inline float dist2_point_segment(float px, float py,
		float ax, float ay,
		float bx, float by)
	{
		const float abx = bx - ax;
		const float aby = by - ay;
		const float apx = px - ax;
		const float apy = py - ay;

		const float ab2 = abx * abx + aby * aby;
		float t = 0.0f;
		if (ab2 > 1e-12f) {
			t = (apx * abx + apy * aby) / ab2;
			t = std::clamp(t, 0.0f, 1.0f);
		}

		const float cx = ax + t * abx;
		const float cy = ay + t * aby;

		const float dx = px - cx;
		const float dy = py - cy;
		return dx * dx + dy * dy;
	}

}