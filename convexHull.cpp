#include "precomp.h"
#include "convexHull.h"

namespace Tmpl8 
{
	int Num_Threads = thread::hardware_concurrency();
	ThreadPool threadpool(Num_Threads);
	
	
ConvexHull::ConvexHull(vector<vec2> points) {
	_points = points;

	calcConvexHull();
}

ConvexHull::~ConvexHull() {
	
}

void ConvexHull::calcConvexHull() {
	// Sort points at x axis (or y axis if x is the same)
	sort->vec2Mergesort(_points, 0, _points.size() - 1);

	// Two threads for bottom and top half of calculating convexhull
	auto thread_top = threadpool.enqueue([this]() {
		bot.resize(_points.size());
		
		//Bottomhalf left to right
		for (size_t i = 0; i < _points.size(); i++) {
			while (botindex >= 2 && crossProd(bot[botindex - 2], bot[botindex - 1], _points[i]) <= 0)
				botindex--;
			bot[botindex++] = _points[i];
		}
		bot.resize(botindex);
	});

	auto thread_bottom = threadpool.enqueue([this]() {
		top.resize(_points.size());

		//Tophalf right to left
		for (size_t i = _points.size() - 1; i > 0; i--) {
			while (topindex >= 2 && crossProd(top[topindex - 2], top[topindex - 1], _points[i - 1]) <= 0)
				topindex--;
			top[topindex++] = _points[i - 1];
		}
		top.resize(topindex);
	});
	
	// Wait for both threads to be done
	thread_top.wait();
	thread_bottom.wait();

	// Merge bot and top vector together
	top.insert(top.end(), bot.begin(), bot.end()-1);

	resultPoints = top;
}

}
