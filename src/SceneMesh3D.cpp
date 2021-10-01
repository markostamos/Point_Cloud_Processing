#include "SceneMesh3D.h"
#include <filesystem>

std::string FOLDER_NAME = "0_5D4KVPBP";
int folder_index = 0;
std::vector<std::string> FOLDERS;
std::string SUBSCENE = "";

using namespace std;
using namespace vvr;

Mesh3DScene::Mesh3DScene()
{
	//! Load settings.
	vvr::Shape::DEF_LINE_WIDTH = 4;
	vvr::Shape::DEF_POINT_SIZE = 6;
	m_perspective_proj = false; // zoom
	//m_bg_col = Colour("768E77");
	m_bg_col = Colour("19191A");
	m_hide_log = false;
	m_create_menus = false;
	m_fullscreen = false;

	FOLDERS = load_folder_names();
	reset();
}

vector<vec> Mesh3DScene::load_point_cloud(string year = "2020", string subscene = "0", string type = "xyz")
{
	//"carla_pointcloud"
	// load point cloud
	vector<vec> point_cloud;
	SUBSCENE = subscene;
	const string pcldir = getBasePath() + "resources/extracted_las_files/" + FOLDER_NAME + "/" + year + "/";

	const string pclFile = pcldir + FOLDER_NAME + "_" + subscene + "_" + type + "_d.bin";
	vec cm(0, 0, 0);
	fstream input(pclFile.c_str(), ios::in | ios::binary);

	if (!input.good())
	{
		cerr << "Could not read file: " << pclFile << endl;
		return point_cloud;
		exit(EXIT_FAILURE);
	}
	input.seekg(0, ios::beg);
	int i;
	for (i = 0; input.good() && !input.eof(); i++)
	{
		vec point;
		float intensity;
		input.read((char *)&point.x, 3 * sizeof(float));
		input.read((char *)&intensity, sizeof(float));
		cm += point;
		point_cloud.push_back(point);
	}
	input.close();

	//Read ground Truth CSV
	const string trim = FOLDER_NAME.substr(FOLDER_NAME.find("_") + 1);
	const string path = getBasePath() + "resources/extracted_las_files/" + FOLDER_NAME + "/" + FOLDER_NAME + "_" + trim + ".csv";
	ifstream myfile;
	myfile.open(path);
	vector<string> lines;
	bool keep = false;

	while (myfile.good())
	{
		string line;
		getline(myfile, line, ',');

		if (keep && truth.lines.size() <= 5)
		{
			lines.push_back(line);
		}
		if (line.substr(line.find('#') + 1) == subscene)
			keep = true;

		if (lines.size() == 4)
		{
			keep = false;
			lines[lines.size() - 1] = lines.back().substr(0, lines.back().find('P') - 1);
		}
	}

	myfile.close();

	///You can also work with the point of interest
	///for something changed remove comments
	cm /= point_cloud.size();

	if (type == "xyz")
	{

		for (vec &pt : point_cloud)
		{
			pt -= cm;
		}
		truth.pos = vec(atof(lines[0].c_str()), atof(lines[1].c_str()), atof(lines[2].c_str()));

		truth.label = lines[3];

		truth.pos -= cm;
	}

	return point_cloud;
}

