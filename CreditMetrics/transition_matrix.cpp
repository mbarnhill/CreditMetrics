#include "transition_matrix.h"

TransitionMatrix::TransitionMatrix() :
	transitionMatrix(loadTransitionMatrix()),
	cumSumMatrix(calculateCumSumMatrix(transitionMatrix)) { }
Matrix TransitionMatrix::loadTransitionMatrix()
{
	// Add an additional matrix row to represent defaulted companies.
	Matrix ret("transition_matrix_for_project.csv", 3);
	ret.push_back({ 0,0,0,0,0,0,0,1 });
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
		for (size_t j = 0, n = row.size(); j < n; j++)
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