
#include "algorithms.h"
using namespace std;




void groundRemoval(std::vector<Point>& ptc, bool ordered,int zDivision)
{
	double minZ=inf;
	double maxZ=-inf;
	for (const Point& point : ptc) {
		vec pt = point.pos;
		if (pt.z > maxZ) maxZ = pt.z;
		if (pt.z < minZ) minZ = pt.z;
	}
	
	//περίπτωση που υπάρχει μόνο ground
	if ((maxZ - minZ) < 0.4) {
		ptc = vector<Point>();
		return;
	}
	double zStep = abs((maxZ - minZ))/zDivision;

	int bestCnt = 0;
	vector<int> newptc_indices;
	int not_improved_cnt = 0;
	// παρόλο που μπορουμε να αφαιρέσουμε μόνο τα πρώτα σημεία όπου Ζ<Zmin+zStep
	// επιλέγεται να κοιτάξουμε τουλάχιστον τα πρώτα 3 παραλληλεπίπεδα καθώς
	// το minZ Μπορεί να είναι λάθος / να οφείλεται σε θόρυβο
	// θέλουμε να κανουμε τον διαχωρισμο εκεί όπου υπάρχουν τα περισσότερα σημεια
	// που θα επηρρεάσουν τα υπόλοιπα στάδια. 
	for (int i = 0; i < zDivision; i++) {
		
		double lowerBound = minZ + zStep * i;
		double upperBound = minZ + zStep * (i+1);

		vector<int> tmp;
		int n = get_point_count(ptc, lowerBound, upperBound,tmp);

		if (n >= bestCnt) {
			
			bestCnt = n;
			newptc_indices = tmp;
			
		}
		else {
			not_improved_cnt++;
		}

		//Αν δεν αλλάξει ο μέγιστος αριθμός σημείων εδάφους για 3 επαναλήψεις 
		// τοτε σταματάει το Loop.
		if (ordered) {
			if (not_improved_cnt == 3) {
				break;
			}
		}
	}

	//διατήρηση των στοιχείων που δεν ανήκουν στο έδαφος
	vector<Point> newptc;
	for (int i : newptc_indices) {
		newptc.push_back(ptc[i]);
	}
	
	ptc = newptc;
	
}

math::Plane planeEstimation(std::vector<Point>& ptc,vvr::Box3D box) {
	
	int N = 100;
	double thr = 0.2;
	
	double zRange = box.z2 - box.z1;

	vector<Point> tmp;
	for (const Point& pt : ptc) {
		if (pt.pos.z < (box.z1 + zRange / 10)) {
			tmp.push_back(pt);
		}
	}



	int mostInliers = 0;
	Plane bestPlane;
	for (int i = 0; i < N; i++) {
		int inliers = 0;
		int ind1, ind2, ind3;
		vec pt1, pt2, pt3;
		while (true) {
			 ind1 = rand() % tmp.size();
			 ind2 = rand() % tmp.size();
			 ind3 = rand() % tmp.size();
			  pt1 = tmp[ind1].pos;
			  pt2 = tmp[ind2].pos;
			  pt3 = tmp[ind3].pos;

			 if (!pt1.AreCollinear(pt1, pt2, pt3)) {
				 break;
			 }
			
		}
		

		

		Plane pl = Plane(pt1, pt2, pt3);
		
		for (Point pt : tmp) {
			if (pl.Distance(pt.pos) <= thr) {
				inliers++;
			}
		}

		if (inliers > mostInliers) {
			mostInliers = inliers;
			bestPlane = pl;
		}

	}
	
	vec ptOnPlane = bestPlane.PointOnPlane();
	
	
	//Remove Ground
	vector<Point> newptc;
	
	for (int i = 0; i < ptc.size(); i++) {

		if (ptc[i].pos.z - ptOnPlane.z > thr) {
			newptc.push_back(ptc[i]);
			
		}
	}

	ptc = newptc;



	return bestPlane;
}



void background_removal(std::vector<Point>& ptc, const std::vector<int>& labels, vec truth)
{
	double minDist = inf;
	int cluster = -1;
	
	for (int i = 0; i < labels.size(); i++) {
		if (labels[i] >= 0) {
			double d = ptc[i].pos.Distance(truth);
			if (d < minDist) {
				minDist = d;
				cluster = labels[i];
			}
		}
	}

	vector<Point> newptc;


	for (int i = 0; i < labels.size(); i++) {
		if (labels[i]==cluster) {
			newptc.push_back(ptc[i]);
			

		}
	}
	ptc = newptc;
}


vector<Point> get_KNN(Point pt, const vector<Point>& ptc, int K) {
	vector<Point> nneighbors;



	auto getMaxDist = [](vector<Point> list, Point item, int& best_index) {
		double maxDist = 0;
		for (int i = 0; i < list.size(); i++) {
			double dist = list[i].pos.Distance(item.pos);
			if (dist > maxDist) {
				maxDist = dist;
				best_index = i;
			}
		}
		return maxDist;
	};


	for (int i = 0; i < ptc.size(); i++) {
		if (nneighbors.size() < K) {
			nneighbors.push_back(ptc[i]);
			continue;
		}
		int rem_index;
		double maxDist = getMaxDist(nneighbors, pt,rem_index);

		if (ptc[i].pos.Distance(pt.pos) < maxDist) {
			nneighbors[rem_index] = ptc[i];
		}
	}

	return nneighbors;
}

