/*
 * Tools for I/O
 *
 * Reference:
 * http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
 * http://www.programmingforums.org/thread16393.html
 * https://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/
 */

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <vector>

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

struct physics_sects {
	vector<double> x1, x2, y1, y2, z1, z2;
	vector<double> h_scalar, p_type_range_min, p_type_range_max;
	string file;
};

struct input_file import_data(string in_filename) {
	/*
	 * Only particle types (input_t) < 3.0 are imported because
	 * >= 3.0 are boundry particles which are auto generated
	 * and not needed to import.
	 *
	 * NOTE: Using getline() to read one line at a time.
	 *
	 * References:
	 * http://stackoverflow.com/questions/24987600/i-would-like-to-create-a-function-that-reads-each-line-of-the-input-and-produces
	 * http://stackoverflow.com/questions/7868936/read-file-line-by-line
	 */
	input_file input_file_prop;
	int bb_i = 0, p_i = 0, v_i = 0, e_i= 0, m_i = 0, pmi_i = 0; // indices for all sections.
	bool bb_sect = true, pos_sect = false, vel_sect = false,
			conn_sect = false, mem_sect = false, part_sect = false;
	double input_x = 0, input_x2 = 0, input_y = 0, input_z = 0, input_t = 0;
	stringstream line_tokens;

	cout<<"input file: "<<in_filename<<endl;
	ifstream inFile(in_filename);
	if (!inFile) {
		cerr << "File "<<in_filename<<" not found." << endl;
		exit (EXIT_FAILURE);
	}

	string line;
	while (getline(inFile, line)) {
		if (line.empty()) continue;

		istringstream file_data(line);

		if (line == "[position]") {pos_sect = true; bb_sect = false;}
		else if (line == "[velocity]") {vel_sect = true; pos_sect = false;}
		else if (line == "[connection]") {conn_sect = true; vel_sect = false;}
		else if (line == "[membranes]") {mem_sect = true; conn_sect = false;}
		else if (line == "[particleMemIndex]") {part_sect = true; mem_sect = false;}
		else if (line == "[end]") {part_sect = false;}
		else if (bb_sect == true) {
			if (file_data >> input_x2) {
				input_file_prop.bounding_box_vert.push_back(input_x2);
			}
		}
		else if (pos_sect == true) {
			if (file_data >> input_x >> input_y >> input_z >> input_t) {
				if (input_t < 3.0) {
					input_file_prop.x.push_back(input_x);
					input_file_prop.y.push_back(input_y);
					input_file_prop.z.push_back(input_z);
					input_file_prop.t.push_back(input_t);
				}
			}
		}
		else if (vel_sect == true) {
			if (file_data >> input_x >> input_y >> input_z >> input_t) {
				if (input_t < 3.0) {
					input_file_prop.vel_x.push_back(input_x);
					input_file_prop.vel_y.push_back(input_y);
					input_file_prop.vel_z.push_back(input_z);
					input_file_prop.vel_t.push_back(input_t);
				}
			}
		}
		else if (conn_sect == true) {
			if (file_data >> input_x >> input_y >> input_z >> input_t) {
				input_file_prop.ela_p_j.push_back(input_x);
				input_file_prop.ela_r_ij.push_back(input_y);
				input_file_prop.ela_val1.push_back(input_z);
				input_file_prop.ela_val2.push_back(input_t);
			}
		}
		else if (mem_sect == true) {
			if (file_data >> input_x >> input_y >> input_z) {
				input_file_prop.mem_val1.push_back(input_x);
				input_file_prop.mem_val2.push_back(input_y);
				input_file_prop.mem_val3.push_back(input_z);
			}
		}
		else if (part_sect == true) {
			if (file_data >> input_x) {
				input_file_prop.partMemInd.push_back(input_x);
			}
		}
	}

	inFile.close();

	return input_file_prop;
}

struct physics_sects import_phys_sects(string phys_sects_file) {
	physics_sects initial_phys_sects;
	double x1, x2, y1, y2, z1, z2;
	double h_scalar, p_type_range_min, p_type_range_max;

	stringstream line_tokens;

	cout<<"phys_sects_file: "<<phys_sects_file<<endl;
	ifstream inFile(phys_sects_file);
	if (!inFile) {
		cerr << "File "<<phys_sects_file<<" not found." << endl;
		exit (EXIT_FAILURE);
	}

