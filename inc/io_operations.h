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

struct physics_sects import_phys_sects(string phys_sects_file);

string int_to_str(int i);

void write_config_file(string out_filename, vector<downsampled_mesh> &downs_sects, int section);

void export_config_files(string temp_downs_output, physics_sects &phys_sects, vector<downsampled_mesh> &downs_sects, string config_gen_path, string current_path, string outfile);

#endif /* IO_OPERATIONS_H_ */
