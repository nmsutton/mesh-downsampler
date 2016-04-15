/*
 * io_operations.h
 *
 *  Created on: Apr 14, 2016
 *      Author: nmsutton
 */

#ifndef IO_OPERATIONS_H_
#define IO_OPERATIONS_H_

#include <string>

using namespace std;

struct input_file {
	double bounding_box_vert[];
	double pos_x[], pos_y[], pos_z[], pos_t[];
	double vel_x[], vel_y[], vel_z[], vel_t[];
	double ela_p_j[], ela_r_ij[], ela_val1[], ela_val2[];
	double mem_val1[], mem_val2[], mem_val3[];
	double partMemInd[];
};

input_file import_data(string in_filename);

#endif /* IO_OPERATIONS_H_ */
