/*
 * create_downsample.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: nmsutton
 */

#include <math.h>       /* sqrt */
#include <stdio.h>
#include <iostream>
#include <vector>
#include "find_distances.h"
//#include "init_downs_data.h"

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

struct W {
	vector<double> x;
	vector<double> y;
	vector<double> z;
};

double neighbor(int map_i, int u, double bmu_dist, int time_step, downsampled_mesh &downs_mesh) {
	/*
	 * Neighbor function determines the area of neighboring verticies to the bmu
	 * that are able to effect weights
	 *
	 * The neigborhood reduces its area over timesteps
	 *
	 * Max score is 1;
	 *
	 * neighbor_score = 1 for the bmu.  else score is reduced by distance
	 *
	 * TODO: A gaussian formula could be used for region calculation if wanted.
	 */
	double neighbor_score = 0.0;
	double neighbor_region = 9 / pow((double) (time_step + 1),.333); // +1 to avoid division by 0
	double v_dist = find_euclidean_dist(downs_mesh.x[map_i], downs_mesh.y[map_i], downs_mesh.z[map_i], downs_mesh.x[u], downs_mesh.y[u], downs_mesh.z[u]);

	if (v_dist <= neighbor_region) {
		neighbor_score = (1 - (v_dist / neighbor_region));
	}
	//cout<<" v_d "<<v_dist<<" "<<" n_s "<<neighbor_score<<" ";//<<" v_x "<<v_x<<" v_y "<<v_y<<" v_z "<<v_z;

	return neighbor_score;
}

double learning_alpha(double time_step) {
	/*
	 * α(s) is a monotonically decreasing learning coefficient
	 */
	double alpha = 0.75;

	alpha = alpha / pow((time_step + 1),.333); // +1 to avoid division by 0
	//cout<<" ^alpha^ "<<alpha<<" time_step "<<time_step<<" "<<pow((time_step + 1),.333)<<" "<<alpha / pow((time_step + 1),1/3);
	return alpha;
}

double weight_update(double orig_coord, double weight, double bmu_dist, double time_step, int map_i, int u, downsampled_mesh &downs_mesh) {
	/*
	 * SOM formula:
	 * Wv(s + 1) = Wv(s) + Θ(u, v, s) α(s)(D(t) - Wv(s))
	 */
	double neighbor_score = neighbor(map_i, u, bmu_dist, time_step, downs_mesh);
	double alpha = learning_alpha(time_step);

	/*if (time_step < 4) {
		cout<<"~+~ w "<<weight<<" n s "<<neighbor_score<<" a "<<alpha<<" o_c "<<orig_coord<<" m_i "<<map_i<<" u "<<u;
	}*/

	weight = weight + neighbor_score * alpha * (orig_coord - weight);

	/*if (time_step < 4) {
		cout<<" new w "<<weight;
	}*/

	return weight;
}

void print_weights(W W, int DOWNS_MESH_VERTS) {
	cout<<endl;
	for (int i = 0; i < DOWNS_MESH_VERTS; i++) {
		cout<<W.x[i]<<"\t"<<W.y[i]<<"\t"<<W.z[i]<<endl;
	}
}

