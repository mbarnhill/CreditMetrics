=======================
772 project description
=======================


BACKGROUND:

(write anything that could be useful for someone who wants to compile and run your code)

- I am developing with the following software: Microsoft Visual Studio

- I use the following special libraries (not installed by default, I downloaded from http://www.boost.org/): 'boost'

- File "Screen Shot 2016-03-13 at 4.10.15 PM.png" is a screenshot showing the outputs of the execution of my program when it runs correctly.



FILES STRUCTURE:

(what are the files here for)

- the file main.cpp is the file calling the function 'main()' where all the routines/functions are themselves called. All responses to the questions in the steps of the project are in main.cpp and their locations are indicated with comments.

- csv.h/hxx/cpp : this is a template class that implements functions to read in and clean up csv files. This class is included in each .cpp file for which a .csv file is read in. Note: there are some issues with template classes and header files in C++, hence the .hxx work-around. 

- industries.h/cpp : implements the classes ‘IndustryEntry’ and ‘IndustryData’ that represent an industry from the file industries.csv.

- issuers.h/cpp : implements the classes ‘IssuerEntry’ and ‘IssuerData’ that represent an issuer from the file issuers.csv.

- matrix.h/cpp : implements the classes ‘MatrixRow’ and ‘Matrix’ to store matrix data as an array of vector doubles. This is used in general to store manipulatable matrices.

- monte.h/cpp : implements the class ‘Monte’ that performs a Monte Carlo simulation of N scenarios.

- portfolio.h/cpp : implements the classes ‘PortfolioEntry’ and ‘PortfolioData’ that represent a portfolio of securities from the file portfolio_for_project.csv. The functions getReportedValue() and getReportedValue(), which calculated the theoretical and market value of the portfolio, are included in PortfolioData.

- rand.h/cpp : implements the classes ‘UniformRandomNumberGenerator’ and ‘NormalRandomNumberGenerator’ that generate random numbers from the uniform or the normal distribution.

- scenario.h/cpp : implements the classes ‘IndustryScenario’, ‘IndustryScenarioData’, ‘ScenarioEntry’ and ’Scenario’ to perform two random draws for each industry, and a random draw for each issuer, which are then used to calculate the percentile draw (and therefore the new rating) for each issuer.

- stdafx.h/cpp : a Visual Studio file, sometimes needs to be included to run in Visual Studio but I think I have addressed this.

- targetver.h : another Visual Studio file.

- transition_matrix.h/cpp : implements the class ‘TransitionMatrix’ that represents the ratings transition matrix, transition_matrix_for_project.csv. We also perform a cumulative summation on said matrix. Uses “Matrix”.

- yields.h/cpp : implements the classes ‘YieldEntry’ and ‘YieldData’ that represent the yield curve matrix.

- correlation_matrix_for_project.csv: the file where the correlations between the issuers are defined.

- industries.csv: the file where the industries are defined, along with their global systemic correlation indices, to be used in calculating the percentile in scenario.cpp.

- issuers.csv: the file where the issuers are defined, along with their rating, industry and industry correlation indices, to be used in calculating the percentile in scenario.cpp.

- portfolio_for_project.csv: the file where all the instruments are defined, along with their issuers, CUSIP, Notional, Maturity, Coupon, Coupon frequency, Price, Yield, Theoretical clean price and Expected recovery rate. This file is used for calculating the original theoretical and actual portfolio price.

- transition_matrix_for_project.csv: the file where the transition probability for each standard rating is given.

- yield_curve_for_project.csv: the file where the yields over separate maturities, for each rating, are given.


 NOTES:
The XXXEntry classes read in the entries from the specific .csv files, and label them. The XXXData classes are used to get all information on the entries from their given label.

 