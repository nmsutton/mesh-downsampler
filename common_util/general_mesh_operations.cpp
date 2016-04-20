/*
 * general_mesh_operations.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: nmsutton
 */

#include <string>
#include <vector>
#include <iostream>

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

extern downsampled_mesh downs_mesh;
extern input_file orig_data;
extern input_file o_mesh_sorted;

extern int ORIG_MESH_VERTS;
extern int DOWNS_MESH_VERTS;

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
					orig_data.x[i]=x; orig_data.y[i]=y; orig_data.z[i]=z;
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
		o_mesh_sorted.x[i] = orig_data.x[i];
		o_mesh_sorted.y[i] = orig_data.y[i];
		o_mesh_sorted.z[i] = orig_data.z[i];
	}
}

void print_data_results(string section) {
	if (section == "start") {
		cout<<endl<<"Mesh position results:"
			<<endl<<"1.) original mesh coordinates"
			<<endl<<"2.) initial downsampled mesh coordinates:"
			<<endl<<"3.) som processed downsampled mesh coordinates:"<<endl<<endl;

		for (int i = 0; i < orig_data.x.size(); i++) {
			cout<<"["<<orig_data.x[i]<<", "<<orig_data.y[i]<<", "<<orig_data.z[i]<<"], "<<endl;
		}

		cout<<endl;
		for (int i = 0; i < downs_mesh.x.size(); i++) {
			cout<<"["<<downs_mesh.x[i]<<", "<<downs_mesh.y[i]<<", "<<downs_mesh.z[i]<<"], "<<endl;
		}
		cout<<endl;
	}
	else if (section == "end") {
		for (int i = 0; i < downs_mesh.x.size(); i++) {
			cout<<"["<<downs_mesh.x[i]<<", "<<downs_mesh.y[i]<<", "<<downs_mesh.z[i]<<"], "<<endl;
		}
		cout<<endl<<endl<<"plot: http://jsfiddle.net/pd3jp1s2/2/"<<endl;
		cout<<endl<<endl<<"For new config file:"<<endl;
		for (int i = 0; i < downs_mesh.x.size(); i++) {
			cout<<downs_mesh.x[i]<<"\t"<<downs_mesh.y[i]<<"\t"<<downs_mesh.z[i]<<"\t2.1"<<endl;
		}
	}
}


