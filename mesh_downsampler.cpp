/*
 * This software downsamples meshes for scenes the format of configuration files
 * in the Open Worm project's Sibernetic software.
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
#include "general_mesh_operations.h"

using namespace std;

int main(int argc, char *argv[]) {
	/*
	 * This is the section that calls the processes that generate downsampling.
	 *
	 * Reference:
	 * http://www.cplusplus.com/forum/articles/13355/
	 * http://www.learncpp.com/cpp-tutorial/73-passing-arguments-by-reference/
	 */

	string infile = "";
	string outfile = "";
	double downs_percent = 100;
	som.active = false;
	som.steps = 10;
	som.neigh = 9.0;
	som.learn = 0.75;

	// parse args
	if (argc == 2) {
		if (string(argv[1]) == "--help") {
			cout << "Help for Usage "<<endl<<endl
				<<"-i = input configuration file"<< endl
				<<"-o = output configuration file"<< endl
				<<"-ds = percentage of input file to output through downsampling"<< endl
				<<"-som = self-organizing maps (som) are activated or not"<< endl
				<<"-steps = iteration steps the som should use.  Default = 10"<< endl
				<<"-neigh = scalar multiplier that increases the vertex neighborhood area"<<endl
				<<"som uses for each downsampled point.  Default = 9.0.  Larger = more."<< endl
				<<"-learn = scalar multiplier that effects the learning rate of the som."<< endl
				<<"The learning rate effects the amount of pull vertices in the neighborhood"<<endl
				<<"cause change on the downsampled points.  Default = 0.75.";
			exit(EXIT_SUCCESS);
		}
	}
	else if (argc < 4) {
		cerr << "usage is -i <input file> -o <output file> -ds <downsample percentage> -som <true/false> -steps <som steps> -neigh <som neighbor multiplier> -learn <som learning multiplier>\r\n";
		cerr << "enter --help for longer help description";
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
			else if (string(argv[i]) == "-som") {
				if (string(argv[i+1])=="true") {som.active = true;}
			}
			else if (string(argv[i]) == "-steps") {
				som.steps = atoi(argv[i+1]);
			}
			else if (string(argv[i]) == "-neigh") {
				som.neigh = double(atoi(argv[i+1]));
			}
			else if (string(argv[i]) == "-learn") {
				som.learn = double(atoi(argv[i+1]));
			}
		}
	}

	orig_data = import_data(infile); // NOTE: how is io_operations different from the past?
	ORIG_MESH_VERTS = orig_data.x.size();
	DOWNS_MESH_VERTS = ceil((double) ORIG_MESH_VERTS * (downs_percent/100.0));

	cout<<endl<<"started"<<endl;

	init_downs_verts(1.0, ORIG_MESH_VERTS, DOWNS_MESH_VERTS, orig_data, downs_mesh);

	print_data_results("start");

	if (som.active == true) {
		downsample_mesh(ORIG_MESH_VERTS, DOWNS_MESH_VERTS, orig_data, downs_mesh, som);
	}

	print_data_results("end");

	cout<<endl<<"finished"<<endl;
	return 0;
}
