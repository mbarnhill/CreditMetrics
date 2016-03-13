#include "matrix.h"

MatrixRow::MatrixRow(const size_t size)
{
	for (size_t i = 0; i < size; i++)
		push_back(0);
}
MatrixRow::MatrixRow(const vector<string>& cells)
{
	for (size_t i = 1, n = cells.size(); i < n; i++)
		push_back(convertDouble(cells.at(i)));
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
Matrix::Matrix(const size_t rows, const size_t columns)
{
	for (size_t i = 0; i < rows; i++)
		push_back(MatrixRow(columns));
}
Matrix::Matrix(const string& filename, size_t skipLines) : CSV(filename, skipLines) {}
const size_t Matrix::rows()
{
	return size();
}
const size_t Matrix::columns()
{
	if (rows() <= 0)
		return 0;
	return at(0).size();
}