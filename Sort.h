#pragma once
namespace Tmpl8
{
class Sort
{

private:
	void vec2MergeSortInterval(vector<vec2>& points, int l, int mid, int r);

public:
	Sort();

	void vec2Mergesort(vector<vec2>& points, int l, int r);
};

}


