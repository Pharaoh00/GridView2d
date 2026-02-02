#pragma once
#include <vector>

namespace grid {

	struct Result {
		std::vector<float> minor;
		std::vector<float> major;
		float step = 1.0f;
		float majorStep = 10.0f;
	};

	Result build(float minX, float maxX, float minY, float maxY, float zoom, int majorEvery = 5);

}