#include "precomp.h"
#include "sort.h"
namespace Tmpl8
{
    Sort::Sort() {

    }

	void Sort::vec2Mergesort(vector<vec2>& points, int l, int r) {
        int mid = (l + r) / 2;
        if (l < r) {
            vec2Mergesort(points, l, mid);
            vec2Mergesort(points, mid + 1, r);
            vec2MergeSortInterval(points, l, mid, r);
        }
	}
    void Sort::vec2MergeSortInterval(vector<vec2>& points, int l, int mid, int r) {
        vector<vec2> temp;

        int lpos = l;
        int rpos = mid + 1;

        while (lpos <= mid && rpos <= r) {
            if (points[lpos].x <= points[rpos].x) {
                temp.push_back(points[lpos]);
                lpos++;
            }
            else if (points[lpos].x == points[rpos].x) {
                if (points[lpos].y <= points[rpos].y) {
                    temp.push_back(points[lpos]);
                    lpos++;
                }
                else {
                    temp.push_back(points[rpos]);
                    rpos++;
                }
            }
            else {
                temp.push_back(points[rpos]);
                rpos++;
            }
        }
        while (lpos <= mid) {
            temp.push_back(points[lpos]);
            lpos++;
        }
        while (rpos <= r) {
            temp.push_back(points[rpos]);
            rpos++;
        }

        for (int i = l; i <= r; i++) {
            int j = i - l;
            points[i] = temp[j];
        }
    }

}
