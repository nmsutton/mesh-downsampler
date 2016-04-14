/*
 * Reference:
 * http://stackoverflow.com/questions/24987600/i-would-like-to-create-a-function-that-reads-each-line-of-the-input-and-produces
 */

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

struct input_file {
	double bounding_box_x1[];
	double bounding_box_x2[];
	double bounding_box_y1[];
	double bounding_box_y2[];
	double bounding_box_z1[];
	double bounding_box_z2[];
};

input_file import_data(string in_filename) {
	/*string line;
	input_file input_file_prop;
	ifstream infile(in_filename);
	for( std::string line; getline( infile, line ); )
	{
	    istringstream iss(line);
	    iss<<

	    // process pair (a,b) //
	}*/

	input_file input_file_prop;
	int b_box_i = 0; // bounding box
	double b_box_val = 0.0;
    //int bb_x, bb_y, bb_z;
    bool bb_sect = false, pos_sect = false, vel_sect = false,
    conn_sect = false, mem_sect = false, part_sect = false;

	  ifstream inFile(in_filename);
	  if (!inFile) {
	    cerr << "File "<<in_filename<<" not found." << endl;
	    return -1;
	  }

	  // Using getline() to read one line at a time.
	  string line;
	  while (getline(inFile, line)) {
	    if (line.empty()) continue;

	    // Using istringstream to read the line into integers.
	    //istringstream iss(line);
	    //int sum = 0, next = 0;
	    //while (iss >> next) sum += next;
	    //input_file_prop.bounding_box_x[b_box_i] << sum << endl;

	    istringstream iss(line);
	    //if (!(iss >> bb_x >> bb_y >> bb_z)) { break; } // error
	    iss >> input_file_prop.bounding_box_x1;
	    iss >> input_file_prop.bounding_box_x2;
	    iss >> input_file_prop.bounding_box_y1;
	    iss >> input_file_prop.bounding_box_y2;
	    iss >> input_file_prop.bounding_box_z1;
	    iss >> input_file_prop.bounding_box_z2;

	    //input_file_prop.bounding_box_y[b_box_i] << bb_y;
	    //input_file_prop.bounding_box_z[b_box_i] << bb_z;

	    b_box_i++;
	  }

	  inFile.close();

	return input_file_prop;
}
