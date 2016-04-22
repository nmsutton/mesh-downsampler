/*
 * init_downs_data.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: nmsutton
 */

#include <set>
#include <math.h>       /* sqrt */
#include <stdio.h>
#include <iostream>
#include <vector>
#include "find_distances.h"

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

using namespace std;

void sort2(int ORIG_MESH_VERTS, input_file &o_mesh_sorted) {
	/*
	 * To avoid duplicate entries indices_processed.find(in_i2) != indices_processed.end() ensures unique new
	 * indices are searched.
	 */
	double new_dist = 100000000;
	double closest_dist = 100000000;
	double swap_x = 0, swap_y = 0, swap_z = 0;
	set<int> indices_processed;

	for (int in_i = 0; in_i < ORIG_MESH_VERTS; in_i++) {
		new_dist = 100000000;
		closest_dist = 100000000;
		swap_x = 0, swap_y = 0, swap_z = 0;
		for (int in_i2 = 0; in_i2 < ORIG_MESH_VERTS; in_i2++) {
			// add unique vert finding code
			new_dist = find_euclidean_dist(o_mesh_sorted.x[in_i], o_mesh_sorted.y[in_i], o_mesh_sorted.z[in_i],
					o_mesh_sorted.x[in_i2], o_mesh_sorted.y[in_i2], o_mesh_sorted.z[in_i2]);
			if (new_dist < closest_dist & indices_processed.find(in_i2) != indices_processed.end()) {
				swap_x       = o_mesh_sorted.x[in_i];
				swap_y       = o_mesh_sorted.y[in_i];
				swap_z       = o_mesh_sorted.z[in_i];
				o_mesh_sorted.x[in_i]   = o_mesh_sorted.x[in_i2];
				o_mesh_sorted.y[in_i]   = o_mesh_sorted.y[in_i2];
				o_mesh_sorted.z[in_i]   = o_mesh_sorted.z[in_i2];
				o_mesh_sorted.x[in_i2] = swap_x;
				o_mesh_sorted.y[in_i2] = swap_y;
				o_mesh_sorted.z[in_i2] = swap_z;

				indices_processed.insert(in_i);
				indices_processed.insert(in_i2);

				closest_dist = new_dist;
			}
		}
	}
}

void find_init_positions2(double window_size, int DOWNS_MESH_VERTS, input_file &orig_data, downsampled_mesh &downs_mesh) {
	int offset_index = 0;

	for (int bb_i = 0; bb_i < orig_data.bounding_box_vert.size(); bb_i++) {
		downs_mesh.bounding_box_vert.push_back(orig_data.bounding_box_vert[bb_i]);
	}

	for (int map_i = 0; map_i < DOWNS_MESH_VERTS; map_i++) {
		offset_index = ceil((double) map_i*window_size);

		downs_mesh.x.push_back(orig_data.x[offset_index]);
		downs_mesh.y.push_back(orig_data.y[offset_index]);
		downs_mesh.z.push_back(orig_data.z[offset_index]);
		downs_mesh.t.push_back(orig_data.t[offset_index]);
	}
}

void init_downs_verts(double s, int ORIG_MESH_VERTS, int DOWNS_MESH_VERTS, input_file &orig_data, downsampled_mesh &downs_mesh) {
	/*
	 * Initialization of downs verts:
	 * s = a parameter that does a scalar multiplication on window_size to increase the size if wanted
	 */
	double window_size = s * ((double) ORIG_MESH_VERTS/(double) DOWNS_MESH_VERTS);

	//sort2();

	find_init_positions2(window_size, DOWNS_MESH_VERTS, orig_data, downs_mesh);

}


