#pragma once

#include "ofMain.h"

class ofxCamshiftMoments {
public:
	ofxCamshiftMoments();
	ofxCamshiftMoments(ofPixels *oPixels, bool bSecond);
	
	float m00;
	float m10;
	float m01;
	float m11;
	float m20;
	float m02;
	float mu00;
	float mu10;
	float mu01;
	float mu11;
	float mu20;
	float mu02;
	float invM00;
	float xc;
	float yc;
	 
};