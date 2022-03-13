
#include <iostream>
#include <cmath>
#include <fstream>

#include "iMatrix.h"

using namespace std;

iMatrix::iMatrix(int N, int M)
{
	int i, j;

	this->N = N;
	this->M = M;

	// add an extra row and col for optional i=0 and j=0
	p = new int [(N+1)*(M+1)];

	if( p == NULL ) {
		cout << "\nmemory allocation error in iMatrix::iMatrix(...)";
		return;
	}

	for(i=0;i<=N;i++) {
		for(j=0;j<=M;j++) {
			e(i,j) = 0;
		}
	}

}


int & iMatrix::e(int i, int j)
{
	int k;
	static int ans = 0;

	if( i<0 || i>N || j<0 || j>M ) {
		cout << "\nerror in iMatrix::e(): out of bounds";
		return ans;
	} 

	// add an extra row and col for optional i=0 and j=0
	k = (M+1)*i + j;

	return p[k];
}


void iMatrix::print()
{
	int i, j;

	for(i=1;i<=N;i++) {
		for(j=1;j<=M;j++) {
			cout << e(i,j) << " ";
		}
		cout << "\n";
	}

}


void iMatrix::print0()
{
	int i, j;

	for(i=0;i<=N;i++) {
		for(j=0;j<=M;j++) {
			cout << e(i,j) << " ";
		}
		cout << "\n";
	}

}

