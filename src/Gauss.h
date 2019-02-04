#ifndef GAUSS_H
#define GAUSS_H
#include "utils.h"
#include <vector>

class Gauss {
	// This class is for low-order Gauss elimination partial pivoting
public:
	// this function solve the system Ly = b, Ux = y
	static Vec solve(Vec, Vec, Vec, Vec);

};
#endif /* GAUSS_H */
