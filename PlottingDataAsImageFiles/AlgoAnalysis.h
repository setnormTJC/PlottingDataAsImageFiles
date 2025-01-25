#pragma once

#include<chrono> 

#include<vector> 
#include<map> 

#include<iostream> 

#include<string> 
#include<numeric> //for accumulate (used in linear regression) 

#include<sstream>


#include<array> 

#include<iomanip>

using std::vector, std::map, std::cout, std::string, std::accumulate, std::to_string;
using std::ostringstream, std::array, std::fixed, std::setprecision, std::pair; 

class CurveFit
{
public: 
	vector<string> fitTypes =
	{
		"constant", //ex: vectorName.front()
		"logarithmic" //ex: binary search 
		"linear",  //ex: sequential search 
		"linearithmic", //ex: quicksort 
		"quadratic", //ex: bubble sort
		"base2exponential", //ex: Hanoi
		"m^n" //ex: chess game tree
	};

	CurveFit() = default; 

	/*
	@returns -> 0th element string is the eqn of best fit
	...1st element of the array is the R^2 value (also as a string)
	*/
	array<string, 2> getLinearFitDetails(map<int, int> experimentalData);

	pair<double, double> getSlopeAndIntercept(map<int, int> experimentalData);

};


class AlgoTimer
{

public: 
	AlgoTimer(); 


	map<int, int> elementCountsToExecutionTimes; 

	/*Just a demo for now*/
	void mapElementCountsToExecutionTimes(); 

	friend class CurveFit;
};

