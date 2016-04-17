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

downsampled_mesh downs_mesh;
input_file orig_data;
input_file o_mesh_sorted;

int ORIG_MESH_VERTS = -1;
int DOWNS_MESH_VERTS = -1;

struct W {
	vector<double> x;
	vector<double> y;
	vector<double> z;
};

#endif /* CORE_OBJECTS_H_ */
