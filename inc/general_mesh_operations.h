/*
 * general_mesh_operations.h
 *
 *  Created on: Apr 16, 2016
 *      Author: nmsutton
 */

#ifndef GENERAL_MESH_OPERATIONS_H_
#define GENERAL_MESH_OPERATIONS_H_

#include <string>
#include "find_distances.h"

using namespace std;

void create_mesh(int mesh_x, int mesh_y, int mesh_z, string type);

void copy_mesh();

void print_data_results(string section);

#endif /* GENERAL_MESH_OPERATIONS_H_ */
