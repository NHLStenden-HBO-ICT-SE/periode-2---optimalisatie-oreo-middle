#include "precomp.h"
#include "convexHull.h"

namespace Tmpl8 
{

ConvexHull::ConvexHull(vec2* points, int countOfPoints, bool closeGraph) {
	_boundingPoints = points;
	_countOfPoints = countOfPoints;
	_closeGraph = closeGraph;

	calcConvexHull();
}

ConvexHull::~ConvexHull() {
	// delete quadrant hullpoints
	delete q1Hullpoints;
	delete q2Hullpoints;
	delete q3Hullpoints;
	delete q4Hullpoints;

}

void ConvexHull::calcConvexHull() {

	// find the quadrant limits
	vec2* boundingPT = _boundingPoints;

	// for every quadrant 2 points
	vec2 q1point1 = *boundingPT;
	vec2 q1point2 = *boundingPT;
	vec2 q2point1 = *boundingPT;
	vec2 q2point2 = *boundingPT;
	vec2 q3point1 = *boundingPT;
	vec2 q3point2 = *boundingPT;
	vec2 q4point1 = *boundingPT;
	vec2 q4point2 = *boundingPT;

	boundingPT++;

	for (int n = _countOfPoints -1; n > 0; n--) {
		vec2& point = *boundingPT;

		//rightside
		if (point.x >= q1point1.x) {
			if (point.x == q1point1.x) {
				if (point.y > q1point1.y) {
					q1point1 = point;
				}
				else if(point.y < q4point2.y) {
					q4point2 = point;
				}
			}
			else {
				q1point1 = point;
				q4point2 = point;
			}
		}

		//leftside
		if (point.x <= q2point2.x) {
			if (point.x == q2point2.x) {
				if (point.y > q2point2.y) {
					q2point2 = point;
				}
				else if (point.y < q3point1.y) {
					q3point1 = point;
				}
			}
			else {
				q2point2 = point;
				q3point1 = point;
			}
		}

		//top
		if (point.y >= q1point2.y) {
			if (point.y == q1point2.y) {
				if (point.x < q2point1.x) {
					q2point1 = point;
				}
				else if (point.x > q2point1.x) {
					q1point2 = point;
				}
			}
			else {
				q1point2 = point;
				q2point1 = point;
			}
		}

		//bottom
		if (point.y <= q3point2.y) {
			if (point.y == q3point2.y) {
				if (point.x < q3point2.x) {
					q3point2 = point;
				}
				else if (point.x > q4point1.x) {
					q4point1 = point;
				}
			}
			else {
				q3point2 = point;
				q4point1 = point;
			}
		}

		boundingPT++;
	}

	vec2 q1originPoint = (q1point2.x, q1point1.y);
	vec2 q2originPoint = (q2point1.x, q2point2.y);
	vec2 q3originPoint = (q3point2.x, q3point1.y);
	vec2 q4originPoint = (q4point1.x, q4point2.y);


	//quadrant 1 init
	q1HullCapacity = _qHullpointinitcount;
	q1Hullpoints = new vec2[q1HullCapacity];

	q1Hullpoints[0] = q1point1;

	if (q1point1.x == q1point2.x && q1point1.y == q1point2.y) {
		q1HullCount = 1;
	}
	else {
		q1Hullpoints[1] = q1point2;
		q1HullCount = 2;
	}

	//quadrant 2 init
	q2HullCapacity = _qHullpointinitcount;
	q2Hullpoints = new vec2[q2HullCapacity];

	q2Hullpoints[0] = q2point1;

	if (q2point1.x == q2point2.x && q2point1.y == q2point2.y) {
		q2HullCount = 1;
	}
	else {
		q2Hullpoints[1] = q2point2;
		q2HullCount = 2;
	}

	//quadrant 3 init
	q3HullCapacity = _qHullpointinitcount;
	q3Hullpoints = new vec2[q3HullCapacity];

	q3Hullpoints[0] = q3point1;

	if (q3point1.x == q3point2.x && q3point1.y == q3point2.y) {
		q3HullCount = 1;
	}
	else {
		q3Hullpoints[1] = q3point2;
		q3HullCount = 2;
	}

	//quadrant 4 init
	q4HullCapacity = _qHullpointinitcount;
	q4Hullpoints = new vec2[q4HullCapacity];

	q4Hullpoints[0] = q4point1;

	if (q4point1.x == q4point2.x && q4point1.y == q4point2.y) {
		q4HullCount = 1;
	}
	else {
		q4Hullpoints[1] = q4point2;
		q4HullCount = 2;
	}

	// calculations 
	int index;
	int low;
	int high;

	boundingPT = _boundingPoints;

	for (int n = _countOfPoints - 1; n >= 0; n--) {
		vec2& point = *boundingPT;

		//quadrant 1 calc
		if (point.x > q1originPoint.x && point.y > q1originPoint.y) { // if point is in the first quadrant
			low = 0;
			high = q1HullCount;

			while (low < high-1) {
				index = ((high - low) >> 1) + low;

				if (point.x <= q1Hullpoints[index].x && point.y <= q1Hullpoints[index].y) { // does quadrant contain point?
					goto currentPointNotPartOfq1Hull;
				}
				else if (point.x > q1Hullpoints[index].x) {
					high = index;
					continue;
				}
				else if (point.x < q1Hullpoints[index].x) {
					low = index;
					continue;
				}

				low = index - 1;
				high = index + 1;
				break;
			}

			if (!right_turn(q1Hullpoints[low], q1Hullpoints[high], point)) {
				goto currentPointNotPartOfq1Hull;
			}

			while (low > 0) {
				if (right_turn(q1Hullpoints[low - 1], point, q1Hullpoints[low])) {
					break;
				}
				low--;
			}

			int maxHigh = q1HullCount - 1;
			while (high < maxHigh) {
				if (right_turn(point, q1Hullpoints[high + 1], q1Hullpoints[high])) {
					break;
				}
				high++;
			}

			if (low + 1 == high) {
				insertPoint(q1Hullpoints, low + 1, point, q1HullCount, q1HullCapacity);
				goto nextPoint;
			}
			else if (low + 2 == high) {
				q1Hullpoints[low + 1] = *boundingPT;
				goto nextPoint;
			}
			else {
				q1Hullpoints[low + 1] = *boundingPT;
				removeRange(q1Hullpoints, low + 2, high -1, q1HullCount);
				goto nextPoint;
			}
		}
	currentPointNotPartOfq1Hull:

		//quadrant 2 calc
		if (point.x < q2originPoint.x && point.y > q2originPoint.y) { // if point is in the first quadrant
			low = 0;
			high = q2HullCount;

			while (low < high - 1) {
				index = ((high - low) >> 1) + low;

				if (point.x >= q2Hullpoints[index].x && point.y <= q2Hullpoints[index].y) { // does quadrant contain point?
					goto currentPointNotPartOfq2Hull;
				}
				else if (point.x > q2Hullpoints[index].x) {
					high = index;
					continue;
				}
				else if (point.x < q2Hullpoints[index].x) {
					low = index;
					continue;
				}

				low = index - 1;
				high = index + 1;
				break;
			}

			if (!right_turn(q2Hullpoints[low], q2Hullpoints[high], point)) {
				goto currentPointNotPartOfq2Hull;
			}

			while (low > 0) {
				if (right_turn(q2Hullpoints[low - 1], point, q2Hullpoints[low])) {
					break;
				}
				low--;
			}

			int maxHigh = q2HullCount - 1;
			while (high < maxHigh) {
				if (right_turn(point, q2Hullpoints[high + 1], q2Hullpoints[high])) {
					break;
				}
				high++;
			}

			if (low + 1 == high) {
				insertPoint(q2Hullpoints, low + 1, point, q2HullCount, q2HullCapacity);
				goto nextPoint;
			}
			else if (low + 2 == high) {
				q2Hullpoints[low + 1] = *boundingPT;
				goto nextPoint;
			}
			else {
				q2Hullpoints[low + 1] = *boundingPT;
				removeRange(q2Hullpoints, low + 2, high - 1, q2HullCount);
				goto nextPoint;
			}
		}

	currentPointNotPartOfq2Hull:

		//quadrant 3 calc
		if (point.x < q3originPoint.x && point.y < q3originPoint.y) { // if point is in the first quadrant
			low = 0;
			high = q3HullCount;

			while (low < high - 1) {
				index = ((high - low) >> 1) + low;

				if (point.x >= q3Hullpoints[index].x && point.y >= q3Hullpoints[index].y) { // does quadrant contain point?
					goto currentPointNotPartOfq3Hull;
				}
				else if (point.x < q3Hullpoints[index].x) {
					high = index;
					continue;
				}
				else if (point.x > q3Hullpoints[index].x) {
					low = index;
					continue;
				}

				low = index - 1;
				high = index + 1;
				break;
			}

			if (!right_turn(q3Hullpoints[low], q3Hullpoints[high], point)) {
				goto currentPointNotPartOfq3Hull;
			}

			while (low > 0) {
				if (right_turn(q3Hullpoints[low - 1], point, q3Hullpoints[low])) {
					break;
				}
				low--;
			}

			int maxHigh = q3HullCount - 1;
			while (high < maxHigh) {
				if (right_turn(point, q3Hullpoints[high + 1], q3Hullpoints[high])) {
					break;
				}
				high++;
			}

			if (low + 1 == high) {
				insertPoint(q3Hullpoints, low + 1, point, q3HullCount, q3HullCapacity);
				goto nextPoint;
			}
			else if (low + 2 == high) {
				q3Hullpoints[low + 1] = *boundingPT;
				goto nextPoint;
			}
			else {
				q3Hullpoints[low + 1] = *boundingPT;
				removeRange(q3Hullpoints, low + 2, high - 1, q3HullCount);
				goto nextPoint;
			}
		}
	currentPointNotPartOfq3Hull:

		//quadrant 4 calc
		if (point.x > q4originPoint.x && point.y < q4originPoint.y) { // if point is in the first quadrant
			low = 0;
			high = q4HullCount;

			while (low < high - 1) {
				index = ((high - low) >> 1) + low;

				if (point.x <= q4Hullpoints[index].x && point.y >= q4Hullpoints[index].y) { // does quadrant contain point?
					goto currentPointNotPartOfq4Hull;
				}
				else if (point.x < q4Hullpoints[index].x) {
					high = index;
					continue;
				}
				else if (point.x > q4Hullpoints[index].x) {
					low = index;
					continue;
				}

				low = index - 1;
				high = index + 1;
				break;
			}

			if (!right_turn(q4Hullpoints[low], q4Hullpoints[high], point)) {
				goto currentPointNotPartOfq4Hull;
			}

			while (low > 0) {
				if (right_turn(q4Hullpoints[low - 1], point, q4Hullpoints[low])) {
					break;
				}
				low--;
			}

			int maxHigh = q4HullCount - 1;
			while (high < maxHigh) {
				if (right_turn(point, q4Hullpoints[high + 1], q4Hullpoints[high])) {
					break;
				}
				high++;
			}

			if (low + 1 == high) {
				insertPoint(q4Hullpoints, low + 1, point, q4HullCount, q4HullCapacity);
				goto nextPoint;
			}
			else if (low + 2 == high) {
				q4Hullpoints[low + 1] = *boundingPT;
				goto nextPoint;
			}
			else {
				q4Hullpoints[low + 1] = *boundingPT;
				removeRange(q4Hullpoints, low + 2, high - 1, q4HullCount);
				goto nextPoint;
			}
		}
	currentPointNotPartOfq4Hull:

	nextPoint:
		boundingPT++;
	}
}

void ConvexHull::insertPoint(vec2*& boundingPoint, int index, vec2& point, int& count, int& capacity) {

	if (count >= capacity) {
		int newCapacity = capacity * 2;
		vec2* newPointArray = new vec2[newCapacity];
		memmove(newPointArray, boundingPoint, capacity * sizeof(vec2));
		delete boundingPoint;
		boundingPoint = newPointArray;
		capacity = newCapacity;
	}

	memmove(&(boundingPoint[index + 1]), &(boundingPoint[index]), (count - index) * sizeof(vec2));

	boundingPoint[index] = point;
	count++;
}

void ConvexHull::removeRange(vec2* boundingPoint, int iStart, int iEnd, int& count) {
	memmove(&(boundingPoint[iStart]), &(boundingPoint[iEnd + 1]), (count - iEnd) * sizeof(vec2));
	count -= (iEnd - iStart + 1);
}

vec2* ConvexHull::getResultAsArray(int& pointCount) {
	pointCount = 0;

	if (_countOfPoints == 0) {
		return NULL;
	}

	int indexQ1start = 0;
	int indexQ2start;
	int indexQ3start;
	int indexQ4start;

	int indexQ1end = q1HullCount - 1;
	int indexQ2end;
	int indexQ3end;
	int indexQ4end;

	vec2 lastPoint = q1Hullpoints[indexQ1end];

	if (q2HullCount == 1) {
		if (*q2Hullpoints == lastPoint) {
			indexQ2start = 1;
			indexQ2end = 0;
		}
		else {
			indexQ2start = 0;
			indexQ2end = 0;
			lastPoint = *q2Hullpoints;
		}
	}
	else {
		if (*q2Hullpoints == lastPoint) {
			indexQ2start = 1;
		}
		else {
			indexQ2start = 0;
		}
		indexQ2end = q2HullCount - 1;
		lastPoint = q2Hullpoints[indexQ2end];
	}

	if (q3HullCount == 1) {
		if (*q3Hullpoints == lastPoint) {
			indexQ3start = 1;
			indexQ3end = 0;
		}
		else {
			indexQ3start = 0;
			indexQ3end = 0;
			lastPoint = *q3Hullpoints;
		}
	}
	else {
		if (*q3Hullpoints == lastPoint) {
			indexQ3start = 1;
		}
		else {
			indexQ3start = 0;
		}
		indexQ3end = q3HullCount - 1;
		lastPoint = q3Hullpoints[indexQ3end];
	}

	if (q4HullCount == 1) {
		if (*q4Hullpoints == lastPoint) {
			indexQ4start = 1;
			indexQ4end = 0;
		}
		else {
			indexQ4start = 0;
			indexQ4end = 0;
			lastPoint = *q4Hullpoints;
		}
	}
	else {
		if (*q4Hullpoints == lastPoint) {
			indexQ4start = 1;
		}
		else {
			indexQ4start = 0;
		}
		indexQ4end = q4HullCount - 1;
		lastPoint = q4Hullpoints[indexQ4end];
	}

	if (q1Hullpoints[indexQ1start] == lastPoint) {
		indexQ1start++;
	}

	int countOfFinalHullPoint = (indexQ1end - indexQ1start) +
		(indexQ2end - indexQ2start) +
		(indexQ3end - indexQ3start) +
		(indexQ4end - indexQ4start) + 4;

	if (countOfFinalHullPoint <= 1) {
		return new vec2[1]{ lastPoint };
	}

	if (countOfFinalHullPoint > 1 && _closeGraph) {
		countOfFinalHullPoint++;
	}

	result = new vec2[countOfFinalHullPoint];

	int resIndex = 0;

	for (int n = indexQ1start; n <= indexQ1end; n++) {
		result[resIndex] = q1Hullpoints[n];
		resIndex++;
	}
	for (int n = indexQ2start; n <= indexQ2end; n++) {
		result[resIndex] = q4Hullpoints[n];
		resIndex++;
	}
	for (int n = indexQ3start; n <= indexQ3end; n++) {
		result[resIndex] = q3Hullpoints[n];
		resIndex++;
	}
	for (int n = indexQ4start; n <= indexQ4end; n++) {
		result[resIndex] = q4Hullpoints[n];
		resIndex++;
	}

	if (countOfFinalHullPoint > 1 && _closeGraph)
	{
		result[resIndex] = result[0];
	}

	pointCount = countOfFinalHullPoint;

	return result;

}

}
