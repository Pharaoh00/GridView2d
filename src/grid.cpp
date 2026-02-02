#include "grid.h"

#include <cmath>

namespace grid {

	static float choose_step_pow2(float zoom) {
		const float worldPerPixel = 1.0f / zoom;
		const float targetStep = 80.0f * worldPerPixel;

		float step = 1.0f;
		while (step < targetStep) step *= 2.0f;
		while (step > targetStep * 2.0f) step *= 0.5f;
		return step;
	}

	static inline void addLine(std::vector<float>& v, float x0, float y0, float x1, float y1) {
		v.push_back(x0); v.push_back(y0);
		v.push_back(x1); v.push_back(y1);
	}

	Result build(float minX, float maxX, float minY, float maxY,
		float zoom, int majorEvery)
	{
		Result r;

		r.step = choose_step_pow2(zoom);
		r.majorStep = r.step * (float)majorEvery;

		const float step = r.step;
		minX -= step * 2; maxX += step * 2;
		minY -= step * 2; maxY += step * 2;

		const int approxVert = (int)std::ceil((maxX - minX) / step) + 8;
		const int approxHorz = (int)std::ceil((maxY - minY) / step) + 8;
		r.minor.reserve((approxVert + approxHorz) * 4);
		r.major.reserve(((approxVert + approxHorz) / majorEvery + 8) * 4);

		const int ix0 = (int)std::floor(minX / step);
		const int ix1 = (int)std::ceil(maxX / step);
		const int iy0 = (int)std::floor(minY / step);
		const int iy1 = (int)std::ceil(maxY / step);

		// verticais
		for (int ix = ix0; ix <= ix1; ++ix) {
			if (ix == 0) continue;
			const float x = ix * step;

			if ((ix % majorEvery) == 0) {
				addLine(r.major, x, minY, x, maxY);
			}
			else {
				addLine(r.minor, x, minY, x, maxY);
			}
		}

		// horizontais
		for (int iy = iy0; iy <= iy1; ++iy) {
			if (iy == 0) continue;
			const float y = iy * step;

			if ((iy % majorEvery) == 0) {
				addLine(r.major, minX, y, maxX, y);
			}
			else {
				addLine(r.minor, minX, y, maxX, y);
			}
		}

		return r;
	}
}
