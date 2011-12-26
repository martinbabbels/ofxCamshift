#include "ofxCamshift.h"
#include "ofxCamshiftTracker.h"

ofxCamshift::ofxCamshift() {
	zoneSelector = new ofxZoneSelector();
	tracker		 = new ofxCamshiftTracker();
	scale		 = .5;
	camWidth	 = 0;
	camHeight	 = 0;

}

void ofxCamshift::initTracker(unsigned char * aPixels, int nWidth, int nHeight) {
	ofImage scaled;
	scaled.setFromPixels(aPixels, nWidth, nHeight, OF_IMAGE_COLOR);
	
	camWidth  = nWidth;
	camHeight = nHeight;
	
	float x_scaled = floor(nWidth*scale);
	float y_scaled = floor(nHeight*scale);
	
	ofRectangle rect_scaled = zoneSelector->getZone();
	
	if(scale<1) {
		scaled.resize(x_scaled, y_scaled);
		rect_scaled.x = floor(rect_scaled.x*scale);
		rect_scaled.y = floor(rect_scaled.y*scale);
		rect_scaled.width = floor(rect_scaled.width*scale);
		rect_scaled.height = floor(rect_scaled.height*scale);
	}
	
	targetImg.loadImage("target.png");
	targetImg.setAnchorPercent(.5,.5);
	
	tracker->initTracker(scaled.getPixels(), x_scaled, y_scaled, rect_scaled);
}

bool ofxCamshift::isTracking() {
	return (state==OFX_CAMSHIFT_TRACK);
}

int ofxCamshift::getState() {
	return state;
}

void ofxCamshift::setScale(float fScale) {
	scale = fScale;
}

void ofxCamshift::setPosition(ofPoint p) {
	zoneSelector->setPosition(p);
}

void ofxCamshift::drawSearchWindow() {
	ofRectangle window = tracker->getSearchWindow();
	float s = 1/scale;
	window.x *= s;
	window.y *= s;
	window.width *= s;
	window.height *= s;
	
	window.x += zoneSelector->getPosition().x;
	window.y += zoneSelector->getPosition().y;

	ofSetColor(255,0,0,255);
	ofDrawBitmapString("Search area", ofPoint(window.x, window.y-10));
	ofLine(window.x, window.y, window.x+window.width, window.y);
	ofLine(window.x+window.width, window.y, window.x+window.width, window.y+window.height);
	ofLine(window.x, window.y+window.height, window.x+window.width, window.y+window.height);
	ofLine(window.x, window.y+window.height, window.x, window.y);
	ofSetColor(255,255,255);
}

void ofxCamshift::drawTrackObj() {
	float s = 1/scale;
	float w = tracker->getTrackObj().width * s;
	float h = tracker->getTrackObj().height * s;
	float x = tracker->getTrackObj().x * s;
	float y = tracker->getTrackObj().y * s;
	
	x += zoneSelector->getPosition().x;
	y += zoneSelector->getPosition().y;
	
	if(x>=camWidth && y>=camHeight) return;
	
	ofSetColor(255,0,0);
	ofDrawBitmapString("Tracked area", ofPoint(x, y-10));
	ofPushMatrix();
		ofLine(x, y, x+w, y);
		ofLine(x+w, y, x+w, y+h);
		ofLine(x, y+h, x+w, y+h);
		ofLine(x, y+h, x, y);
	ofPopMatrix();
	
	float angle = (tracker->getTrackObj().angle * 180 / PI)-90;
	float half_h = h/2;
	float half_w = w/2;
	
	ofPushMatrix();
		ofTranslate(x+half_w, y+half_h);
		ofRotate(angle);
		ofSetColor(255,255,0);
		ofLine(-half_w, 0, half_w, 0);
		ofSetColor(0,0,255);
		ofLine(0, -half_h, 0, half_h);
	ofPopMatrix();
	
	ofSetColor(255,255,255);
}

void ofxCamshift::track(unsigned char * aPixels, int nWidth, int nHeight) {
	if (zoneSelector->getZone().width != 0 && zoneSelector->getZone().height != 0) {
		state = OFX_CAMSHIFT_TRACK;
		initTracker(aPixels, nWidth, nHeight);	
	}
	else state = OFX_CAMSHIFT_WAIT;	
}

void ofxCamshift::drawBackProjection(int x, int y) {
	ofPixels bpp = tracker->getBackProjectionPixels();
	backProj.setFromPixels(bpp.getPixels(), bpp.getWidth(), bpp.getHeight(), OF_IMAGE_COLOR);
	backProj.draw(x,y,camWidth,camHeight);
}

void ofxCamshift::update(unsigned char * aPixels, int nWidth, int nHeight) {
	if(state == OFX_CAMSHIFT_TRACK) {
		ofImage scaled;
		scaled.setFromPixels(aPixels, nWidth, nHeight, OF_IMAGE_COLOR);
		float x_scaled = floor(nWidth*scale);
		float y_scaled = floor(nHeight*scale);
		
		if(scale<1) {
			scaled.resize(x_scaled, y_scaled);
		}
		
		// camshift computed at each frame
		tracker->track(scaled.getPixels(), x_scaled, y_scaled);
	}
}
void ofxCamshift::drawZoneSelector() {
	zoneSelector->draw();
}

void ofxCamshift::onStartSelect(int iMouseX, int iMouseY) {
	state = OFX_CAMSHIFT_WINDOW;
	zoneSelector->startSelect(iMouseX,iMouseY);
}

void ofxCamshift::onSelectUpdate(int iMouseX, int iMouseY) {
	if(state == OFX_CAMSHIFT_WINDOW) {
		zoneSelector->update(iMouseX,iMouseY);
	}
}

void ofxCamshift::setZone(ofRectangle oRect) {
	zoneSelector->startSelect(oRect.x,oRect.y);
	zoneSelector->update(oRect.x+oRect.width,oRect.y+oRect.height);
}