	string line;
	while (getline(inFile, line)) {
		if (line.empty()) continue;

		istringstream file_data(line);

		if (file_data >> x1 >> x2 >> y1 >> y2 >> z1 >> z2 >> h_scalar >> p_type_range_min >> p_type_range_max) {
			initial_phys_sects.x1.push_back(x1);
			initial_phys_sects.x2.push_back(x2);
			initial_phys_sects.y1.push_back(y1);
			initial_phys_sects.y2.push_back(y2);
			initial_phys_sects.z1.push_back(z1);
			initial_phys_sects.z2.push_back(z2);
			initial_phys_sects.h_scalar.push_back(h_scalar);
			initial_phys_sects.p_type_range_min.push_back(p_type_range_min);
			initial_phys_sects.p_type_range_max.push_back(p_type_range_max);
		}
	}

	inFile.close();

	return initial_phys_sects;
}


string exec(const char* cmd) {
	FILE* pipe = popen(cmd, "r");
	if (!pipe) return "ERROR";
	char buffer[128];
	string result = "";
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	return result;
}

string dbl_to_str(double i) {
	stringstream ss;
	ss << i;
	return ss.str();
}

string int_to_str(int i) {
	stringstream ss;
	ss << i;
	return ss.str();
}

void write_config_file(string out_filename, vector<downsampled_mesh> &downs_sects, int section) {
	/*
	 * Write a temp output file containing all points in a region the physics group specifies.
	 * E.g. one mesh such as a box in some cases
	 */
	downsampled_mesh d_sect = downs_sects[section];
	ofstream outFile(out_filename);

	for (int bb_i = 0; bb_i < d_sect.bounding_box_vert.size(); bb_i++) {
		outFile<<d_sect.bounding_box_vert[bb_i]<<endl;
	}

	outFile<<"[position]"<<endl;

	for (int i = 0; i < d_sect.x.size(); i++) {
		outFile<<d_sect.x[i]<<"\t"<<d_sect.y[i]<<"\t"<<d_sect.z[i]<<"\t"<<d_sect.t[i]<<endl;
	}

	outFile.close();
}

void export_config_files(string temp_downs_output, physics_sects &phys_sects, vector<downsampled_mesh> &downs_sects, string config_gen_path, string current_path, string outfile) {
	/*
	 * output downsampled data.  process it with sibernetic_config_gen.
	 * export config file with sibernetic_config_gen to be run with
	 * sibernetic.
	 */
	string trimmed_current_path = current_path.substr(0,current_path.size()-24);
	string trimmed_temp_downs = "";
	string temp_downs_filename = "";

	for (int sect_i = 0; sect_i < phys_sects.h_scalar.size(); sect_i++) {
		temp_downs_filename = temp_downs_output+"_"+int_to_str(sect_i);
		trimmed_temp_downs = temp_downs_filename.substr(2,temp_downs_filename.size());
		write_config_file(temp_downs_filename, downs_sects, sect_i);

		cout<<endl<<"initiating sibernetic_config_gen with config sect "<<int_to_str(sect_i)<<endl;

		//wrapper for sibernetic_config_gen
		string prog_and_new_data = "cd " + config_gen_path + " && python -u " + config_gen_path + "main.py -i " + trimmed_current_path +
				trimmed_temp_downs + " -p " + dbl_to_str(phys_sects.h_scalar[sect_i]) + " -o " + trimmed_current_path + outfile + "_" + int_to_str(sect_i);
		const char * prog_with_downs_data = prog_and_new_data.c_str ();
		cout<<endl<<"running: "<<prog_and_new_data<<endl;
		exec(prog_with_downs_data);
	}
}

void find_sect_positions(string in_filename, vector<long> &sects_line_indices) {
	/*
	 * +1 in sects_line_indices positioning to account for end of lines.
	 * What is wanted is the start of the next line after the marker.
	 */
	string pos_marker = "[position]", vel_marker = "[connection]", con_marker = "[connection]",
			mem_marker = "[membranes]", pmi_marker = "[particleMemIndex]", end_marker = "[end]";

	cout<<endl<<"finding config section line indices"<<endl;

	ifstream inFile(in_filename);
	if (!inFile) {
		cerr << "File "<<in_filename<<" not found." << endl;
		exit (EXIT_FAILURE);
	}

	string line;
	while (getline(inFile, line)) {
		if (line.empty()) continue;

		istringstream file_data(line);

		if (line == pos_marker) {sects_line_indices[0] = inFile.tellg()+pos_marker.size()+1;}
		else if (line == vel_marker) {sects_line_indices[1] = inFile.tellg()+vel_marker.size()+1;}
		else if (line == con_marker) {sects_line_indices[2] = inFile.tellg()+con_marker.size()+1;}
		else if (line == mem_marker) {sects_line_indices[3] = inFile.tellg()+mem_marker.size()+1;}
		else if (line == pmi_marker) {sects_line_indices[4] = inFile.tellg()+pmi_marker.size()+1;}
		else if (line == end_marker) {sects_line_indices[5] = inFile.tellg()+end_marker.size()+1;}
	}
}

