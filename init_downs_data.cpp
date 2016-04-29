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

/*struct downs_conf_sects {
	vector<downsampled_mesh> downs_mesh;
};*/

struct physics_sects {
	vector<double> x1, x2, y1, y2, z1, z2;
	vector<double> h_scalar, p_type_range_min, p_type_range_max;
	string file;
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

void init_downs_verts(double s, int ORIG_MESH_VERTS, int DOWNS_MESH_VERTS, input_file &orig_data, physics_sects &phys_sects, vector<downsampled_mesh> &downs_sects) {
	/*
	 * Initialization of downs verts:
	 * s = a parameter that does a scalar multiplication on window_size to increase the size if wanted
	 */
	double window_size = s * ((double) ORIG_MESH_VERTS/(double) DOWNS_MESH_VERTS);

	//sort2();

	double curr_x, curr_y, curr_z, curr_t;
	int offset_index = 0;

	// initialize.  NOTE: the struct is passed by reference and not by pointer here right?
	for (int sect_i = 0; sect_i < phys_sects.h_scalar.size(); sect_i++) {
		downsampled_mesh new_downs_mesh;
		downs_sects.push_back(new_downs_mesh);
	}

	// bounding box
	for (int sect_i = 0; sect_i < phys_sects.h_scalar.size(); sect_i++) {
		for (int bb_i = 0; bb_i < orig_data.bounding_box_vert.size(); bb_i++) {
			downs_sects[sect_i].bounding_box_vert.push_back(orig_data.bounding_box_vert[bb_i]);
		}
	}

	// sections
	for (int map_i = 0; map_i < DOWNS_MESH_VERTS; map_i++) {
		offset_index = ceil((double) map_i*window_size);

		for (int sect_i = 0; sect_i < phys_sects.h_scalar.size(); sect_i++) {
			curr_x = orig_data.x[offset_index];
			curr_y = orig_data.y[offset_index];
			curr_z = orig_data.z[offset_index];
			curr_t = orig_data.t[offset_index];
			if (curr_x >= phys_sects.x1[sect_i] & curr_x <= phys_sects.x2[sect_i] &
					curr_y >= phys_sects.y1[sect_i] & curr_y <= phys_sects.y2[sect_i] &
					curr_z >= phys_sects.z1[sect_i] & curr_z <= phys_sects.z2[sect_i] &
					curr_t >= phys_sects.p_type_range_min[sect_i] & curr_t <= phys_sects.p_type_range_max[sect_i]) {

				downs_sects[sect_i].x.push_back(orig_data.x[offset_index]);
				downs_sects[sect_i].y.push_back(orig_data.y[offset_index]);
				downs_sects[sect_i].z.push_back(orig_data.z[offset_index]);
				downs_sects[sect_i].t.push_back(orig_data.t[offset_index]);
			}
		}
	}

}


