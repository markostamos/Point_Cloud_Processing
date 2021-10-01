#pragma once
#include <VVRScene/canvas.h>
#include <VVRScene/mesh.h>
#include <VVRScene/settings.h>
#include <VVRScene/utils.h>
#include <MathGeoLib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <set>
#include "symmetriceigensolver3x3.h"
#include "utils.h"






/*Χωρίζει τα σημεία σε #zDivision επίπεδα και ελέγχει για το επίπεδο με τα περισσότερα σημεία
 Έαν ordered = true ελέγχει μόνο τα πρώτα (κάτω προς πάνω ) τμήματα και σταματάει όταν ο αριθμός των inliers δεν βελτιώνεται για 2 συνεχόμενα επίπεδα.
*/
void groundRemoval(std::vector<Point>& ptc,bool ordered=true,int zDivision=10);
math::Plane planeEstimation(std::vector<Point>& ptc,vvr::Box3D box);


//Βρίσκει διαφορά σε δυο ιστογράμματα με τη μεθοδο chi-square
double chi_square_dist(std::vector<double> hist1, std::vector<double> hist2);


void Clustering_DBSCAN(std::vector<Point>& ptc,std::vector<int>& labels_out, int minPts, double e);

//Διατήρηση μόνο του αντικειμένου ενδιαφέροντος, καλείται μετά το σταδιο του clustering
void background_removal(std::vector<Point>& ptc, const std::vector<int>& labels, vec truth);
std::vector<double> get_histogram(const std::vector<Point>& ptc, int num_bins);
std::vector<vec> rgb_to_hsv(const std::vector<Point>& ptc);
//δημιουργεί το heatmap των δυο Point cloud
void generate_heatmap(const std::vector<Point>& pt2016, const std::vector<Point>& pt2020, std::vector<Point>& ptc1, std::vector<Point>& ptc2,double& maxDist);