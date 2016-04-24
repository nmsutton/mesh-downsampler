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
	string current_path = "";
	string config_gen_path = "";
	string phys_sects_file = "";
	string python_path = "";
	double downs_percent = 100;
	som.active = false;
	som.steps = 10;
	som.neigh = 9.0;
	som.learn = 0.75;
	bool detailed_results_print = false;

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
				<<"cause change on the downsampled points.  Default = 0.75."<<endl
				<<"-config_gen_path = path of the sibernetic_config_gen folder"<<endl
				<<"NOTE: this software assumes python can be run from any folder"<<endl
				<<"because it will try to run python from config_gen_path"<<endl
				<<endl<<"Example: -i input/scene7 -o configuration/new_scene7b -ds 50 -som true"<<endl
				<<"-config_gen_program /CompNeuro/Software/openworm/sibernetic_config_gen/";
			exit(EXIT_SUCCESS);
		}
	}
	else if (argc < 4) {
		cerr << "usage is -i <input file> -o <output file> -ds <downsample percentage> -som <true/false> -steps <som steps> -neigh <som neighbor multiplier> -learn <som learning multiplier>\r\n";
		cerr << "enter --help for longer help description";
		exit(EXIT_FAILURE);
	}
	else {
		current_path = string(argv[0]);
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
			else if (string(argv[i]) == "-config_gen_program") {
				config_gen_path = string(argv[i+1]);
			}
			else if (string(argv[i]) == "-phys_sects_file") {
				phys_sects_file = string(argv[i+1]);
			}
			else if (string(argv[i]) == "-python_path") {
				// currently not used
				python_path = string(argv[i+1]);
			}
		}
	}

	orig_data = import_data(infile);
	phys_sects = import_phys_sects(phys_sects_file);
	ORIG_MESH_VERTS = orig_data.x.size();
	DOWNS_MESH_VERTS = ceil((double) ORIG_MESH_VERTS * (downs_percent/100.0));

	cout<<endl<<"started"<<endl;

	init_downs_verts(1.0, ORIG_MESH_VERTS, DOWNS_MESH_VERTS, orig_data, phys_sects, downs_sects);

	if (detailed_results_print) {print_data_results("start");}

	if (som.active == true) {
		downsample_mesh(ORIG_MESH_VERTS, DOWNS_MESH_VERTS, orig_data, downs_mesh, som);
	}

	if (detailed_results_print) {print_data_results("end");}

	export_config_files(temp_downs_output, phys_sects, downs_sects, config_gen_path, current_path, outfile);

	combine_config_files(phys_sects, current_path, outfile);

	cout<<endl<<"finished"<<endl;

	return 0;
}