std::vector<string> Mesh3DScene::load_folder_names()
{
	std::vector<std::string> str = {
		"0_5D4KVPBP",
		"10_5D4KVPXD",
		"11_5D4KVPYD",
		"12_5D4KVQ0P",
		"13_5D4KVQ9U",
		"14_5D4KVRER",
		"17_5D4KVT9X",
		"18_5D4KX2ZN",

		"20_5D4KX3EC",
		"21_5D4KX3LW",
		"22_5D4KX3PZ",
		"23_5D4KX3RR",
		"24_5D4KX3TQ",
		"25_5D4KX3VN",
		"26_5D4KX40Y",
		"27_5D4KX4HE",
		"28_5D4KX4QC",
		"29_5D4KX4ZE",
		"2_5D4KVPDO",
		"30_5D4KX56H",
		"31_5D4KX5G9",
		"32_5D4KX5NM",
		"33_5D4KX5WV",
		"34_5D4KX66R",
		"35_5D4KX6L3",
		"36_5D4KX6T5",
		"37_5D4KX76F",
		"38_5D4KX7FN",
		"39_5D4KX7IA",
		"3_5D4KVPFI",
		"40_5D4KX7KT",
		"41_5D4KX7RD",
		"42_5D4KX826",
		"43_5D4KX8IR",
		"44_5D4KX8UQ",
		"45_5D4KX8Y6",
		"46_5D4KX993",
		"47_5D4KX9N2",
		"48_5D4KX9SD",
		"49_5D4KX9SY",
		"4_5D4KVPG4",
		"50_5D4KX9ZE",
		"51_5D4KXA0G",
		"52_5D4KXAW7",
		"53_5D4KXB8D",
		"54_5D4KXBC8",
		"55_5D4KXBTC",
		"56_5D4L1GZR",
		"57_5D4L1IQ4",
		"58_5D4L1JIE",
		"59_5D4L1M3I",
		"5_5D4KVPIN",
		"60_5D4L1MGO",
		"61_5D4L1P8E",
		"62_5D4L1QP2",
		"63_5D4L1RDR",
		"64_5D4L1RW5",
		"65_5D4L1TDI",
		"66_5D4L1TH9",
		"67_5D4L1TX7",
		"68_5D4L1TYC",
		"69_5D4L1WHI",
		"6_5D4KVPJE",
		"70_5D4L1XPJ",
		"71_5D4L1Y38",
		"72_5D4L1YDX",
		"73_5D4L2BFI",
		"74_5D4L2C9B",
		"75_5D4L2DGW",
		"76_5D4L2DTM",
		"77_5D4L2FRJ",
		"78_5D4L2G9K",
		"79_5D4LHQUX",
		"7_5D4KVPNC",
		"8_5D4KVPT5",
		"9_5D4KVPX2"};
	return str;
}

void Mesh3DScene::center_point_clouds(vector<vec>& pt2016,vector<vec>& pt2020)
{
	vec cm;

	center_point_cloud(pt2020, cm);
	truth.pos -= cm;

	center_point_cloud(pt2016, cm);
}

void Mesh3DScene::reset()
{
	Scene::reset();

	initialize("0");

	//αρχικά flags
	show_bounding_box = true;
	show_task = 0;
	show_plane = false;
}

void Mesh3DScene::resize()
{
	static bool first_pass = true;

	if (first_pass)
	{
		first_pass = false;
	}
}

void Mesh3DScene::initialize(string input)
{
	msg("====================");

	echo(FOLDER_NAME);
	echo(SUBSCENE);
	msg("====================");

	/*m_model_original = m_model;*/

	vector<vec> rgb2016 = load_point_cloud("2016", input, "rgb");
	vector<vec> rgb2020 = load_point_cloud("2020", input, "rgb");

	vector<vec> xyz2016 = load_point_cloud("2016", input, "xyz");

	vector<vec> xyz2020 = load_point_cloud("2020", input, "xyz");

	center_point_clouds(xyz2016,xyz2020);

	//create bounding box
	box2016 = create_bounding_box(xyz2016);
	box2020 = create_bounding_box(xyz2020);


	//clear previous ptclouds
	pt2016.clear();
	pt2020.clear();
	//fill the Point Struct vectors
	for (int i = 0; i < xyz2016.size(); i++) {
		
		pt2016.push_back(Point(xyz2016[i], rgb2016[i]));

	}
	for (int i = 0; i < xyz2020.size(); i++) {

		pt2020.push_back(Point(xyz2020[i], rgb2020[i]));

	}


}

void Mesh3DScene::change_scene()
{
	pt2016.clear();
	
	pt2020.clear();
	

	string input;
	cout << "Scene Folder : ";
	cin >> input;
	FOLDER_NAME = input;
	reset();
}

vvr::Box3D Mesh3DScene::create_bounding_box(std::vector<vec> pts)
{
	double max_x = -inf, max_y = -inf, max_z = -inf;
	double min_x = inf, min_y = inf, min_z = inf;

	for (const vec &pt : pts)
	{
		if (pt.x > max_x)
			max_x = pt.x;
		if (pt.y > max_y)
			max_y = pt.y;
		if (pt.z > max_z)
			max_z = pt.z;

		if (pt.x < min_x)
			min_x = pt.x;
		if (pt.y < min_y)
			min_y = pt.y;
		if (pt.z < min_z)
			min_z = pt.z;
	}
	return vvr::Box3D(min_x, min_y, min_z, max_x, max_y, max_z, vvr::Colour::black);
}

