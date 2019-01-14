#include "Gauss.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

Vec Gauss::solve(Vec c1, Vec c2, Vec c3, Vec b) {
    std::vector<int> perm;
    double a[3][3];
    perm = vector<int>();
    for (int i = 0; i < 3; i++) {
        a[i][0] = c1[i];
        a[i][1] = c2[i];
        a[i][2] = c3[i];
    }

    // keep track of the permutation of the rows

    for (int i = 0; i < 3; i++)
        perm.push_back(i);

    bool warn = false;
    // now we eliminate the j-th column
    for (int j = 0; j < 3; j++) {
        // first we use the partial pivoting strategy
        // i.e. we choose the pivot with the largest abs value
        double best = a[perm[j]][j];
        int ind = j;
        for (int i = j + 1; i < 3; i++)
            if (abs(a[perm[i]][j]) > abs(best)) {
                best = a[perm[i]][j];
                ind = i;
            }

        swap(perm[ind], perm[j]);

        // Do the subtraction
        for (int i = j + 1; i < 3; i++) {
            if (abs(a[perm[j]][j]) < 1e-10)
                warn = true;
            double ratio = a[perm[i]][j] / a[perm[j]][j];
            a[perm[i]][j] = ratio;
            for (int jj = j + 1; jj < 3; jj++)
                a[perm[i]][jj] -= a[perm[j]][jj] * ratio;
        }
    }

    if (warn)
        cerr <<"<     Warning!! Condition number might be large...      >" <<endl <<endl;
	assert(3 && 3);
	//forward substituion to solve Ly = b
	vector<double> y;
	for (int i = 0; i < 3; i++) {
		double rVal = b[perm[i]];
		for (int j = 0; j < i; j++) 
			rVal -= y[j] * a[perm[i]][j];
		y.push_back(rVal);
	}

	//backward substituion to solve Ux = y 
	vector<double> x;	
	for (int i = 3 - 1; i >= 0; i--) {
		double rVal = y[i];
		for (int j = 3 - 1; j > i; j--)
			rVal -= x[3 - j - 1] * a[perm[i]][j];

		x.push_back(rVal / a[perm[i]][i]);
	}
	return {x[2], x[1], x[0]};
}