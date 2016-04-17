/*
 * find_distances.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: nmsutton
 */

#include <math.h>       /* sqrt */

using namespace std;

double find_euclidean_dist(double x_1, double y_1, double z_1, double x_2, double y_2, double z_2) {
	/*
	 * https://en.wikipedia.org/wiki/Euclidean_distance
	 */
	double distance = sqrt(pow((x_2-x_1),2)+pow((y_2-y_1),2)+pow((z_2-z_1),2));
	return distance;
}


