/*! main.cpp : Defines the entry point for the console application.
	Note: The given .csv files are not formatted consistently. The .csv parsers below are 
	designed to handle the inconsistencies present in the files in a somewhat 
	general manner by removing extra characters and white space.
	Note: Some of the values in the csv file are set to "-", which is read as zero by c++, 
	so we do not account for this particular inconsistency, as we intend that any of these to be zero regardless.
*/
#include <string>
#include <vector>
#include <numeric>
#include <boost/numeric/ublas/matrix.hpp>

#include "csv.h"
#include "issuers.h"
#include "portfolio.h"
#include "price_matrix.h"
#include "yields.h"
#include "matrix.h"
#include "transition_matrix.h"
#include "scenario.h"
#include "monte.h"
#include "industries.h"
#include "rand.h"

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		//Random number generator
		//UniformRandomNumberGenerator unirand(0, 1);

		// Response for Part B, Step 1) 
		// Read in all the .csv files and create transition and correlation matrices.
		IssuerData issuerData;
		IndustryData industryData;
		PortfolioData portfolioData;
		YieldData yieldData;
		Matrix correlationMatrix("correlation_matrix_for_project.csv", 1);
		TransitionMatrix transitionMatrix;

		// Response for Part B, Step 2)
		// Get reported and theoretical portfolio values.
		cout << "The initial reported portfolio value is " << portfolioData.getReportedValue() << endl;
		cout << "The initial theoretical portfolio value is " << portfolioData.getTheorValue() << endl;

		//Calculate prices for each portfolio based on yields
		PriceMatrix priceMatrix(portfolioData, yieldData);

		// Response for Part B, Step 4)
		// Set N generate N scenarios for the ratings of the companies.
		int N = 2000;
		NormalRandomNumberGenerator nrand(0,1);
		Monte monteCarlo(N, nrand, issuerData, industryData, transitionMatrix);
	
		// Response for Part B, Step 5)
		// Go through the N scenarios and compute the value of the portfolio,
		// and change in value of the portfolio in each scenario.
		vector<double> portfolioValues;
		vector<double> changeInValues;
		double changeInValueTotal = 0;
		for (size_t i = 0, n1 = monteCarlo.scenarios.size(); i < n1; i++)
		{
			double portfolioValue = 0;
			Scenario& scenario = monteCarlo.scenarios.at(i);
			for (size_t j = 0, n2 = portfolioData.size(); j < n2; j++)
			{
				PortfolioEntry& portfolio = portfolioData.at(j);
				ScenarioEntry* scenarioEntry = scenario.getByName(portfolio.name);
				if (!scenarioEntry)
					throw runtime_error("No known scenario entry for \"" + portfolio.name + "\"");
				portfolioValue = portfolioValue + ((priceMatrix[j][scenarioEntry->rating]*portfolio.notional) * ((double)1/100));
			}
			portfolioValues.push_back(portfolioValue);
			double changeInValue = portfolioValue - portfolioData.getTheorValue();
			changeInValues.push_back(changeInValue);
			changeInValueTotal = changeInValueTotal + changeInValue;
		}

		// Response for Part B, Step 6)
		// Calculate statistics for the change in portfolio value
		double meanChangeInValue = changeInValueTotal / (double)changeInValues.size();
		double sq_sum = inner_product(changeInValues.begin(), changeInValues.end(), changeInValues.begin(), 0.0);
		// NOTE: This calculation of standard deviation may not work for small values. 
		// TO DO: Fix this so a standard deviation of 0 does not cause an overflow.
		double stdev = sqrt((sq_sum / (double)changeInValues.size()) - (meanChangeInValue * meanChangeInValue));
		// Quick Fix: Set stdev to zero if stdev is +/-NaN, due to above issue.
		if (stdev != stdev)
			stdev = 0;

		cout << "The average change in value is " << meanChangeInValue << endl;
		cout << "The standard deviations of the change in value is " << stdev << endl;

		sort(changeInValues.begin(), changeInValues.end());
		double var95Percentile = changeInValues.at((int) (changeInValues.size()*0.05));
		double var99Percentile = changeInValues.at((int) (changeInValues.size()*0.01));
		// Using (int) will ensure that the index is an integer. It performs a 'floor' operation by 
		// removing the decimal part of changeInValues.size()*alpha, which is what is needed for VaR calculations. 

		double percentile = 98;
		double cvar = 0;
		for (size_t i = 0, n1 = (int)(((100-percentile)*0.01)*changeInValues.size()); i < n1; i++)
		{
			cvar = cvar + changeInValues.at(i);
		}
		double cvarPercentile = cvar / (double)(((100 - percentile)*0.01)*changeInValues.size());
	
		cout << "Note: Negative VaR indicates a loss" << endl;
		cout << "VaR at the 95th percentile is " << var95Percentile << endl;
		cout << "VaR at the 99th percentile is " << var99Percentile << endl;
		cout << "CVaR at the desired percentile is " << cvarPercentile << endl;
	}
	// Print all exceptions to stderr
	catch (const exception &e)
	{
		cerr << "error: " << e.what() << "\n";
	}
	cout << "Hit enter to exit" << endl;
	getchar();
}
