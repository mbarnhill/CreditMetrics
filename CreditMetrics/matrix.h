#ifndef __matrix_h__
#define __matrix_h__

#include "csv.h"

/*! MatrixRow
Stores an array of doubles, representing a row in a matrix
*/
class MatrixRow : public vector<double>
{
public:
	MatrixRow(const vector<string>&);
	using vector::vector;
	const string toString();
};
/*! Matrix
Stores an array of matrix rows (vector of vectors of doubles), representing a full matrix
*/
class Matrix : public CSV<MatrixRow>
{
public:
	Matrix(const string&, size_t);
};
#endif