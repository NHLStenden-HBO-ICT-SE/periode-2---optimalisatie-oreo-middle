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
	sort = new Sort();
	sort->vec2Mergesort(_points, 0, _points.size() - 1);

	size_t index = 0;

	// List with size 2x the size of all points (for checking bottom and top points)
	vector<vec2> topNbot(2*_points.size());
	
	//Bottomhalf
	for (size_t i = 0; i < _points.size(); i++) {
		while (index >= 2 && crossProd(topNbot[index-2], topNbot[index-1], _points[i]) <= 0)
			index--;
		topNbot[index++] = _points[i];
	}

	//Tophalf
	int b = index + 1;
	for (size_t i = _points.size() - 1; i > 0; i--) {
		while (index >= b && crossProd(topNbot[index - 2], topNbot[index - 1], _points[i-1]) <= 0)
			index--;
		topNbot[index++] = _points[i-1];
	}

	//remove last point because its the same as the first one
	topNbot.resize(index - 1);

	resultPoints = topNbot;
}

}
