#include "price_matrix.h"

PriceMatrix::PriceMatrix(PortfolioData& portfolioData, YieldData& yieldData) 
	: Matrix(portfolioData.size(), 8)
{
	for (size_t i = 0, n1 = this->rows(); i < n1; i++)
	{
		PortfolioEntry& row = portfolioData[i];
		double timeToMaturity = 3;
		YieldEntry* yield = yieldData.getByTerm(timeToMaturity);
		if(! yield)
			throw new runtime_error(string() + "Did not find yield for term: " + to_string(timeToMaturity));
		if (row.instrumentType == "CDS")
		{
			for (size_t j = 0, n2 = this->columns() - 1; j < n2; j++) {
				double discount = (1 / pow((1 + ((*yield)[j]/row.couponsPerYear)), row.couponsPerYear*timeToMaturity));
				(*this)[i][j] = 100 * ((((*yield)[j] - row.coupon - (*yield)[7]) / (*yield)[j]) * (1 - discount));
			}
		}
		else
		{
			for (size_t j = 0, n2 = this->columns() - 1; j < n2; j++) {
				double discount = (1 / pow((1 + ((*yield)[j]/row.couponsPerYear)), row.couponsPerYear*timeToMaturity));
				(*this)[i][j] = 100 * (((row.coupon / (*yield)[j]) * (1 - discount)) + discount);
			}
		}
		double alpha = 1;
		double beta = (1 - row.exprr) / row.exprr;
		(*this)[i][this->columns()-1] = 100 * BetaRandomNumberGenerator(alpha, beta).rand();
	}
}