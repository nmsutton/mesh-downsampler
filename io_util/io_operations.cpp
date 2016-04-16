/*
 * Tools for I/O
 */

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <vector>

using namespace std;

struct input_file {
	/*
	 * vectors were used to enable dynamic sizing of the value groups
	 * in a straightforward way
	 */
	vector<double> bounding_box_vert;
	vector<double> pos_x, pos_y, pos_z, pos_t;
	vector<double> vel_x, vel_y, vel_z, vel_t;
	vector<double> ela_p_j, ela_r_ij, ela_val1, ela_val2;
	vector<double> mem_val1, mem_val2, mem_val3;
	vector<double> partMemInd;
};

struct input_file import_data(string in_filename) {
	/*
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
	//s(in_filename);

	cout<<"input file: "<<in_filename<<endl;
	ifstream inFile(in_filename);
	if (!inFile) {
		cerr << "File "<<in_filename<<" not found." << endl;
		exit (EXIT_FAILURE);
	}

	// Using getline() to read one line at a time.
	string line;
	while (getline(inFile, line)) {
		if (line.empty()) continue;

		istringstream file_data(line);
		//cout<<endl<<" line "<<line;

		if (line == "[position]") {pos_sect = true; bb_sect = false;}
		else if (line == "[velocity]") {vel_sect = true; pos_sect = false;}
		else if (line == "[connection]") {conn_sect = true; vel_sect = false;}
		else if (line == "[membranes]") {mem_sect = true; conn_sect = false;}
		else if (line == "[particleMemIndex]") {part_sect = true; mem_sect = false;}
		else if (line == "[end]") {part_sect = false;}
		else if (bb_sect == true) {
			if (file_data >> input_x2) {
				input_file_prop.bounding_box_vert.push_back(input_x2);
				//cout<<endl<<"BB "<<input_x2<<endl;
				//cout<<endl<<"BB "<<input_file_prop.bounding_box_vert.at(bb_i)<<endl;
				//bb_i++;
			}
		}
		else if (pos_sect == true) {
			if (file_data >> input_x >> input_y >> input_z >> input_t) {
				input_file_prop.pos_x.push_back(input_x);
				input_file_prop.pos_y.push_back(input_y);
				input_file_prop.pos_z.push_back(input_z);
				//input_file_prop.pos_t[p_i] = input_t;
				//cout<<endl<<"BB3 "<<input_file_prop.bounding_box_vert.at(1)<<endl;
				//p_i++;
			}
		}
		else if (vel_sect == true) {
			if (file_data >> input_x >> input_y >> input_z >> input_t) {
				input_file_prop.vel_x.push_back(input_x);
				input_file_prop.vel_y.push_back(input_y);
				input_file_prop.vel_z.push_back(input_z);
				input_file_prop.vel_t.push_back(input_t);
				//v_i++;
			}
		}
		else if (conn_sect == true) {
			if (file_data >> input_x >> input_y >> input_z >> input_t) {
				input_file_prop.ela_p_j.push_back(input_x);
				input_file_prop.ela_r_ij.push_back(input_y);
				input_file_prop.ela_val1.push_back(input_z);
				input_file_prop.ela_val2.push_back(input_t);
				//e_i++;
			}
		}
		else if (mem_sect == true) {
			if (file_data >> input_x >> input_y >> input_z) {
				input_file_prop.mem_val1.push_back(input_x);
				input_file_prop.mem_val2.push_back(input_y);
				input_file_prop.mem_val3.push_back(input_z);
				//m_i++;
			}
		}
		else if (part_sect == true) {
			if (file_data >> input_x) {
				input_file_prop.partMemInd.push_back(input_x);
				//pmi_i++;
			}
		}
	}

	inFile.close();

	//cout<<endl<<"BB2 "<<input_file_prop.bounding_box_vert.at(7)<<endl;

	return input_file_prop;
}

int test() {
	return 5;
}