vector<double> get_Avg_Distances(const std::vector<Point>& ptc1, const std::vector<Point>& ptc2,int num_neighbors) {
	vector<double> distances;
	for (int i = 0; i < ptc1.size(); i++) {
		vector<Point> neighbors = get_KNN(ptc1[i], ptc2, num_neighbors);

		double avg_dist = 0;
		for (auto n : neighbors) {
			avg_dist += n.pos.Distance(ptc1[i].pos);
		}
		avg_dist /= num_neighbors;
		distances.push_back(avg_dist);
	}
	
	return distances;
}

void modifyColour(std::vector<Point>& ptc, vector<double>& distances,double maxDist) {
	
	
	for (int i = 0; i < ptc.size(); i++) {
		double dist = distances[i] / maxDist;

		double r, g, b;
		if (dist < 0.5) {
			r = Lerp(0, 125, dist);
			b = Lerp(255, 125, dist);
			g = Lerp(0, 255, dist);
		}
		else {
			r = Lerp(125, 255, dist);
			b = Lerp(125, 0, dist);
			g = Lerp(250, 0, dist);
		}

		ptc[i].col = vec(r, g, b);
	}
}
void generate_heatmap(const std::vector<Point>& pt2016, const std::vector<Point>& pt2020, std::vector<Point>& ptc1, std::vector<Point>& ptc2,double& maxDist)
{
	int num_neighbors = 2;
	double dist_thres = 0.3;

	ptc1 = pt2016;
	ptc2 = pt2020;

	vector<double> distances1;
	vector<double> distances2;
	vector<int> closest_neighbors;
	
	distances1 = get_Avg_Distances(ptc1, ptc2, num_neighbors);
	distances2 = get_Avg_Distances(ptc2, ptc1, num_neighbors);
	double maxDist1 = *max_element(distances1.begin(), distances1.end());
	double maxDist2 = *max_element(distances2.begin(), distances2.end());
	
	maxDist = max(maxDist1, maxDist2);

	modifyColour(ptc1, distances1,maxDist1);
	modifyColour(ptc2, distances2,maxDist2);

}



double chi_square_dist(std::vector<double> hist1, std::vector<double> hist2)
{
	double chiVal = 0;
	for (int i = 0; i < hist1.size(); i++) {
		double x = double(hist1[i]);
		double y = double(hist2[i]);
		
		if ((x + y) == 0) continue;
		chiVal += (x - y) * (x - y)/(x+y);
		

	}
	return chiVal;
}

void Clustering_DBSCAN(std::vector<Point>& ptc,vector<int>& labels_out,int minPts,double e) {

	vector<Point> newptc;

	//Δειγματοληψία για τα πολύ μεγάλα Point clouds
	if (ptc.size() > 15000) {
		for (int i = 0; i < ptc.size(); i++) {

			if (i % 10 == 0) {
				newptc.push_back(ptc[i]);
				
			}


			
		}
		ptc = newptc;
	}
	
	int C = 0;
	
	vector<vector<int>> clusters;
	
	//-1 undefined
	//-2 noise
	//index--> deixnei ton cluster
	vector<int>labels(ptc.size(),-1);
	
	for (int i = 0; i < ptc.size(); i++) {
		if (labels[i] != -1) continue;

		vector<int> neighbors = getNeighbours(ptc, ptc[i],e);

		if (neighbors.size() < minPts) {
			labels[i] = -2;
			continue;
		}
		C++;
		
		labels[i] = C;
		vector<int> S = neighbors;
		for (int j = 0;j<S.size();j++) {
			int Q = S[j];
			if (labels[Q]==-2) {
				labels[Q] = C;
			}
			if (labels[Q] != -1) {
				continue;
			}
			labels[Q] = C;

			vector<int> N = getNeighbours(ptc, ptc[Q], e);

			if (N.size() > minPts) {
				S.insert(S.end(), N.begin(), N.end());
			}

		}

	}

	
	labels_out = labels;
}


std::vector<vec> rgb_to_hsv(const std::vector<Point>& ptc) {
	
	vector<vec> hsv;

	for (const Point& pt : ptc) {
		vector<double> rgb = { pt.col[0] / 255,pt.col[1] / 255, pt.col[2] / 255 };

		double Cmax = *max_element(rgb.begin(), rgb.end());
		double Cmin = *min_element(rgb.begin(), rgb.end());
		double delta = Cmax - Cmin;

		double h,s,v;
		double r, g, b;
		r = rgb[0];
		g = rgb[1];
		b = rgb[2];

		if (delta == 0) h = 0;
		else if (Cmax == r) {
			h = 60 * fmod(((g - b) / delta), 6);
		}
		else if (Cmax==g ){
			h = 60 * (((b - r) / delta) + 2  );
		}
		else if (Cmax == b) {
			h = 60 * (((r - g) / delta) + 4);
		}


		if (Cmax == 0) {
			s = 0;
		}
		else {
			s = delta / Cmax;
		}

		v = Cmax;


		if (s < 0) s = 0;
		if (s > 1)  s = 1;
		if (v < 0) v = 0;
		if (v > 1) v = 1;
		if (h < 0) h = 360 + h;


		hsv.push_back(vec(h, s, v));
		
	}

	return hsv;

}

std::vector<double> get_histogram(const std::vector<Point>& ptc,int num_bins) {

	vector<vec> hsv = rgb_to_hsv(ptc);
	
	
	
	vector<double> hist(num_bins, 0);

	double x = 360 / num_bins;
	for (int i = 0; i < hsv.size(); i++) {
		int hval = floor(hsv[i][0]/x);

		hist[hval] += 1;

	}

	
	for (int i = 0; i < hist.size(); i++) {
		hist[i] = hist[i] / hsv.size();
	}

	
	
	
	return hist;

}