/*
 * io_operations.h
 *
 *  Created on: Apr 14, 2016
 *      Author: nmsutton
 */

#ifndef IO_OPERATIONS_H_
#define IO_OPERATIONS_H_

#include <string>
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

struct input_file import_data(string in_filename);

int test();

#endif /* IO_OPERATIONS_H_ */
