#pragma once

#include "ofMain.h"

class ofxZoneSelector {
public:
	ofxZoneSelector();
	void startSelect(int iMouseX, int iMouseY);
	void update(int iMouseX, int iMouseY);
	void draw();
	void setPosition(ofPoint p);
	ofPoint getPosition();
	ofRectangle getZone();
	
private:
	ofRectangle selectedArea;
	ofPoint position;
};