#include "ofApp.h"

void ofApp::setup() {
    ofSetVerticalSync(true);
    ofSetWindowShape(1280, 960);
    
    blobColors[0] = ofColor(255, 0, 0);
    blobColors[1] = ofColor(0, 255, 0);
    blobColors[2] = ofColor(0, 0, 255);
    blobColors[3] = ofColor(255, 255, 0);
    blobColors[4] = ofColor(255, 0, 255);
    blobColors[5] = ofColor(0, 255, 255);
    blobColors[6] = ofColor(255, 127, 0);
    blobColors[7] = ofColor(127, 0, 255);
    blobColors[8] = ofColor(0, 255, 127);
    blobColors[9] = ofColor(127, 255, 0);
    blobColors[10]= ofColor(255, 0, 127);
    blobColors[11]= ofColor(0, 127, 255);
    
    krepro.setup( PROJECTOR_RESOLUTION_X, PROJECTOR_RESOLUTION_Y,
            "/home/nick/oF/addons/ofxKinectProjectorToolkit/calibration/bin/data/calibration.xml", 250 );

    gui.setup("parameters");    
    gui.add( krepro.ui );
    
}

void ofApp::update() {
}

void ofApp::draw() {

    // GUI
    ofBackground(0);
    ofSetColor(255);

    ofPushMatrix();
        //krepro.kinect.draw(0, 0);
        //ofTranslate(640, 0);
        //krepro.grayImage.draw(0, 0);
        //ofTranslate(-640, 480);
        //krepro.contourFinder.draw();
        //ofTranslate(640, 0);
    ofPopMatrix();
    
    gui.draw();
    
}

void ofApp::drawSecondWindow(ofEventArgs &args){
    
    ofBackground(0);
    
    vector<ofPolyline> contours = krepro.getContours();
    
    for(int i = 0; i < contours.size(); i++) {
        ofSetColor(blobColors[i % 12]);
        contours[i].draw();
    }
    
}

void ofApp::exit() {
    krepro.exit();
}

void ofApp::keyPressed(int key) {
    
}
