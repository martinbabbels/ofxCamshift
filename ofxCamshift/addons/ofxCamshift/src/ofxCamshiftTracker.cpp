#include "ofxCamshiftTracker.h"

ofxCamshiftTracker::ofxCamshiftTracker() {
	trackObj.x			= 0;
	trackObj.y			= 0;
	trackObj.width		= 0;
	trackObj.height		= 0;
	trackObj.angle		= 0;
	
	searchWindow.x		= 0;
	searchWindow.y		= 0;
	searchWindow.width	= 0;
	searchWindow.height = 0;

}

ofRectangle ofxCamshiftTracker::getSearchWindow() {
	return searchWindow;
}

ofPixels ofxCamshiftTracker::getBackProjectionPixels() {
	return pdf;
}

void ofxCamshiftTracker::initTracker(unsigned char *aPixels, int nWidth, int nHeight, ofRectangle oRect) {
	ofPixels frame;
	frame.setFromPixels(aPixels, nWidth, nHeight, OF_IMAGE_COLOR);
	
	ofPixels cropped;
	cropped.allocate(oRect.width, oRect.height, OF_IMAGE_COLOR);
	
	ofPixelUtils::cropFromTo(frame, cropped, oRect.x, oRect.y, oRect.width, oRect.height);

	modelHist = new ofxImageHistogram(cropped.getPixels(), oRect.width, oRect.height);
	searchWindow.set(oRect);
}

void ofxCamshiftTracker::track(unsigned char* aPixels, int nWidth, int nHeight) {
	ofPixels frame;
	frame.setFromPixels(aPixels, nWidth, nHeight, OF_IMAGE_COLOR);
	
	camShift(frame, nWidth, nHeight);
}

TrackObj ofxCamshiftTracker::getTrackObj() {
	return trackObj;
}

void ofxCamshiftTracker::camShift(ofPixels oPixels, int nWidth, int nHeight) {
	// search location
	ofxCamshiftMoments *m = meanShift(oPixels, nWidth, nHeight);
	
	// use moments to find size and orientation
	float a = m->mu20 * m->invM00;
	float b = m->mu11 * m->invM00;
	float c = m->mu02 * m->invM00;
	float d = a + c;
	float e = sqrt((4*b * b) + ((a - c) * (a - c)));
	
	// check if tracked object is into the limit
	trackObj.x = MAX(0, MIN(searchWindow.x, nWidth));
	trackObj.y = MAX(0, MIN(searchWindow.y, nHeight));

	
	trackObj.width = ofClamp((sqrt((d - e)*0.5)*4)+trackObj.x, 0, nWidth) - trackObj.x;
	trackObj.height = ofClamp((sqrt((d + e)*0.5)*4)+trackObj.y, 0, nHeight) - trackObj.y;

	trackObj.angle = atan2(  2 * b, a - c + e );
	// to have a positive counter clockwise angle
	if (trackObj.angle < 0) trackObj.angle = trackObj.angle + PI;

	// new search window size
	searchWindow.width = 1.1 * trackObj.width;
	searchWindow.height = 1.1 * trackObj.height;
	
	
}

ofxCamshiftMoments* ofxCamshiftTracker::meanShift(ofPixels oPixels, int nWidth, int nHeight) {
	
	ofxCamshiftMoments *m;
	
	ofxImageHistogram *roiHist = new ofxImageHistogram(oPixels.getPixels(), nWidth, nHeight);
	float *weights = getWeights(modelHist, roiHist);
	
	// Color probabilities distributions
	pdf = getBackProjectionData(&oPixels, weights);

	int meanShiftIterations = 10;
	ofPixels windowAreaData;

	int x,y,i;
	
	// Locate by iteration the maximun of density into the probalities distribution
	for(i=0; i<meanShiftIterations; i++) {
		ofPixelUtils::cropFromTo(pdf, windowAreaData, searchWindow.x, searchWindow.y, searchWindow.width, searchWindow.height);
		m = new ofxCamshiftMoments(&windowAreaData, (i == meanShiftIterations -1));
		
		x = m->xc;
		y = m->yc;
		
		searchWindow.x += x - searchWindow.width/2;
		searchWindow.y += y - searchWindow.height/2; 
	}
	searchWindow.x = MAX(0, MIN(searchWindow.x, nWidth));
	searchWindow.y = MAX(0, MIN(searchWindow.y, nHeight));

	return m;
}

float* ofxCamshiftTracker::getWeights(ofxImageHistogram *mh, ofxImageHistogram *ch) {
	int i;
	float *weights = new float[4096];
	for(i=0; i<4096; i++) {
		weights[i] = (ch->getBin(i) != 0) ? MIN(mh->getBin(i)/ch->getBin(i),1.0):0.0;;
	}
	
	return weights;
}

ofPixels ofxCamshiftTracker::getBackProjectionData(ofPixels *oPixels, float *aWeights) {
	ofPixels data;
	data.allocate(oPixels->getWidth(), oPixels->getHeight(), OF_IMAGE_COLOR);
	
	int		x,y,r,g,b;
	int		pixel;
	ofColor color;
	
	for(x=0; x<oPixels->getWidth(); ++x) {
		for(y=0; y<oPixels->getHeight(); ++y) {
			pixel = oPixels->getColor(x, y).getHex();
			r = (pixel>>16 & 0xFF)/16;
			g = (pixel>>8 & 0xFF)/16;
			b = (pixel & 0xFF)/16;
			
			float c = aWeights[256 * r + 16 * g +  b];
			color.r = floor(c*255);
			color.g = color.r;
			color.b = color.r;
			
			data.setColor(x, y, color);
		}
	}
	return data;
}