#include "AlgoAnalysis.h"


AlgoTimer::AlgoTimer() = default; 

void AlgoTimer::mapElementCountsToExecutionTimes()
{
	vector<int> elementCounts = { 10, 100, 1'000, 10'000, 100'000, 200'000, 500'000, 1'000'000};

	/*measure time required to loop up to element count*/

	for (const auto& elementCount : elementCounts)
	{
		auto start = std::chrono::high_resolution_clock::now(); 
		for (int i = 0; i < elementCount; ++i)
		{
			//nothing 
			//double root = sqrt(i); //leads to "interesting" plot
		}
		auto end = std::chrono::high_resolution_clock::now(); 

		auto executionTime = (end - start).count(); 

		elementCountsToExecutionTimes.insert({ elementCount, executionTime });
	}
	cout.imbue(std::locale(""));

	for (const auto& currentPair : elementCountsToExecutionTimes)
	{
		cout << currentPair.first << " took " << currentPair.second << " ns\n";
	}
}


array<string, 2> CurveFit::getLinearFitDetails(map<int, int> experimentalData)
{
	vector<int> x; 
	vector<int> y; 
	vector<long long> xSquared; 
	vector<long long> xy; 
	int N = experimentalData.size(); 

	for (const auto& currentPair : experimentalData)
	{
		x.push_back(currentPair.first);
		y.push_back(currentPair.second); 

		xSquared.push_back(static_cast<long long>(currentPair.first) * currentPair.first);
		xy.push_back(static_cast<long long>(currentPair.first) * currentPair.second);
	}

	double sumY = accumulate(y.begin(), y.end(), 0.0);
	double sumXSquared = accumulate(xSquared.begin(), xSquared.end(), 0.0);
	double sumX = accumulate(x.begin(), x.end(), 0.0);
	double sumXY = accumulate(xy.begin(), xy.end(), 0.0);

	/*Using: https://byjus.com/maths/linear-regression/*/
	/*a is y-intercept*/
	double numeratorOfy_intercept = (sumY * sumXSquared) - (sumX * sumXY);
	double denominatorOfy_intercept = N * sumXSquared - (sumX * sumX);

	double y_intercept = numeratorOfy_intercept / denominatorOfy_intercept;

	//cout << "A (y-interecept) is: " << a << "\n";
	double numeratorOfSlope = N * sumXY - (sumX * sumY);
	double denominatorOfSlope = N * sumXSquared - (sumX * sumX);

	double slope = numeratorOfSlope / denominatorOfSlope;

	//cout << "B (slope) is: " << b << "\n";

	// Calculate R^2
	double meanY = sumY / N;
	double ssTotal = 0.0;
	double ssResidual = 0.0;

	for (int i = 0; i < N; ++i)
	{
		double yPredicted = y_intercept + slope * x[i];
		ssTotal += (y[i] - meanY) * (y[i] - meanY);
		ssResidual += (y[i] - yPredicted) * (y[i] - yPredicted);
	}

	double rSquared = 1 - (ssResidual / ssTotal);

	//cout << "R^2 is: " << rSquared << "\n";

	ostringstream ossPart1; 
	ossPart1 << "T = " << fixed << setprecision(2) << slope << "N + " << y_intercept;

	ostringstream ossPart2;
	ossPart2 << "WITH R^2 = " << rSquared;

	array<string, 2> linearFitDetails =
	{
		ossPart1.str(),
		ossPart2.str()
	};

	return linearFitDetails; 
	
}

pair<double, double> CurveFit::getSlopeAndIntercept(map<int, int> experimentalData)
{
	vector<int> x;
	vector<int> y;
	vector<long long> xSquared;
	vector<long long> xy;
	int N = experimentalData.size();

	for (const auto& currentPair : experimentalData)
	{
		x.push_back(currentPair.first);
		y.push_back(currentPair.second);

		xSquared.push_back(static_cast<long long>(currentPair.first) * currentPair.first);
		xy.push_back(static_cast<long long>(currentPair.first) * currentPair.second);
	}

	double sumY = accumulate(y.begin(), y.end(), 0.0);
	double sumXSquared = accumulate(xSquared.begin(), xSquared.end(), 0.0);
	double sumX = accumulate(x.begin(), x.end(), 0.0);
	double sumXY = accumulate(xy.begin(), xy.end(), 0.0);

	/*Using: https://byjus.com/maths/linear-regression/*/
	/*a is y-intercept*/
	double numeratorOfYIntercept = (sumY * sumXSquared) - (sumX * sumXY);
	double denominatorOfYIntercept = N * sumXSquared - (sumX * sumX);

	double y_intercept = numeratorOfYIntercept / denominatorOfYIntercept;

	double numeratorOfSlope = N * sumXY - (sumX * sumY);
	double denominatorOfSlope = N * sumXSquared - (sumX * sumX);

	double slope = numeratorOfSlope / denominatorOfSlope;

	return { slope, y_intercept };
}
