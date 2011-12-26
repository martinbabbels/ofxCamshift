#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	grabber.initGrabber(320, 240);
	
	// Grabber position
	position.x = 10;
	position.y = 20;
	
	// Set the position of the video
	camShift.setPosition(position);
	
	// Scale down the image to improve performance
	camShift.setScale(.7);
}

//--------------------------------------------------------------
void testApp::update(){
	// Grab camera frame
	grabber.grabFrame();
	
	// Update camshift
	camShift.update(grabber.getPixels(), grabber.getWidth(), grabber.getHeight());
	
	// Show FPS
	FPS();
}

//--------------------------------------------------------------
void testApp::FPS() {
	string fps = ofToString(round(ofGetFrameRate()));
	fps.append(" FPS");
	ofSetWindowTitle(fps);
}

//--------------------------------------------------------------
bool testApp::withinVidBounds(int x, int y) {
	bool within_bounds_x = (x >= position.x && x <= position.x+grabber.getWidth());
	bool within_bounds_y = (y >= position.y && y <= position.y+grabber.getHeight());
	
	return (within_bounds_x && within_bounds_y);
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0,0,0);
	ofDrawBitmapString("Webcam", ofPoint(10,10));
	
	ofSetColor(255,255,255);
	// Draw camera
	grabber.draw(position.x,position.y);
	
	if(camShift.isTracking()) {
		// Draw back projection image
		ofSetColor(0,0,0);
		ofDrawBitmapString("Back projection", ofPoint(position.x+grabber.getWidth()+20,10));
		ofSetColor(255,255,255);
		camShift.drawBackProjection(position.x+grabber.getWidth()+20, position.y);
		
		// Draw tracked object
		camShift.drawTrackObj();
	}
	
	ofSetColor(255,255,255);
	
	if(isMouseDown) {
		// Draw zone selector
		camShift.drawZoneSelector();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(!withinVidBounds(x,y)) return;
	
	camShift.onSelectUpdate(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(!withinVidBounds(x,y)) return;
	
	// Set selected zone
	camShift.onStartSelect(x,y);
	isMouseDown = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(!withinVidBounds(x,y) || camShift.isTracking()) return;
	
	// Start tracking
	camShift.track(grabber.getPixels(), grabber.getWidth(), grabber.getHeight());
	isMouseDown = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}