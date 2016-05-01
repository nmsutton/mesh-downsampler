/*
 * core_objects.h
 *
 *  Created on: Apr 16, 2016
 *      Author: nmsutton
 */

#ifndef CORE_OBJECTS_H_
#define CORE_OBJECTS_H_

#include <vector>

using namespace std;

string temp_downs_output = "./temp_output/temp_downs";

struct input_file {
	vector<double> bounding_box_vert;
	vector<double> x, y, z, t;
	vector<double> vel_x, vel_y, vel_z, vel_t;
	vector<double> ela_p_j, ela_r_ij, ela_val1, ela_val2;
	vector<double> mem_val1, mem_val2, mem_val3;
	vector<double> partMemInd;
};

struct downsampled_mesh {
	vector<double> bounding_box_vert;
	vector<double> x, y, z, t;
	vector<double> vel_x, vel_y, vel_z, vel_t;
	vector<double> ela_p_j, ela_r_ij, ela_val1, ela_val2;
	vector<double> mem_val1, mem_val2, mem_val3;
	vector<double> partMemInd;
};

struct W {
	vector<double> x;
	vector<double> y;
	vector<double> z;
};

struct som_data {
	bool active;
	int steps;
	double neigh;
	double learn;
};

struct physics_sects {
	vector<double> x1, x2, y1, y2, z1, z2;
	vector<double> h_scalar, p_type_range_min, p_type_range_max;
	string file;
};

struct particle_range_sections{
	/*
	 * Ranges to have adjusted physics values
	 */

	vector<int> start_range, end_range;
	vector<double> mod_r0, p_type_range_min, p_type_range_max;
	string output_filename;
};

downsampled_mesh downs_mesh;
vector<downsampled_mesh> downs_sects;
input_file orig_data;
input_file o_mesh_sorted;
som_data som;
physics_sects phys_sects;
particle_range_sections particle_ranges;
vector<double> particle_physics_mods;
string phys_mod_filename;

int ORIG_MESH_VERTS = -1;
int DOWNS_MESH_VERTS = -1;

#endif /* CORE_OBJECTS_H_ */
