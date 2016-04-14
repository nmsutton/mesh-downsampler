/*
 * main.cpp
 *
 *  Created on: Apr 6, 2016
 *      Author: nmsutton
 */

#include <stdio.h>
#include <iostream>
#include <math.h>       /* sqrt */
#include <sstream>		/* ostringstream */
#include <set>

using namespace std;

struct original_mesh {
	double x[12]; // = {1,2,3};
	double y[12]; // = {4,5,6};
	double z[12]; // = {3,2,1};
};

struct downsampled_mesh {
	double x[8]; // = {1,1};
	double y[8]; // = {1,1};
	double z[8]; // = {1,1};
};

original_mesh orig_mesh;
original_mesh o_mesh_sorted;
downsampled_mesh downs_mesh;

const int ORIG_MESH_VERTS = sizeof(orig_mesh.x)/sizeof(orig_mesh.x[0]);  // from http://stackoverflow.com/questions/2037736/finding-size-of-int-array
const int DOWNS_MESH_VERTS = sizeof(downs_mesh.x)/sizeof(downs_mesh.x[0]);

struct W {
	double x[DOWNS_MESH_VERTS];
	double y[DOWNS_MESH_VERTS];
	double z[DOWNS_MESH_VERTS];
};

double find_euclidean_dist(double x_1, double y_1, double z_1, double x_2, double y_2, double z_2) {
	/*
	 * https://en.wikipedia.org/wiki/Euclidean_distance
	 */
	double distance = sqrt(pow((x_2-x_1),2)+pow((y_2-y_1),2)+pow((z_2-z_1),2));
	return distance;
}

