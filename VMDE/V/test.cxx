#include "V.hpp"
#include <cstdio>
int main() {
	VVector<double> v;
	v.push(1.2);
	v.push(3.4);
	v.push(7.5);
	v.push(9.9);
	v.push(11.2);
	v.push(13.4);
	v.push(17.5);
	v.push(19.9);
	v.push(111.2);
	v.push(113.4);
	v.push(117.5);
	v.push(119.9);
	printf("%f %f %f %f\n", v[0], v[1], v[2], v[3]);
	printf("%f %f %f %f\n", v[4], v[5], v[6], v[7]);
	printf("%f %f %f %f\n", v[8], v[9], v[10], v[11]);
	printf("%zu\n", v.size());
	printf("%f\n", v.pop());
}
