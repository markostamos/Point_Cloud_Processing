
#include "utils.h"

using namespace std;
void center_point_cloud(vector<vec>& ptc, vec& old_cm) {
	
		vec cm(0, 0, 0);
		for (const vec& pt : ptc) {
			cm += pt;
		}
		cm /= ptc.size();
		old_cm = cm;
		
		for (vec& pt : ptc) {

			pt -= cm;
		}

}

int get_point_count(const vector<Point>& ptc, const double lowerBound, const double upperBound,vector<int>& other)
{
	int cnt = 0;
	
	for (int i = 0; i < ptc.size(); i++) {
		if (ptc[i].pos.z <= upperBound && ptc[i].pos.z >= lowerBound) {
			cnt++;
		}
		else {
			other.push_back(i);
		}
	}


	return cnt;
}



int nearestCentroid(vector<vec> centroids, vec pt) {

	int best = -1;
	double minDist = inf;
	for (int i = 0; i < centroids.size(); i++) {
		//double d = dist2D(centroids[i], pt);
		double d = centroids[i].Distance(pt);
		
		if (d < minDist) {
			minDist = d;
			best = i;
		}
	}

	return best;
}



vec get_center_mass(std::vector<vec> ptc, std::vector<int> clusterpts)
{
	vec cm(0, 0, 0);

	for (int i : clusterpts) {
		cm += ptc[i];
	}
	cm /= clusterpts.size();
	return cm;
}

bool equals(vec v1, vec v2,double dist)
{
	if (abs(v1.x - v2.x) <= dist && abs(v1.y - v2.y) <= dist) {
		return true;
	}
	else return false;
	
}

bool isIn(const std::vector<int>& vec, int elem)
{
	bool result = false;
	if (find(vec.begin(), vec.end(), elem) != vec.end())
	{
		result = true;
	}
	return result;
}

std::vector<int> getNeighbours(const std::vector<Point> ptc, Point pt,double e)
{
	vector<int> neighbours;
	for (int i = 0; i < ptc.size(); i++) {
		if (pt.pos.Distance(ptc[i].pos) < e) {
			neighbours.push_back(i);
		}
	}
	return neighbours;
}

