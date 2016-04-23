/*
 * init_downs_data.h
 *
 *  Created on: Apr 16, 2016
 *      Author: nmsutton
 */

#ifndef INIT_DOWNS_DATA_H_
#define INIT_DOWNS_DATA_H_

#include <set>
#include <math.h>       /* sqrt */
#include <stdio.h>
#include <iostream>
//#include "general_mesh_operations.h"

using namespace std;

void sort2();

void find_init_positions2(double window_size, int DOWNS_MESH_VERTS, input_file &orig_data, physics_sects &phys_sects, downs_conf_sects &downs_sects);

void init_downs_verts(double s, int ORIG_MESH_VERTS, int DOWNS_MESH_VERTS, input_file &orig_data, physics_sects &phys_sects, downs_conf_sects &downs_sects);

#endif /* INIT_DOWNS_DATA_H_ */
