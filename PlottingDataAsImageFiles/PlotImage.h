#pragma once
#include "BMPImage.h"
#include"Font.h"
#include"AlgoAnalysis.h"

#include<string> 

using std::pair, std::to_string; 

class PlotImage :public BMPImage
{
 

	/*Alters bitmap member var*/
	//void setFont();

	unsigned int imageHeight{}, imageWidth{};

	unsigned int plotWidth;// = imageWidth - 100; //100 is arbitary padding 
	unsigned int plotHeight;// = imageHeight - 100;

	const unsigned int PADDING = 100; 

	pair<int, int> origin = { PADDING, PADDING };
	

	/*for displaying text on plot*/
	Bitmap fontBitmap;

	void drawXAxis(unsigned int x0, unsigned int y0, unsigned int xf, const ColorEnum& color);
	void drawYAxis(unsigned int x0, unsigned int y0, unsigned int yf, const ColorEnum& color);


public: 
	PlotImage() = delete;
	PlotImage(unsigned int imageWidth, unsigned int imageHeight, const Color& fillColor);

	void plotData(const map<int, int>& elementCountsToExecutionTimes, const ColorEnum& color);

	void displayText(const string& textToRender, unsigned int xOffset, unsigned int yOffset, const ColorEnum& color, int fontSize);

	/*
	@param bestFitEqn will be in the form y = ??X + ??"
	*/
	void drawLineOfBestFit(pair<double, double> slopeAndYIntercept, const map<int, int>& elementCountsToExecutionTimes, const ColorEnum& color);
};

/****************************************"Auxillary method(s)*************************/

/*Converts an integer (ex the execution time for an algo) to text AND scientific notation */
string convertIntToText(int coordinateComponent);



