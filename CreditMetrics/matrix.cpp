#include "matrix.h"

MatrixRow::MatrixRow(const vector<string>& cells)
{
	for (size_t i = 1, n = cells.size(); i < n; i++)
	{
		push_back(convertDouble(cells.at(i)));
	}
}
const string MatrixRow::toString()
{
	string ret;
	for (size_t i = 0, n = size(); i < n; i++)
	{
		if (i > 0)
			ret = ret + ",";
		ret = ret + to_string(at(i));
	}
	return ret;
}
Matrix::Matrix(const string& filename, size_t skipLines) : CSV(filename, skipLines) {}