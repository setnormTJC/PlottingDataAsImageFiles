#include "PlotImage.h"



PlotImage::PlotImage(unsigned int imageWidth, unsigned int imageHeight, const Color& fillColor)
	:BMPImage(imageWidth, imageHeight, fillColor)
    , imageWidth(imageWidth), imageHeight(imageHeight)
{

    plotWidth = imageWidth - PADDING; //100 is arbitary padding value
    plotHeight = imageHeight - PADDING; 

    displayText("N (ELEMENT COUNT)", plotWidth / 2, PADDING/2, ColorEnum::Cyan, 24);

    displayText("TIME (NANOSECONDS)", 0, plotHeight, ColorEnum::Cyan, 24);

    drawXAxis(origin.first, origin.second, plotWidth, ColorEnum::Magenta);
    drawYAxis(origin.first, origin.second, plotHeight, ColorEnum::Green);

}

void PlotImage::displayText(const string& textToRender, unsigned int xOffset, unsigned  int yOffset
    , const ColorEnum& color, int fontSize)
{
    std::swap(xOffset, yOffset); //

    std::string pathToTTFFontFile = "C:\\Windows\\Fonts\\CascadiaMono.ttf";
    //int fontSize = 24;

    Font font(textToRender, pathToTTFFontFile, fontSize);

    fontBitmap = font.bitmap;

    for (int row = 0; row < fontBitmap.height; ++row)
    {
        for (int col = 0; col < fontBitmap.width; ++col)
        {
            unsigned char pixel = fontBitmap.buffer[row * fontBitmap.width + col];
            //std::cout << (pixel > 128 ? '#' : ' ');
            if (pixel > 128) //arbitrary-ish cutoff
            {
                pixelData.pixelMatrix[row + xOffset][col + yOffset] = Color{ color };
            }

            // Print '#' for pixels with value > 128, otherwise print ' '
        }
        //std::cout << "\n";
    }

}

void PlotImage::drawLineOfBestFit(pair<double, double> slopeAndYIntercept, 
    const map<int, int>& elementCountsToExecutionTimes, const ColorEnum& color)
{

    ////cout << "\n\nm = " << slopeAndYIntercept.first << "\n";
    ////cout << "b = " << slopeAndYIntercept.second << "\n\n";

    double m = slopeAndYIntercept.first;
    double b = slopeAndYIntercept.second;

    map<int, int> bestFit_NTCoordinates; 
    //first get all y values in the "t versus N" coordinates
    for (int n = static_cast<int>(b); n < elementCountsToExecutionTimes.rbegin()->first; ++n)
    {
        int t = static_cast<int>(m * n + b);
        //if (n % 10'000 == 0)
        //{
        //    cout << n << "\t" << t << "\n";
        //}
        if (n % plotWidth == 0) //likely assuming square plot dimensions here 
        {
            bestFit_NTCoordinates.insert({ n, t });
        }
    }

    //now convert to pixel coordinates 
        // Get the maximum element count and execution time for scaling
    int maxElementCount = elementCountsToExecutionTimes.rbegin()->first;
    int maxExecutionTime = 0;
    for (const auto& currentPair : elementCountsToExecutionTimes)
    {
        if (currentPair.second > maxExecutionTime)
        {
            maxExecutionTime = currentPair.second;
        }
    }

    // Convert to pixel coordinates and plot
    for (const auto& ntPair : bestFit_NTCoordinates)
    {
        int n = ntPair.first;
        int t = ntPair.second;

        int x = origin.first + (n * (plotWidth - origin.first)) / maxElementCount;
        int y = origin.second + (t * (plotHeight - origin.second)) / maxExecutionTime;

        if (x >= 0 && x < plotWidth && y >= 0 && y < plotHeight)
        {
            setPixelToColor_withThickness(x, y, Color{ color }, 1);
        }
    }

}


void PlotImage::drawXAxis(unsigned int x0, unsigned int y0, unsigned int xf, const ColorEnum& color)
{
    const int TICK_SPACING = plotWidth / 4; 

    for (unsigned int x = x0; x < xf; ++x)
    {
        pixelData.pixelMatrix[y0][x] = Color{ color };

        //display "tick" every 25% of the way: 
        if ((x - x0) % TICK_SPACING == 0)
        {
            //add extra vertical pixels:
            const int yPixelCountForTick = 10; 

            for (int y = y0 - yPixelCountForTick; y < y0 + yPixelCountForTick; ++y)
            {
                pixelData.pixelMatrix[y][x] = Color{ color }; 
            }
        }
    }
}

void PlotImage::drawYAxis(unsigned int x0, unsigned int y0, unsigned int yf, const ColorEnum& color)
{
    if (x0 < 0 || y0 < 0 || yf > imageHeight)
    {
        throw std::runtime_error("attempting to drawLine outside of imageWidth or imageHeight");
    }

    const int TICK_SPACING = plotHeight / 4;

    for (unsigned int y = y0; y < yf; ++y)
    {
        pixelData.pixelMatrix[y][x0] = Color{ color }; //note the "swapped" order of x and y here

        //display "tick" every 25% of the way: 
        if ((y - y0) % TICK_SPACING == 0)
        {
            //add extra vertical pixels:
            const int xPixelCountForTick = 10;

            for (int x = x0 - xPixelCountForTick; x < x0 + xPixelCountForTick; ++x)
            {
                pixelData.pixelMatrix[y][x] = Color{ color };
            }
        }
    }
}

void PlotImage::plotData(const map<int, int>& elementCountsToExecutionTimes, const ColorEnum& color)
{
    // Get the maximum element count (last key in the map) for SCALING 
    int maxElementCount = elementCountsToExecutionTimes.rbegin()->first;
    //also maxExecutionTime, for scaling (POSSIBLE that this won't be rbegin()->second (if "unlucky" algo)
    int maxExecutionTime = 0; 
    for (const auto& currentPair : elementCountsToExecutionTimes)
    {
        if (currentPair.second > maxExecutionTime)
        {
            maxExecutionTime = currentPair.second;
        }
    }

    int pairCounter = 0; 

    for (const auto& currentPair : elementCountsToExecutionTimes)
    {
        pairCounter++; 
        //int x = origin.first + currentPair.first / plotWidth;  
        //int y = origin.second + currentPair.second / plotHeight; 
        int x = origin.first + (currentPair.first * (plotWidth - origin.first)) / maxElementCount;
        int y = origin.second + (currentPair.second * (plotHeight - origin.second)) / maxExecutionTime;

        setPixelToColor_withThickness(x, y, Color{ color }, 3);

        //label the last point with text
        if (pairCounter == elementCountsToExecutionTimes.size())
        {
            int lastElementCount = elementCountsToExecutionTimes.rbegin()->first;
            int lastExecutionTime = elementCountsToExecutionTimes.rbegin()->second; 

            string lastElementCount_Text = convertIntToText(lastElementCount);
            
            displayText(lastElementCount_Text, x, PADDING - 16 * 2, ColorEnum::Red, 16);

            //display HALFWAY to max y value (don't want to mess up y-axis label):
            string lastExecutionTime_Text = convertIntToText(lastExecutionTime/2);

            displayText(lastExecutionTime_Text, PADDING/10, y/2, ColorEnum::Red, 16);
        }
    }
}

string convertIntToText(int coordinateComponent)
{
    //convert element count to scientific notation: 
    int exponent = 0;
    while (coordinateComponent > 9)
    {
        coordinateComponent = coordinateComponent / 10;
        exponent++;
    }

    string coordComp_string = to_string(coordinateComponent)
        + " X 10^" + to_string(exponent);

    return coordComp_string; 

}
