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
#include <filesystem>
#include "algorithms.h"

#define FLAG_SHOW_AXES       1
#define FLAG_SHOW_WIRE       2
#define FLAG_SHOW_SOLID      4
#define FLAG_SHOW_NORMALS    8
#define FLAG_SHOW_PLANE     16
#define FLAG_SHOW_AABB      32

 

struct groundTruth {
	vec pos;
	std::string label;
	std::vector<std::string>lines;
};

class Mesh3DScene : public vvr::Scene
{
public:
	Mesh3DScene();
	const char* getName() const { return "Something_Changed 1059391"; }
	void keyEvent(unsigned char key, bool up, int modif) override;
	void arrowEvent(vvr::ArrowDir dir, int modif) override;


	
	void initialize(std::string input);
	void change_scene();


	
	//Tasks
	void task1(int modif);
	void task2_clustering(int modif);
	void task3_background_removal();
	void task4_registration();
	void task5_ColorComp();
	void task6_Classify();
	
	
private:
	void draw() override;
	void reset() override;
	void resize() override;
	
	vvr::Box3D create_bounding_box(std::vector<vec> pts);
	std::vector<vec> load_point_cloud(std::string year, std::string num, std::string type);
	std::vector<std::string> load_folder_names();
	void center_point_clouds(std::vector<vec>& pt2016, std::vector<vec>& pt2020);
	void draw_point_cloud(const std::vector<Point>& pts, int offset);
	void draw_point_cloud(const std::vector<Point>& ptc, const std::vector<int> pts_to_draw, const int offset);
	void draw_bounding_box(vvr::Box3D box, int offset, vvr::Colour col);
private:
	
	math::Plane m_plane;
	
	//FLAGS
	bool show_bounding_box;
	int show_task;      //0:initial  //1:task1  etc..
	bool show_plane;
	vvr::Point3D testpt;


	//point Clouds για ένα subscene από τα έτη 2016,2020
	std::vector<Point> pt2020,pt2016;

	//bounding boxes
	vvr::Box3D box2016, box2020;

	//ground Truth απο τα csv
	groundTruth truth;
	
	//heatmap ptc
	std::vector<Point> heatmapPtc2016,heatmapPtc2020;

	//histograms
	std::vector<double> hist2016, hist2020;
	
	
	// τα indices των clusters a: αντικείμενο ενδιαφέροντος b:αντικείμενα από background
	std::vector<int> labels2020,labels2016;
	int numClusters2016, numClusters2020;

	std::vector<std::vector<int>> clusters_many;
	std::vector<vec> centroids_many;

	//classification
	double chival,maxDist;


};