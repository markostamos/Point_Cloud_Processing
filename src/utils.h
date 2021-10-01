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



struct Point {
	vec pos;
	vec col;
	Point(vec inpos, vec incol) {
		pos = inpos;
		col = incol;
	}
};



//Κεντράρει το point_cloud και επιστρέφει το παλιό κέντρο βάρους(πρίν γίνει 0).
void center_point_cloud(std::vector<vec>& ptc,vec& old_cm);
//Επιστρέφει τον αριθμό των σημείων με  Z τιμές ανάμεσα στα lowerBound και UpperBound καθώς και τα indices των σημείων που βρίσκονται εκεί.
int get_point_count(const std::vector<Point>& ptc,const double lowerBound,const double upperBound,std::vector<int>& other);




int nearestCentroid(std::vector<vec> centroids, vec pt);

vec get_center_mass(std::vector<vec> ptc, std::vector<int> clusterpts);


bool equals(vec v1, vec v2,double dist);

bool isIn(const std::vector<int>& vec, int elem);

std::vector<int> getNeighbours(const std::vector<Point> ptc, Point pt,double e);
