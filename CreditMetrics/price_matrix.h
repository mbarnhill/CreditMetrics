#ifndef __price_matrix_h__
#define __price_matrix_h__

#include "matrix.h"
#include "portfolio.h"
#include "rand.h"
#include "yields.h"
#include "math.h"

/*! Matrix
Stores an array of matrix rows (vector of vectors of doubles), representing a full matrix
*/
class PriceMatrix : public Matrix
{
public:
	PriceMatrix(PortfolioData&, YieldData&);
};
#endif
