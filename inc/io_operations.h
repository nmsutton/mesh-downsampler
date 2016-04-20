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

struct input_file import_data(string in_filename);

void export_config_file(string temp_downs_output, downsampled_mesh downs_mesh, string config_gen_program, string python_path, string outfile);

#endif /* IO_OPERATIONS_H_ */