void Mesh3DScene::arrowEvent(ArrowDir dir, int modif)
{

	if (dir == RIGHT)
	{
		show_task = 0;
		string newsub = to_string(stoi(SUBSCENE) + 1);
		initialize(newsub);
	}
	if (dir == LEFT)
	{
		show_task = 0;
		string newsub = to_string(stoi(SUBSCENE) - 1);
		initialize(newsub);
	}
	if (dir == UP)
	{
		show_task = 0;
		folder_index++;
		if (folder_index > FOLDERS.size() - 1)
			folder_index = 0;
		FOLDER_NAME = FOLDERS[folder_index];

		reset();
	}
	if (dir == DOWN)
	{
		show_task = 0;
		folder_index--;
		if (folder_index < 0)
		{
			folder_index = 0;
		}

		FOLDER_NAME = FOLDERS[folder_index];

		reset();
	}
	/*math::vec n = m_plane.normal;
	if (dir == UP) m_plane_d += 1;
	if (dir == DOWN) m_plane_d -= 1;
	else if (dir == LEFT) n = math::float3x3::RotateY(DegToRad(1)).Transform(n);
	else if (dir == RIGHT) n = math::float3x3::RotateY(DegToRad(-1)).Transform(n);
	m_plane = Plane(n.Normalized(), m_plane_d);

	if (SPLIT_INSTEAD_OF_INTERSECT == false) {
		m_intersections.clear();
	}
	else {
		m_model = Mesh(m_model_original);
	}*/
}

void Mesh3DScene::keyEvent(unsigned char key, bool up, int modif)
{
	//Scene::keyEvent(key, up, modif);

	key = tolower(key);



	switch (key)
	{

	case 'b':
		show_bounding_box = !show_bounding_box;
		break;

	//change subscene
	case '1':
		task1(modif);
		break;
	case '2':
		task2_clustering(1);
		break;
	case '3':
		task3_background_removal();
		break;
	case '4':
		task4_registration();
		break;
	case '5':
		task5_ColorComp();
		break;
	case '6':
		task6_Classify();
		break;


	//change Scene
	case 'c':
		change_scene();
		break;

	case 'r':
		reset();
		break;
	case 'p':
		show_plane = !show_plane;
		break;
	}
}

void Mesh3DScene::draw()
{

	//Αρχική κατάσταση εμφανίζονται οι δύο σκηνές 2016 και 2020
	// μετατοπισμένες για να μην πέφτει η μία πάνω στην άλλη
	if (show_task == 0 || show_task == 1)
	{

		draw_point_cloud(pt2016, -5);
		draw_point_cloud(pt2020, +5);
	}

	//εμφανίζει τα bounding boxes μετατοπισμένα.
	if (show_bounding_box )
	{
		draw_bounding_box(box2016, -5, vvr::Colour::green);
		draw_bounding_box(box2020, +5, vvr::Colour::blue);
	}

	//Todo

	if (show_task == 2)
	{

		draw_point_cloud(pt2020, labels2020, 5);
		draw_point_cloud(pt2016, labels2016, -5);
	}
	if (show_task == 3)
	{
		draw_point_cloud(pt2016, -5);
		draw_point_cloud(pt2020, +5);
	}

	if (show_task == 4) {
		draw_point_cloud(heatmapPtc2016, -5);
		draw_point_cloud(heatmapPtc2020, 5);

	}
	if (show_task == 5) {
		double a = 5;
	}

	//vvr::Point3D(truth.pos.x + 5, truth.pos.y, truth.pos.z, vvr::Colour::black).draw();
	//vvr::Point3D(truth.pos.x - 5, truth.pos.y, truth.pos.z, vvr::Colour::black).draw();

	if (show_plane)
	{
		vvr::Colour colPlane(0x41, 0x14, 0xB3);
		float u = 20, v = 20;
		math::vec p0(m_plane.Point(-u, -v, math::vec(0, 0, 0)));
		math::vec p1(m_plane.Point(-u, v, math::vec(0, 0, 0)));
		math::vec p2(m_plane.Point(u, -v, math::vec(0, 0, 0)));
		math::vec p3(m_plane.Point(u, v, math::vec(0, 0, 0)));
		vvr::Triangle3D(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, colPlane).draw();
		vvr::Triangle3D(p2.x, p2.y, p2.z, p1.x, p1.y, p1.z, p3.x, p3.y, p3.z, colPlane).draw();
	}
}
void Mesh3DScene::draw_point_cloud(const vector<Point> &pts, int offset)
{
	for (int i = 0; i < pts.size(); i++)
	{
		vec ptcol = pts[i].col;
		vec pos = pts[i].pos;

		vvr::Colour col((int)ptcol.x, (int)ptcol.y, (int)ptcol.z);
		Point3D(pos.x + offset, pos.y, pos.z, col).draw();
	}
}

