#include "scene.h"
#include "math2d.h"

#include <cmath>

static inline void pushLine(std::vector<float>& v, float x0, float y0, float x1, float y1) {
	v.push_back(x0);
	v.push_back(y0);
	v.push_back(x1);
	v.push_back(y1);
}

void Scene::buildTestScene() {
	m_segments.clear();

	const float x0 = -5.0f, y0 = -3.0f;
	const float x1 = 5.0f, y1 = 3.0f;

	m_segments.push_back({ x0, y0, x1, y0 });
	m_segments.push_back({ x1, y0, x1, y1 });
	m_segments.push_back({ x1, y1, x0, y1 });
	m_segments.push_back({ x0, y1, x0, y0 });

	// parede interna
	m_segments.push_back({ -1.0f, -3.0f, -1.0f, 3.0f });

	rebuildVerts();
}

void Scene::rebuildVerts() {
	m_verts.clear();
	m_verts.reserve(m_segments.size() * 4);

	for (const auto& s : m_segments) {
		pushLine(m_verts, s.x0, s.y0, s.x1, s.y1);
	}
}

int Scene::pick(float wx, float wy, float thresholdWorld) const {
	int best = -1;
	float bestD2 = thresholdWorld * thresholdWorld;

	for (int i = 0; i < (int)m_segments.size(); ++i) {
		const auto& s = m_segments[i];
		const float d2 = math2d::dist2_point_segment(wx, wy, s.x0, s.y0, s.x1, s.y1);
		if (d2 < bestD2) {
			bestD2 = d2;
			best = i;
		}
	}
	return best;
}