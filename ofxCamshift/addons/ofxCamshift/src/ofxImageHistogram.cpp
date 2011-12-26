#include "ofxImageHistogram.h"

ofxImageHistogram::ofxImageHistogram(unsigned char * aPixels, int nWidth, int nHeight) {
	width = nWidth;
	height = nHeight;
	
	ofPixels pixels;
	pixels.setFromPixels(aPixels, nWidth, nHeight, OF_IMAGE_COLOR);
	createBins(pixels);
}

float ofxImageHistogram::getBin(int nIndex) {
	return bins[nIndex];
}

void ofxImageHistogram::createBins(ofPixels oPixels) {
	int i,x,y;
	int r,g,b;
	int pixel;
	
	// init bins
	for(i=0; i<4096; ++i) {
		bins[i]=0.0;
	}
	for(y=0; y<height; ++y) {
		for(x=0; x<width; ++x) {
			pixel = oPixels.getColor(x, y).getHex();
			r = (pixel>>16 & 0xFF)/16;
			g = (pixel>>8 & 0xFF)/16;
			b = (pixel & 0xFF)/16;
			bins[256 * r + 16 * g +  b] +=  1.0;
		}
	}
}