void downsample_mesh(int ORIG_MESH_VERTS, int DOWNS_MESH_VERTS, input_file &orig_data, downsampled_mesh &downs_mesh) {
	/*
	Use self organizing maps to cluster downsampled verticies

	from: https://en.wikipedia.org/wiki/Self-organizing_map

	s = the current iteration, a.k.a. the 'step' of the computations
	L = the iteration limit
	t = the index of the target input data vector in the input data set D
	IV = number of input vector verticies
	T = number of verticies in input vector
	D(t) = a target input data vector
	v = the index of the node in the map (map being the vectors to map the input vector onto)
	W_v = the current weight vector of node v
	u = the index of the best matching unit (BMU) in the map for D(t) (the input vector)
	bmu_dist = Θ(u, v, s) = a restraint due to distance from BMU, usually called the neighborhood function
	the neighborhood function gives the distance between the neuron u and the neuron v in step s
	α(s) = a learning restraint due to iteration progress.

	SOM formula:
	Wv(s + 1) = Wv(s) + Θ(u, v, s) α(s)(D(t) - Wv(s))

	TODO: Stop running algorithm once it reaches a convergence level instead of just the maximum L

	W is initialized as 1s for now, if wanted later work could address "The weights of the neurons
	are initialized either to small random values or sampled evenly from the subspace spanned by
	the two largest principal component eigenvectors."

	first BMU is found for each vertex
	second bmu_dist is used to update SOM weights

	BMU is found with a loop before I can be used in the neighborhood function
	 */

	int s = 0, t = 0, v = 0, u = 0;
	int L = 10;
	int IV = ORIG_MESH_VERTS;
	int T = DOWNS_MESH_VERTS;
	//double alpha = 1.0;
	double new_dist = 0;
	double bmu_dist = 0;

	W W;

	// initialize
	for (int map_i = 0; map_i < T; map_i++) {W.x.push_back(1); W.y.push_back(1); W.z.push_back(1);}

	// train
	for (int L_i = 0; L_i < L; L_i++) {
		//cout<<endl<<"SOM iteration "<<L_i;
		for (int in_i = 0; in_i < IV; in_i++) {
			u = 0;
			new_dist = 0;
			bmu_dist = 100000;//find_euclidean_dist(orig_mesh.x[0], orig_mesh.y[0], orig_mesh.z[0], downs_mesh.x[0], downs_mesh.y[0], downs_mesh.z[0]); // reinitialize
			//cout<<endl<<"bmu find o_m index "<<in_i;
			for (int map_i = 0; map_i < T; map_i++) {
				new_dist = find_euclidean_dist(orig_data.x[in_i], orig_data.y[in_i], orig_data.z[in_i], downs_mesh.x[map_i], downs_mesh.y[map_i], downs_mesh.z[map_i]);
				if (new_dist < bmu_dist) {bmu_dist = new_dist; u = map_i;}
				//cout<<endl<<"\tu"<<u<<" o_x "<<orig_mesh.x[in_i]<<"\to_y "<<orig_mesh.y[in_i]<<"\to_z "<< orig_mesh.z[in_i]<<"\td_x "<< downs_mesh.x[map_i]<<"\td_y "<< downs_mesh.y[map_i]<<"\td_z "<< downs_mesh.z[map_i]<<"\tbmu_dist "<<bmu_dist<<" new_dist "<<new_dist;
			}
			//cout<<endl<<"\tu\t"<<u<<"\tbmu_dist\t"<<bmu_dist;
			for (int map_i = 0; map_i < T; map_i++) {
				//cout<<" *x*";
				W.x[map_i] = weight_update(orig_data.x[u], W.x[map_i], bmu_dist, L_i, map_i, u, downs_mesh);
				//cout<<" *y* ";
				W.y[map_i] = weight_update(orig_data.y[u], W.y[map_i], bmu_dist, L_i, map_i, u, downs_mesh);
				//cout<<" *z* ";
				W.z[map_i] = weight_update(orig_data.z[u], W.z[map_i], bmu_dist, L_i, map_i, u, downs_mesh);
			}
		}
		//print_weights(W);
	}

	// apply
	/*
	 * directly saving weight values as new mesh vertex points instead of multiplying points by weights,
	 * this appeared to create good results and will be used unless a better way is found.  SOM algorithm
	 * didn't specify in particular another approach should be done.
	 */
	for (int i = 0; i < T; i++) {
		downs_mesh.x[i] = W.x[i];
		downs_mesh.y[i] = W.y[i];//W.y[map_i] = 1;
		downs_mesh.z[i] = W.z[i];//W.z[map_i] = 1;
	}
}