void copy_file(string file_in, string file_out) {
	ifstream inFile(file_in);
	ofstream outFile(file_out);

	string line;
	while (getline(inFile, line)) {
		if (line.empty()) continue;

		outFile << line << endl;
	}

	outFile.close();
	inFile.close();
}

void combine_config_files(physics_sects &phys_sects, vector<downsampled_mesh> &downs_sects, string current_path, string outfile) {
	/*
	 * Combine multiple config files into one config file
	 *
	 * First the input file for the first section is copyied to the output file.
	 * Data from each remaining section is then appended to the output file.
	 */
	bool bb_sect = true, pos_sect = false, vel_sect = false,
			conn_sect = false, mem_sect = false, part_sect = false;
	string trimmed_current_path = current_path.substr(0,current_path.size()-24);
	string in_filename = "";
	string out_filename = outfile+"_comb";
	string trimmed_out_filename = out_filename.substr(2,out_filename.size());
	vector<string> pos_markers = {"[position]", "[velocity]", "[connection]", "[membranes]", "[particleMemIndex]", "[end]"};
	vector<vector<string>> config_output_sects;
	int file_sections = pos_markers.size();
	for (int i = 0; i < file_sections; i++) {vector<string> section; config_output_sects.push_back(section);};

	cout<<endl<<"combining config file sections"<<endl;

	/*in_filename = trimmed_current_path + outfile + "_0";//temp_downs_output+"_0";
	copy_file(in_filename, out_filename);*/

	for (int sect_i = 0; sect_i < phys_sects.h_scalar.size(); sect_i++) {
		in_filename = trimmed_current_path + outfile + "_" +int_to_str(sect_i);

		bb_sect = true; pos_sect = false; vel_sect = false;
		conn_sect = false; mem_sect = false; part_sect = false;
		/*find_sect_positions(out_filename, sects_line_indices);
		cout<<endl<<"pos "<<sects_line_indices[1]<<endl;*/

		ifstream inFile(in_filename);
		if (!inFile) {
			cerr << "File "<<in_filename<<" not found." << endl;
			exit (EXIT_FAILURE);
		}

		string line;
		while (getline(inFile, line)) {
			if (line.empty()) continue;

			istringstream file_data(line);

			if (line == pos_markers[0]) {pos_sect = true; bb_sect = false;}// outFile.seekp(sects_line_indices[0]); outFile<<endl;}
			else if (line == pos_markers[1]) {vel_sect = true; pos_sect = false;}//  outFile.seekp(sects_line_indices[1]); outFile<<endl;}
			else if (line == pos_markers[2]) {conn_sect = true; vel_sect = false;}//  outFile.seekp(sects_line_indices[2]); outFile<<endl;}
			else if (line == pos_markers[3]) {mem_sect = true; conn_sect = false;}//  outFile.seekp(sects_line_indices[3]); outFile<<endl;}
			else if (line == pos_markers[4]) {part_sect = true; mem_sect = false;}//  outFile.seekp(sects_line_indices[4]); outFile<<endl;}
			else if (line == pos_markers[5]) {part_sect = false;}
			/*else if (bb_sect == true) {

			}*/
			else if (pos_sect == true & line != pos_markers[0]) {
				config_output_sects[0].push_back(line);
			}
			else if (vel_sect == true & line != pos_markers[1]) {
				config_output_sects[1].push_back(line);
			}
			else if (conn_sect == true & line != pos_markers[2]) {
				config_output_sects[2].push_back(line);
			}
			else if (mem_sect == true & line != pos_markers[3]) {
				config_output_sects[3].push_back(line);
			}
			else if (part_sect == true & line != pos_markers[4]) {
				config_output_sects[4].push_back(line);
			}
		}

		inFile.close();
	}

	ofstream outFile(out_filename);

	for (int i = 0; i < downs_sects[0].bounding_box_vert.size(); i++) {
		outFile<<downs_sects[0].bounding_box_vert[i]<<endl;
	}

	for (int i = 0; i < (file_sections); i++) {
		outFile<<pos_markers[i]<<endl;
		for (string new_conf_line : config_output_sects[i]) {
			outFile<<new_conf_line<<endl;
		}
	}

	//outFile<<

	outFile.close();
}
