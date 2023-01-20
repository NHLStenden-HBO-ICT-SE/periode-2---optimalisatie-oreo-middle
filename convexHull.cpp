#include "precomp.h"
#include "convexHull.h"

namespace Tmpl8 
{

ConvexHull::ConvexHull(vector<vec2> points) {
	_points = points;

	calcConvexHull();
}

ConvexHull::~ConvexHull() {

}

void ConvexHull::calcConvexHull() {
	// Sort points at x axis (or y axis if x is the same)
	sort->vec2Mergesort(_points, 0, _points.size() - 1);

	// List with size 2x the size of all points (for checking bottom and top points)
	vector<vec2> top_bot(2*_points.size());
	
	//Bottomhalf
	for (size_t i = 0; i < _points.size(); i++) {
		while (index >= 2 && crossProd(top_bot[index-2], top_bot[index-1], _points[i]) <= 0)
			index--;
		top_bot[index++] = _points[i];
	}

	//Tophalf
	int top_begin = index + 1;
	for (size_t i = _points.size() - 1; i > 0; i--) {
		while (index >= top_begin && crossProd(top_bot[index - 2], top_bot[index - 1], _points[i-1]) <= 0)
			index--;
		top_bot[index++] = _points[i-1];
	}

	//remove last point because its the same as the first one
	top_bot.resize(index - 1);

	resultPoints = top_bot;
}

}
