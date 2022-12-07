#pragma once
namespace Tmpl8 {

class ConvexHull
{
private:
	vec2* _boundingPoints;
	int _countOfPoints;
	bool _closeGraph;

	const int _qHullpointinitcount = 2500; // moet verandert worden naar tanks.size()
	const int _qHullpointgrowcount = 2500;

	// following quadrands of points
	vec2* q1Hullpoints;
	vec2* q2Hullpoints;
	vec2* q3Hullpoints;
	vec2* q4Hullpoints;
	vec2* q1HullpointLast;
	vec2* q2HullpointLast;
	vec2* q3HullpointLast;
	vec2* q4HullpointLast;
	int q1HullCapacity, q2HullCapacity, q3HullCapacity, q4HullCapacity;
	int q1HullCount = 0, q2HullCount = 0, q3HullCount = 0, q4HullCount = 0;

	vec2* result; // for GetResultAsArray function

	void calcConvexHull();
	inline static void insertPoint(vec2*& boundingPoint, int index, vec2& point, int& count, int& capacity);
	inline static void removeRange(vec2* boundingPoint, int iStart, int iEnd, int& count);

public:
	ConvexHull(vec2* points, int countOfPoints, bool closeGraph = true);
	~ConvexHull();

	/* Left-turn, right-turn and collinear predicates */
	#define area(a, b, c) (((b).x-(a).x)*((c).y-(a).y) \
								 - ((b).y-(a).y)*((c).x-(a).x))
	#define right_turn(a, b, c) (area(a, b, c) < 0)
	#define left_turn(a, b, c) (area(a, b, c) > 0)
	#define collinear(a, b, c) (area(a, b, c) == 0)


	vec2* getResultAsArray(int& count);
};
}


