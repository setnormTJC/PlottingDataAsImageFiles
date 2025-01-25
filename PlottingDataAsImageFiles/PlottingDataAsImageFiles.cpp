


#include"PlotImage.h"

//#include"AlgoTimer.h"

void demoLinearForLoopPlottingAndFitting()
{
    AlgoTimer algoTimer{};

    algoTimer.mapElementCountsToExecutionTimes();

    CurveFit curveFit{};

    //string linearFitEqn = curveFit.getLinearFit(simpleTestData);
    array<string, 2> linearFitDetails = curveFit.getLinearFitDetails(algoTimer.elementCountsToExecutionTimes);
    pair<double, double> slopeAndYIntercept = curveFit.getSlopeAndIntercept(algoTimer.elementCountsToExecutionTimes);


    try {

        const string filename = "out.bmp";

        unsigned int plotWidth = 1'000u;
        unsigned int plotHeight = 1'000u;

        PlotImage image(plotWidth, plotHeight, ColorEnum::Black);

        image.plotData(algoTimer.elementCountsToExecutionTimes, ColorEnum::Yellow);

        //Display best fit eqn: 
        image.displayText(linearFitDetails[0], plotWidth / 4, 3 * plotHeight / 4, ColorEnum::White, 20);
        //Display R^2:
        image.displayText(linearFitDetails[1], plotWidth / 4, 3 * plotHeight / 4 - plotHeight / 20, ColorEnum::Red, 20);

        //Draw line of best fit:
        image.drawLineOfBestFit(slopeAndYIntercept, algoTimer.elementCountsToExecutionTimes, ColorEnum::White);

        image.writeImageFile(filename);


        //std::cin.get(); 


        system(filename.c_str());


    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        __debugbreak();
    }

}

void testLinearPlottingAndFitting(const map<int, int> testXYData)
{
    PlotImage image(1000u, 1000u, ColorEnum::Black);
    image.plotData(testXYData, ColorEnum::Yellow);

    CurveFit curveFit{};
    auto linearFitDetails = curveFit.getLinearFitDetails(testXYData);
    image.displayText(linearFitDetails[0], 100u, 100u, ColorEnum::White, 20);
    image.displayText(linearFitDetails[1], 100u, 75u, ColorEnum::Red, 20);

    auto slopeAndIntercept = curveFit.getSlopeAndIntercept(testXYData); 

    image.drawLineOfBestFit(slopeAndIntercept, testXYData, ColorEnum::White);

    image.writeImageFile("simpleLinear.bmp"); 

    system("simpleLinear.bmp"); 
}

int main() 
{

    demoLinearForLoopPlottingAndFitting(); 

    map<int, int> simpleXYData =
    {
        {1, 3},
        {2, 6},
        {3, 9},
        {4, 12},
        {5, 15},
        {6, 18}
    };
    
    testLinearPlottingAndFitting(simpleXYData); 

    return 0;
}