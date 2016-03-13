#ifndef __transition_matrix_h__
#define __transition_matrix_h__

#include "matrix.h"

class TransitionMatrix
{
public:
	TransitionMatrix();
	Matrix transitionMatrix;
	Matrix cumSumMatrix;
private:
	Matrix loadTransitionMatrix();
	Matrix calculateCumSumMatrix(Matrix& matrix);
};

#endif