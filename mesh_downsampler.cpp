/*
 * main.cpp
 *
 *  Created on: Apr 6, 2016
 *      Author: nmsutton
 */

#include <math.h>       /* sqrt */
#include <stdio.h>
#include <iostream>
#include <sstream>		/* ostringstream */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include "core_objects.h"
#include "io_operations.h"
#include "init_downs_data.h"
#include "create_downsample.h"

using namespace std;

int main(int argc, char *argv[]) {
	/*
	 * This is the section that calls the processes that generate downsampling.
	 *
	 * Reference:
	 * http://www.cplusplus.com/forum/articles/13355/
	 * http://www.learncpp.com/cpp-tutorial/73-passing-arguments-by-reference/
	 */
	ostringstream orig_mesh_print;
	ostringstream targ_mesh_print;
	string infile = "";
	string outfile = "";
	double downs_percent = 100;
	// parse args
	if (argc < 4) {
		cerr << "usage is -i <input file> -o <output file> -ds <downsample percentage>";
		exit(EXIT_FAILURE);
	}
	else {
		for (int i = 1; i < argc; i++) {
			if (string(argv[i]) == "-i") {
				infile = string(argv[i+1]);
			}
			else if (string(argv[i]) == "-o") {
				outfile = string(argv[i+1]);
			}
			else if (string(argv[i]) == "-ds") {
				downs_percent = double(atoi(argv[i+1]));
			}
		}
	}

	orig_data = import_data(infile); //how is io_operations different from the past?
	ORIG_MESH_VERTS = orig_data.x.size();
	DOWNS_MESH_VERTS = ceil((double) ORIG_MESH_VERTS * (downs_percent/100.0));

	cout<<endl<<"started"<<endl;

	init_downs_verts(1.0, ORIG_MESH_VERTS, DOWNS_MESH_VERTS, orig_data, downs_mesh);

	cout<<endl<<endl<<"1.) original mesh coordinates";
	cout<<endl<<"2.) initial downsampled mesh coordinates:";
	cout<<endl<<"3.) processed downsampled mesh coordinates:"<<endl<<endl;

	for (int i = 0; i < orig_data.x.size(); i++) {
		cout<<"["<<orig_data.x[i]<<", "<<orig_data.y[i]<<", "<<orig_data.z[i]<<"], "<<endl;
	}

	cout<<endl;
	for (int i = 0; i < downs_mesh.x.size(); i++) {
		cout<<"["<<downs_mesh.x[i]<<", "<<downs_mesh.y[i]<<", "<<downs_mesh.z[i]<<"], "<<endl;
	}

	downsample_mesh(ORIG_MESH_VERTS, DOWNS_MESH_VERTS, orig_data, downs_mesh);

	cout<<endl;
	for (int i = 0; i < downs_mesh.x.size(); i++) {
		cout<<"["<<downs_mesh.x[i]<<", "<<downs_mesh.y[i]<<", "<<downs_mesh.z[i]<<"], "<<endl;
	}
	cout<<endl<<endl<<"plot: http://jsfiddle.net/pd3jp1s2/2/"<<endl;

	cout<<endl<<"finished"<<endl;
	return 0;
}
