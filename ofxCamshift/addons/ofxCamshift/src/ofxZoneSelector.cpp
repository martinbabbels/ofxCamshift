#include "ofxZoneSelector.h"

ofxZoneSelector::ofxZoneSelector() {
}
		
ofRectangle ofxZoneSelector::getZone() {
	ofRectangle sa;
	sa.set(selectedArea);
	
	sa.x += position.x;
	sa.y += position.y;
	
	return sa;
}
		
void ofxZoneSelector::startSelect(int iMouseX, int iMouseY) {
	selectedArea.x = iMouseX;
	selectedArea.y = iMouseY;
	selectedArea.width = 0;
	selectedArea.height = 0;	
}

void ofxZoneSelector::update(int iMouseX, int iMouseY){
	selectedArea.width = MAX(0, iMouseX - selectedArea.x);
	selectedArea.height = MAX(0, iMouseY - selectedArea.y);
}

void ofxZoneSelector::setPosition(ofPoint p) {
	position = p;
}

ofPoint ofxZoneSelector::getPosition() {
	return position;
}

void ofxZoneSelector::draw() {
	ofEnableAlphaBlending();
	ofSetColor(255,255,255,100);
	ofRect(selectedArea);
	ofSetColor(255,255,255,255);
	ofDisableAlphaBlending();
}

