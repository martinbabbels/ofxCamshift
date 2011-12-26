#pragma once

#include "ofMain.h"
#include "ofxImageHistogram.h"
#include "ofxCamshiftMoments.h"
#include "ofPixelUtils.h"

#define HIST_SIZE 4096;
typedef struct {
	float width;
	float height;
	float angle;
	float x;
	float y;
}TrackObj;

class ofxCamshiftTracker {
public:
	ofxCamshiftTracker();
	ofRectangle getSearchWindow();
	TrackObj getTrackObj();
	ofPixels getBackProjectionPixels();
	void initTracker(unsigned char *aPixels, int nWidth, int nHeight, ofRectangle oRect);
	void track(unsigned char* aPixels, int nWidth, int nHeight);
	
private:
	void camShift(ofPixels oPixels, int nWidth, int nHeight);
	ofxCamshiftMoments *meanShift(ofPixels oPixels, int nWidth, int nHeight);
	ofPixels getBackProjectionData(ofPixels *oPixels, float *aWeights);
	float* getWeights(ofxImageHistogram *oMh, ofxImageHistogram *ch);
	ofxImageHistogram* modelHist;
	ofxImageHistogram* curHist;
	ofxImageHistogram* trackHist;
	
	ofRectangle trackArea;
	ofRectangle searchWindow;
	ofPixels pdf;
	ofImage croppedImage;
	TrackObj trackObj;
	
};
