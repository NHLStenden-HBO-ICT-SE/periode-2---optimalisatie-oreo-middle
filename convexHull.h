#pragma once
namespace Tmpl8 {

	class Sort;
class ConvexHull
{

private:
	vector<vec2> _points;
	vector<vec2> resultPoints;
	size_t index = 0;

	std::unique_ptr<Sort> sort;
	
	int crossProd(vec2 o, vec2 a, vec2 b) { return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x); };
	void calcConvexHull();

public:
	ConvexHull(vector<vec2> points);
	~ConvexHull();

	vector<vec2> getResultslist() { return resultPoints; };
};
}


