/*
 * create_downsample.h
 *
 *  Created on: Apr 16, 2016
 *      Author: nmsutton
 */

#ifndef CREATE_DOWNSAMPLE_H_
#define CREATE_DOWNSAMPLE_H_

#include <math.h>       /* sqrt */
#include <stdio.h>
//#include "init_downs_data.h"

using namespace std;

double neighbor(int map_i, int u, double bmu_dist, int time_step);

double learning_alpha(double time_step);

double weight_update(double orig_coord, double weight, double bmu_dist, double time_step, int map_i, int u);

void print_weights(W W);

void downsample_mesh(int ORIG_MESH_VERTS, int DOWNS_MESH_VERTS, input_file &orig_data, downsampled_mesh &downs_mesh, som_data &som);

#endif /* CREATE_DOWNSAMPLE_H_ */
