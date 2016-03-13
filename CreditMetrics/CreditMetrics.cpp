/*! CreditMetrics.cpp : Defines the entry point for the console application.
	Note: The given .csv files are not formatted consistently. The .csv parsers below are 
	designed to handle the inconsistencies present in the files in a somewhat 
	general manner by removing extra characters and white space.
	Note: Some of the values in the csv file are set to "-", which is read as zero by c++, 
	so we do not account for this particular inconsistency, as we intend that any of these to be zero regardless.
*/
#include <string>
#include <vector>
#include <numeric>
#include <random>
#include <boost/numeric/ublas/matrix.hpp>

#include "csv.h"
#include "issuers.h"
#include "portfolio.h"
#include "yields.h"
#include "matrix.h"
#include "scenario.h"
#include "monte.h"

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		// Sets up random number generator
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<> dis(0, 1);

		// Response for Part B, Step 1) 
		// Read in all the .csv files and create transition and correlation matrices.
		IssuerData issuerData;
		// cout << issuerData.toString();
		PortfolioData portfolioData;
		// cout << portfolioData.toString();
		YieldData yieldData;
		// cout << yieldData.toString();
		Matrix correlationMatrix("correlation_matrix_for_project.csv", 1);
		// cout << correlationMatrix.toString();
		Matrix transitionMatrix("transition_matrix_for_project.csv", 3);
		
		// Add an additional matrix row to represent defaulted companies.
		MatrixRow row{ 0,0,0,0,0,0,0,1 };
		transitionMatrix.push_back(row);
		// cout << transitionMatrix.toString();

		// Response for Part B, Step 2)
		// Get reported and theoretical portfolio values.
		cout << "The initial reported portfolio value is " << portfolioData.getReportedValue() << endl;
		cout << "The initial theoretical portfolio value is " << portfolioData.getTheorValue() << endl;

		/* Response for Part B, Step 3) 
		Return a matrix of possible prices for each instrument for each possible credit rating.
		For the moment, we designate each performing bond price to be the theoretical price from above, 
		and each performing CDS to be a random number between 0 and 1 (on a notional of $100).
		Defaulted bonds and CDSs are designated as the expected recovery values.
		NOTE: requires the boost library.
		*/
		boost::numeric::ublas::matrix<double> priceMatrix(portfolioData.size(), 8);
		for (size_t i = 0, n1 = priceMatrix.size1(); i < n1; i++)
		{
			PortfolioEntry& row = portfolioData.at(i);
			if (row.instrumentType == "CDS")
			{
				for (size_t j = 0, n2 = priceMatrix.size2() - 1; j < n2; j++)
					priceMatrix(i, j) = dis(gen);
			}
			else
			{
				for (size_t j = 0, n2 = priceMatrix.size2() - 1; j < n2; j++)
					priceMatrix(i, j) = row.cleanPrice;
			}
			priceMatrix(i, priceMatrix.size2() - 1) = row.exprr * 100;
		}
		//cout << priceMatrix << endl;

		// Response for Part B, Step 4)
		// Set N generate N scenarios for the ratings of the companies.
		int N = 2000;
		Monte monteCarlo(N, issuerData);
	
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
				portfolioValue = portfolioValue + ((priceMatrix(j, scenarioEntry->rating)*portfolio.notional) * ((double)1/100));
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
		// QuickFix: Set stdev to zero if stdev is +/-NaN, due to above issue.
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