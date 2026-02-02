#pragma once
#include <vector>
#include "segment.h"

class Scene {
public:
	void buildTestScene();
	void rebuildVerts();

	int pick(float wx, float wy, float thresholdWorld) const;

	void setSelected(int idx) { m_selected = idx; }
	int  selected() const { return m_selected; }

	const std::vector<float>& verts() const { return m_verts; }
	const std::vector<Segment>& segments() const { return m_segments; }

private:
	std::vector<Segment> m_segments;
	std::vector<float>   m_verts;
	int m_selected = -1;
};