#include "transition_matrix.h"

TransitionMatrix::TransitionMatrix() :
	transitionMatrix(loadTransitionMatrix()),
	cumSumMatrix(calculateCumSumMatrix(transitionMatrix)) { }
Matrix TransitionMatrix::loadTransitionMatrix()
{
	// Add an additional matrix row to represent defaulted companies.
	Matrix ret("transition_matrix_for_project.csv", 3);
	MatrixRow defaultRow(7); //Default 7 rows to 0
	defaultRow.push_back(1); //Add last row as 1
	ret.push_back(defaultRow);
	for (size_t i = 0, n = ret.rows(); i < n; i++)
	{
		for (size_t j = 0, m = ret.columns(); j < m;  j++)
			ret[i][j] = ret[i][j] / 100;
	}
	return ret;
}
Matrix TransitionMatrix::calculateCumSumMatrix(Matrix& matrix)
{
	Matrix ret(matrix.rows(), matrix.columns());
	//For each row
	for (size_t i = 0, n = matrix.rows(); i < n; i++)
	{
		//For each column
		MatrixRow& row = matrix[i];
		//First entry is always 1 to solve rounding errors
		//Cummulative summations should always add to one
		ret[i][0] = 1;
		for (size_t j = 1, n = row.size(); j < n; j++)
		{
			double sum = 0;
			//Sum forwards
			for (size_t k = j; k < n; k++)
				sum = sum + row[k];
			ret[i][j] = sum;
		}
	}
	return ret;
}
