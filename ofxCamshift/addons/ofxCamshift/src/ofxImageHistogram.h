#pragma once

#include "ofMain.h"

#define HIST_SIZE 4096;

class ofxImageHistogram {
public:
	ofxImageHistogram();
	ofxImageHistogram(unsigned char *aPixels, int nWidth, int nHeight);
	float getBin(int nIndex);
private:
	void createBins(ofPixels oPixels);
	float bins[4096];
	int width;
	int height;
};