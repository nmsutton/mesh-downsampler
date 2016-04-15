/*
 * Tools for I/O
 */

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>     /* exit, EXIT_FAILURE */

using namespace std;

struct input_file {
	double bounding_box_vert[];
	double pos_x[], pos_y[], pos_z[], pos_t[];
	double vel_x[], vel_y[], vel_z[], vel_t[];
	double ela_p_j[], ela_r_ij[], ela_val1[], ela_val2[];
	double mem_val1[], mem_val2[], mem_val3[];
	double partMemInd[];
};

input_file import_data(string in_filename) {
	/*
	 * References:
	 * http://stackoverflow.com/questions/24987600/i-would-like-to-create-a-function-that-reads-each-line-of-the-input-and-produces
	 * http://stackoverflow.com/questions/7868936/read-file-line-by-line
	 */
	input_file input_file_prop;
	int bb_i = 0, p_i = 0, v_i = 0, e_i= 0, m_i = 0, pmi_i = 0; // indices for all sections.
	bool bb_sect = true, pos_sect = false, vel_sect = false,
			conn_sect = false, mem_sect = false, part_sect = false;
	double input_x = 0, input_y = 0, input_z = 0, input_t = 0;

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

		if (file_data == "[position]") {pos_sect = true; bb_sect = false;}
		else if (file_data == "[velocity]") {vel_sect = true; pos_sect = false;}
		else if (file_data == "[connection]") {conn_sect = true; vel_sect = false;}
		else if (file_data == "[membranes]") {mem_sect = true; conn_sect = false;}
		else if (file_data == "[particleMemIndex]") {part_sect = true; mem_sect = false;}
		else if (file_data == "[end]") {part_sect = false;}
		else if (bb_sect == true) {
			if (file_data >> input_x) {
				input_file_prop.bounding_box_vert[bb_i] = input_x;
				bb_i++;
			}
		}
		else if (pos_sect == true) {
			if (file_data >> input_x >> input_y >> input_z >> input_t) {
				input_file_prop.pos_x[p_i] = input_x;
				input_file_prop.pos_y[p_i] = input_y;
				input_file_prop.pos_z[p_i] = input_z;
				input_file_prop.pos_t[p_i] = input_t;
				p_i++;
			}
		}
		else if (vel_sect == true) {
			if (file_data >> input_x >> input_y >> input_z >> input_t) {
				input_file_prop.vel_x[v_i] = input_x;
				input_file_prop.vel_y[v_i] = input_y;
				input_file_prop.vel_z[v_i] = input_z;
				input_file_prop.vel_t[v_i] = input_t;
				v_i++;
			}
		}
		else if (conn_sect == true) {
			if (file_data >> input_x >> input_y >> input_z >> input_t) {
				input_file_prop.ela_p_j[e_i] = input_x;
				input_file_prop.ela_r_ij[e_i] = input_y;
				input_file_prop.ela_val1[e_i] = input_z;
				input_file_prop.ela_val2[e_i] = input_t;
				e_i++;
			}
		}
		else if (mem_sect == true) {
			if (file_data >> input_x >> input_y >> input_z) {
				input_file_prop.mem_val1[m_i] = input_x;
				input_file_prop.mem_val2[m_i] = input_y;
				input_file_prop.mem_val3[m_i] = input_z;
				m_i++;
			}
		}
		else if (part_sect == true) {
			if (file_data >> input_x) {
				input_file_prop.partMemInd[pmi_i] = input_x;
				pmi_i++;
			}
		}
	}

	inFile.close();

	return input_file_prop;
}
