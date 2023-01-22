#include "precomp.h"
#include "sort.h"
namespace Tmpl8
{
    //ThreadPool sort_threadpool(4);
    Sort::Sort() {

    }

	void Sort::vec2Mergesort(vector<vec2>& points, int l, int r) {
        int mid = (l + r) / 2;
        if (l < r) {
            vec2Mergesort(points, l, mid);
            vec2Mergesort(points, mid + 1, r);
            vec2Merge(points, l, mid, r);
        }
	}
    void Sort::vec2Merge(vector<vec2>& points, int l, int mid, int r) {
        int lpos = l;
        int rpos = mid + 1;

        vector<vec2> temp = {};

        // Loop until end of lpos or rpos, choose larger one and place them in order
        while (lpos <= mid && rpos <= r) {
            if (points[lpos].x <= points[rpos].x) {
                temp.push_back(std::move(points[lpos]));
                lpos++;
            }
            else if (points[lpos].x == points[rpos].x) {
                if (points[lpos].y <= points[rpos].y) {
                    temp.push_back(std::move(points[lpos]));
                    lpos++;
                }
                else {
                    temp.push_back(std::move(points[rpos]));
                    rpos++;
                }
            }
            else {
                temp.push_back(std::move(points[rpos]));
                rpos++;
            }
        }
        while (lpos <= mid) {
            temp.push_back(std::move(points[lpos]));
            lpos++;
        }
        while (rpos <= r) {
            temp.push_back(std::move(points[rpos]));
            rpos++;
        }

        // Copy the elements from the temporary vector back to the original array
        for (size_t i = l; i <= r; i++)
            points[i] = std::move(temp[i - l]);
    }

}
