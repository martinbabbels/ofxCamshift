#pragma once

#include "ofMain.h"
#include "ofxCamshiftTracker.h"
#include "ofxZoneSelector.h"

enum OFX_CAMSHIFT_STATE {
	 OFX_CAMSHIFT_WAIT = 0
	,OFX_CAMSHIFT_TRACK = 1
	,OFX_CAMSHIFT_WINDOW = 2
};

class ofxCamshift {
public:
	ofxCamshift();
	void setup();
	void update(unsigned char *aPixels, int nWidth, int nHeight);
	void track(unsigned char * aPixels, int nWidth, int nHeight);
	
	void onStartSelect(int iMouseX, int iMouseY);
	void onSelectUpdate(int iMouseX, int iMouseY);
	void setZone(ofRectangle oRect);
	
	void drawBackProjection(int x, int y);
	void drawTrackObj();
	void drawSearchWindow();
	void drawZoneSelector();
	
	bool isTracking();
	int getState();
	
	void setPosition(ofPoint p);
	void setScale(float fScale);
	
	ofxZoneSelector *zoneSelector;
	ofxCamshiftTracker *tracker;
	
	
	
private:
	void initZoneSelector();
	void initTracker(unsigned char * aPixels, int nWidth, int nHeight);
	
	ofPixels curFrame;
	ofPixels *pixels;
	ofImage backProj;
	ofImage targetImg;
	
	int state;
	int camWidth;
	int camHeight;
	
	float scale;
	
	
	
};