void Mesh3DScene::draw_point_cloud(const std::vector<Point> &ptc, const std::vector<int> cluster_labels, const int offset)
{
	static const vvr::Colour Pallete[7] = {
		vvr::Colour::red, vvr::Colour::green, vvr::Colour::blue, vvr::Colour::orange,
		vvr::Colour::yellow, vvr::Colour::cyan, vvr::Colour::magenta};

	for (int i = 0; i < cluster_labels.size(); i++)
	{
		int label = cluster_labels[i];

		if (label == -2)
		{
			Point3D(ptc[i].pos.x + offset, ptc[i].pos.y, ptc[i].pos.z, vvr::Colour::white).draw();
		}
		else
		{
			vvr::Colour col = Pallete[label & 6];
			Point3D(ptc[i].pos.x + offset, ptc[i].pos.y, ptc[i].pos.z, col).draw();
		}
	}
}

void Mesh3DScene::draw_bounding_box(vvr::Box3D box, int offset, vvr::Colour col)
{
	box.setTransparency(1);
	box.setColour(col);
	box.x1 += offset;
	box.x2 += offset;
	box.draw();
}

void Mesh3DScene::task1(int modif)
{
	show_task = 1;
	if (modif == 0)
	{
		msg("-----Threshold Ground Removal-----");
		groundRemoval(pt2020);
		groundRemoval(pt2016);
	}

	if (modif == 4)
	{
		msg("-----RANSAC Ground Removal-----");
		msg("{p} to display plane ");
		m_plane = planeEstimation(pt2020, box2020);
		m_plane = planeEstimation(pt2016, box2016);
	}
}
void Mesh3DScene::task2_clustering(int modif)
{
	show_task = 2;
	if (modif == 1)

	{
		msg("-----Clustering-----");
		Clustering_DBSCAN(pt2020, labels2020, 10, 0.2);
		Clustering_DBSCAN(pt2016, labels2016, 10, 0.2);
	}
}
void Mesh3DScene::task3_background_removal()
{
	show_task = 3;
	msg("-----Background Removal-----");
	background_removal(pt2020, labels2020, truth.pos);
	background_removal(pt2016, labels2016, truth.pos);
}

void Mesh3DScene::task4_registration()
{
	show_task = 4;
	msg("-----Heatmap-----");
	generate_heatmap(pt2016, pt2020,heatmapPtc2016,heatmapPtc2020,maxDist);
	echo(maxDist);

}
void Mesh3DScene::task5_ColorComp()
{
	msg("-----Color comparison chi-square -----");
	show_task = 1;
	hist2016 = get_histogram(pt2016, 360);
	hist2020 = get_histogram(pt2020, 360);

	
	std::ofstream myfile;
	myfile.open("hist2016.csv");
	for (int i = 0; i < hist2016.size(); i++) {
		myfile << hist2016[i] << endl;
	}
	myfile.close();
	myfile.open("hist2020.csv");
	for (int i = 0; i < hist2020.size(); i++) {
		myfile << hist2020[i] << endl;
	}
	myfile.close();
	double chival= chi_square_dist(hist2016, hist2020);
	echo(chival);
	
}
void Mesh3DScene::task6_Classify()
{
	msg("------CLASSIFICATION-------");
	double size1 = pt2016.size();
	double size2 = pt2020.size();

	double sizeChange = abs(size1 - size2) / max(size1, size2);
	
	if (sizeChange > 0.8) {
		if (size1 > size2) {
			msg("Removed");
		}
		else {
			msg("Added");
		}
	}
	else if (maxDist > 0.35 ||chival>0.5) {
		msg("change");
	}
	else if (chival > 0.25) {
		msg("color_change");
	}
	else{
		msg("no_change");
	}
	
}
int main(int argc, char *argv[])
{
	try
	{
		return vvr::mainLoop(argc, argv, new Mesh3DScene);
	}
	catch (std::string exc)
	{
		cerr << exc << endl;
		return 1;
	}
	catch (...)
	{
		cerr << "Unknown exception" << endl;
		return 1;
	}
}