double neighbor(int map_i, int u, double bmu_dist, int time_step) {
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

void sort(double mesh[], string axis) {
	/*
	 * for convenience using the simple bubble sort
	 *
	 * Reference:
	 * code from http://www.cplusplus.com/forum/general/127295/
	 */
	double swap = 0;

	for (int in_i = 0; in_i < ORIG_MESH_VERTS; in_i++) {
		for (int in_i2 = 0; in_i2 < ORIG_MESH_VERTS; in_i2++) {
			if (mesh[in_i2] > mesh[in_i]) /* For decreasing order use < */
			{
				swap       = mesh[in_i];
				mesh[in_i]   = mesh[in_i2];
				mesh[in_i2] = swap;
			}
		}
	}

	for (int i = 0; i < ORIG_MESH_VERTS; i++) {
		if (axis == "x") {o_mesh_sorted.x[i] = mesh[i];}
		else if (axis == "y") {o_mesh_sorted.y[i] = mesh[i];}
		else if (axis == "z") {o_mesh_sorted.z[i] = mesh[i];}
 	}
}

void sort2() {
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

void find_init_positions(double mesh[], string axis, double window_size) {
	/*
	 * Slide windows of vertices through each axis of the original mesh.  For each window take the avg value of the positions
	 * of the vertices in the axis.  Store the avg values in each downs vert.
	 *
	 * TODO: make window slide in a more centered position amongst orig_mesh vertices.  I am guessing this is a low priority
	 * todo, the resulting dows_mesh could be more centered compared to the orig_mesh this way but just having windows starting
	 * at orig_mesh indices sequentially instead of shifted to center seems like it can be fine.
	 */
	double downs_pos = 0;
	int win_i = 0;
	double last_i = -1;
	int offset_index = 0;

	for (int map_i = 0; map_i < DOWNS_MESH_VERTS; map_i++) {
		downs_pos = 0;
		last_i = -1;
		cout<<endl<<"map_i "<<map_i<<" window_size: "<<window_size;
		for (win_i = 0; win_i < window_size; win_i++) {
			offset_index = ceil(((double) map_i*window_size)+(double) win_i);
			//cout<<endl<<"offset_index "<<offset_index;
			//offset_index = (map_i)+win_i;
			if (offset_index < ORIG_MESH_VERTS) {
				downs_pos += mesh[offset_index];
				//cout<<"mesh[offset_index] "<<mesh[offset_index];
			}
			else {
				last_i = 1.0;
			}
		}
		if (last_i == -1) {last_i = (double) win_i;};
		//cout<<endl<<"lasti"<<last_i;

		downs_pos = downs_pos / last_i;

		if (axis == "x") {downs_mesh.x[map_i] = downs_pos;}
		else if (axis == "y") {downs_mesh.y[map_i] = downs_pos;}
		else if (axis == "z") {downs_mesh.z[map_i] = downs_pos;}
	}
}

void find_init_positions2(double window_size) {
	int offset_index = 0;

	cout<<endl<<"start";
	for (int map_i = 0; map_i < DOWNS_MESH_VERTS; map_i++) {
		offset_index = ceil((double) map_i*window_size);
		cout<<endl<<offset_index;

		/*downs_mesh.x[offset_index] = o_mesh_sorted.x[offset_index];
		downs_mesh.y[offset_index] = o_mesh_sorted.y[offset_index];
		downs_mesh.z[offset_index] = o_mesh_sorted.z[offset_index];*/

		downs_mesh.x[map_i] = orig_mesh.x[offset_index];
		downs_mesh.y[map_i] = orig_mesh.y[offset_index];
		downs_mesh.z[map_i] = orig_mesh.z[offset_index];

		/*last_i = -1;
		//cout<<endl<<"map_i "<<map_i<<" window_size: "<<window_size;
		for (win_i = 0; win_i < window_size; win_i++) {
			offset_index = ceil(((double) map_i*window_size)+(double) win_i);
			//cout<<endl<<"offset_index "<<offset_index;
			//offset_index = (map_i)+win_i;
			if (offset_index < ORIG_MESH_VERTS) {
				downs_pos += mesh[offset_index];
				//cout<<"mesh[offset_index] "<<mesh[offset_index];
			}
			else {
				last_i = 1.0;
			}
		}*/
		//if (last_i == -1) {last_i = (double) win_i;};
		//cout<<endl<<"lasti"<<last_i;

		//downs_pos = downs_pos / last_i;

		/*if (axis == "x") {downs_mesh.x[map_i] = downs_pos;}
		else if (axis == "y") {downs_mesh.y[map_i] = downs_pos;}
		else if (axis == "z") {downs_mesh.z[map_i] = downs_pos;}*/
	}
}

void init_downs_verts(double s) {
	/*
	 * Initialization of downs verts:
	 * s = a parameter that does a scalar multiplication on window_size to increase the size if wanted
	 */
	double window_size = s * ((double) ORIG_MESH_VERTS/(double) DOWNS_MESH_VERTS);

	/*sort(orig_mesh.x, "x");
	sort(orig_mesh.y, "y");
	sort(orig_mesh.z, "z");*/
	sort2();

	find_init_positions2(window_size);

	/*find_init_positions(o_mesh_sorted.x, "x", window_size);
	find_init_positions(o_mesh_sorted.y, "y", window_size);
	find_init_positions(o_mesh_sorted.z, "z", window_size);*/

}

double weight_update(double orig_coord, double weight, double bmu_dist, double time_step, int map_i, int u) {
	/*
	 * SOM formula:
	 * Wv(s + 1) = Wv(s) + Θ(u, v, s) α(s)(D(t) - Wv(s))
	 */
	double neighbor_score = neighbor(map_i, u, bmu_dist, time_step);
	double alpha = learning_alpha(time_step);

	if (time_step < 4) {
		cout<<"~+~ w "<<weight<<" n s "<<neighbor_score<<" a "<<alpha<<" o_c "<<orig_coord<<" m_i "<<map_i<<" u "<<u;
	}

	weight = weight + neighbor_score * alpha * (orig_coord - weight);

	if (time_step < 4) {
		cout<<" new w "<<weight;
	}

	return weight;
}

void print_weights(W W) {
	cout<<endl;
	for (int i = 0; i < DOWNS_MESH_VERTS; i++) {
		cout<<W.x[i]<<"\t"<<W.y[i]<<"\t"<<W.z[i]<<endl;
	}
}

void downsample_mesh2(double downsample_percent) {
	/*
	 * New algorithm for downsampling
	 *
	 * (cluster_bin_size - 1) is used because original vertex that is compared to the others adds (+1) to create cluster_bin_size size
	 */
	int s = 0, t = 0, v = 0, u = 0;
	int L = 10;
	int IV = ORIG_MESH_VERTS;
	int T = DOWNS_MESH_VERTS;
	//double alpha = 1.0;
	double new_dist = 0;
	double bmu_dist = 0;

	const int MAX_BIN_SIZE = 10000;
	struct bmus_verts {
		int i[MAX_BIN_SIZE]; // hardcoded at 10000 bin size limit but can make dynamic later
		double dist[MAX_BIN_SIZE];
	};
	bmus_verts bmus;
	for (int i = 0; i < MAX_BIN_SIZE; i++) {bmus.dist[i]=1000000;}

	bool bmu_found = false;
	set<int> total_bmus_found;
	double new_x = 0, new_y = 0, new_z = 0;
	int downs_vert_counter = 0;

	W W;

	double cluster_bin_size = ceil(100/downsample_percent); // this needs to be less than total orig verts


	for (int in_i = 0; in_i < IV; in_i++) {
		total_bmus_found.insert(in_i);
		for (int in_i2 = 0; in_i2 < IV; in_i2++) {
			if (total_bmus_found.find(in_i2) != total_bmus_found.end()) {
				// new bmu search
				bmu_found = false;
				new_dist = 1000000;
				for (int bmus_i = 0; bmus_i < (cluster_bin_size - 1); bmus_i++) {
					new_dist = find_euclidean_dist(orig_mesh.x[in_i], orig_mesh.y[in_i], orig_mesh.z[in_i], orig_mesh.x[in_i2], orig_mesh.y[in_i2], orig_mesh.z[in_i2]);
					if (new_dist < bmus.dist[bmus_i] & bmu_found == false) {
						bmus.i[bmus_i] = in_i2;
						bmus.dist[bmus_i] = new_dist;
						bmu_found == true;
					}
				}
			}
		}

		// downsampled verts
		new_x = 0, new_y = 0, new_z = 0;
		for (int bmus_i = 0; bmus_i < (cluster_bin_size - 1); bmus_i++) {
			new_x += orig_mesh.x[bmus.i[bmus_i]];
			new_y += orig_mesh.y[bmus.i[bmus_i]];
			new_z += orig_mesh.z[bmus.i[bmus_i]];

			total_bmus_found.insert(bmus.i[bmus_i]);
		}
		new_x = new_x / cluster_bin_size;
		new_y = new_y / cluster_bin_size;
		new_z = new_z / cluster_bin_size;

		downs_mesh.x[downs_vert_counter] = new_x;
		downs_mesh.y[downs_vert_counter] = new_y;
		downs_mesh.z[downs_vert_counter] = new_z;
		downs_vert_counter++;
	}

	cout<<endl<<"found bmus:"<<endl;
	/*for (std::set<int>::const_iterator it = total_bmus_found.begin();it!=total_bmus_found.end();it++) {
		std::cout << "total_bmus_found " << it. << std::endl;
	}*/
	for (int const& bmu_entry : total_bmus_found)
	{
		std::cout << bmu_entry << ' ';
	}
}

void downsample_mesh() {
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
	for (int map_i = 0; map_i < T; map_i++) {W.x[map_i] = 1; W.y[map_i] = 1; W.z[map_i] = 1;}

	// train
	for (int L_i = 0; L_i < L; L_i++) {
		//cout<<endl<<"SOM iteration "<<L_i;
		for (int in_i = 0; in_i < IV; in_i++) {
			u = 0;
			new_dist = 0;
			bmu_dist = 100000;//find_euclidean_dist(orig_mesh.x[0], orig_mesh.y[0], orig_mesh.z[0], downs_mesh.x[0], downs_mesh.y[0], downs_mesh.z[0]); // reinitialize
			//cout<<endl<<"bmu find o_m index "<<in_i;
			for (int map_i = 0; map_i < T; map_i++) {
				new_dist = find_euclidean_dist(orig_mesh.x[in_i], orig_mesh.y[in_i], orig_mesh.z[in_i], downs_mesh.x[map_i], downs_mesh.y[map_i], downs_mesh.z[map_i]);
				if (new_dist < bmu_dist) {bmu_dist = new_dist; u = map_i;}
				//cout<<endl<<"\tu"<<u<<" o_x "<<orig_mesh.x[in_i]<<"\to_y "<<orig_mesh.y[in_i]<<"\to_z "<< orig_mesh.z[in_i]<<"\td_x "<< downs_mesh.x[map_i]<<"\td_y "<< downs_mesh.y[map_i]<<"\td_z "<< downs_mesh.z[map_i]<<"\tbmu_dist "<<bmu_dist<<" new_dist "<<new_dist;
			}
			//cout<<endl<<"\tu\t"<<u<<"\tbmu_dist\t"<<bmu_dist;
			for (int map_i = 0; map_i < T; map_i++) {
				//cout<<" *x*";
				W.x[map_i] = weight_update(orig_mesh.x[u], W.x[map_i], bmu_dist, L_i, map_i, u);
				//cout<<" *y* ";
				W.y[map_i] = weight_update(orig_mesh.y[u], W.y[map_i], bmu_dist, L_i, map_i, u);
				//cout<<" *z* ";
				W.z[map_i] = weight_update(orig_mesh.z[u], W.z[map_i], bmu_dist, L_i, map_i, u);
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

void create_mesh(int mesh_x, int mesh_y, int mesh_z, string type) {
	/*
	 * Fill in sample coordinates into meshs for testing
	 *
	 * x,y,z can be modified in loop to change resulting verticies
	 */
	int i = 0;
	double x = 0, y = 0, z = 0;

	for (int x_i = 0; x_i < mesh_x; x_i++) {
		for (int y_i = 0; y_i < mesh_y; y_i++) {
			for (int z_i = 0; z_i < mesh_z; z_i++) {
				if (type == "orig") {
					x = x_i*4-1.5;
					y = y_i*4-1.5;
					z = z_i*4-1.5;
					orig_mesh.x[i]=x; orig_mesh.y[i]=y; orig_mesh.z[i]=z;
					//cout<<endl<<x<<"\t"<<y<<"\t"<<z<<"\t"<<i<<"\t"<<(double)x;
				}
				if (type == "downs") {
					x = x_i*2;
					y = y_i*2;
					z = z_i*2;
					downs_mesh.x[i]=x; downs_mesh.y[i]=y; downs_mesh.z[i]=z;
					//cout<<endl<<x2<<"\t"<<y2<<"\t"<<z2<<"\t"<<i<<"\t"<<(double)x;
				}
				i++;
			}
		}
	}
	//cout<<endl<<endl<<"test "<<orig_mesh.x[0]<<" "<<orig_mesh.y[0]<<" "<<orig_mesh.z[0];
}

void copy_mesh() {
	for (int i = 0; i < ORIG_MESH_VERTS; i++) {
		o_mesh_sorted.x[i] = orig_mesh.x[i];
		o_mesh_sorted.y[i] = orig_mesh.y[i];
		o_mesh_sorted.z[i] = orig_mesh.z[i];
	}
}

int main() {
	/*
	 * Create downsampling
	 */
	ostringstream orig_mesh_print;
	ostringstream targ_mesh_print;

	cout<<endl<<"started"<<endl;

	create_mesh(3,2,2,"orig");
	copy_mesh();
	create_mesh(2,2,2,"downs");

	orig_mesh_print<<"\r\n"<<"\r\n"<<"original mesh coordinates:"<<"\r\n";
	for (int i = 0; i < ORIG_MESH_VERTS; i++) {
		orig_mesh_print<<orig_mesh.x[i]<<"\t"<<orig_mesh.y[i]<<"\t"<<orig_mesh.z[i]<<"\r\n";
	}

	targ_mesh_print<<"\r\n"<<"\r\n"<<"target mesh coordinates:"<<"\r\n";
	for (int i = 0; i < DOWNS_MESH_VERTS; i++) {
		targ_mesh_print<<downs_mesh.x[i]<<"\t"<<downs_mesh.y[i]<<"\t"<<downs_mesh.z[i]<<"\r\n";
	}

	//downsample_mesh();
	//downsample_mesh2(50);
	init_downs_verts(1.0);

	cout<<targ_mesh_print.str();
	cout<<orig_mesh_print.str();

	/*cout<<endl<<endl<<"sorted mesh coordinates:"<<endl;
	for (int i = 0; i < ORIG_MESH_VERTS; i++) {
		cout<<o_mesh_sorted.x[i]<<"\t"<<o_mesh_sorted.y[i]<<"\t"<<o_mesh_sorted.z[i]<<endl;
	}*/

	cout<<endl<<endl<<"initial downs mesh coordinates:"<<endl;
	for (int i = 0; i < DOWNS_MESH_VERTS; i++) {
		cout<<downs_mesh.x[i]<<"\t"<<downs_mesh.y[i]<<"\t"<<downs_mesh.z[i]<<endl;
	}

	downsample_mesh();

	cout<<endl<<endl<<"downsampled mesh coordinates:"<<endl;
	for (int i = 0; i < DOWNS_MESH_VERTS; i++) {
		cout<<downs_mesh.x[i]<<"\t"<<downs_mesh.y[i]<<"\t"<<downs_mesh.z[i]<<endl;
	}

	/*double test[3];
	test[0] = 4;
	test[0] = 3;
	test[2] = 5;
	cout<<endl<<test[0]<<"\t"<<test[1]<<"\t"<<test[2];
	cout<<endl<<ORIG_MESH_VERTS<<" "<<DOWNS_MESH_VERTS;*/

	/*
		original mesh coordinates:
		-1.5	-1.5	-1.5
		-1.5	-1.5	2.5
		-1.5	2.5	-1.5
		-1.5	2.5	2.5
		2.5	-1.5	-1.5
		2.5	-1.5	2.5
		2.5	2.5	-1.5
		2.5	2.5	2.5
		6.5	-1.5	-1.5
		6.5	-1.5	2.5
		6.5	2.5	-1.5
		6.5	2.5	2.5

		target mesh coordinates:
		0	0	0
		0	0	2
		0	2	0
		0	2	2
		2	0	0
		2	0	2
		2	2	0
		2	2	2
	 */

	cout<<endl<<"finished"<<endl;
	return 0;
}






