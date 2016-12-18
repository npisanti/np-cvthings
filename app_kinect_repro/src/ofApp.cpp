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
            "/home/nick/oF/addons/ofxKinectProjectorToolkit/calibration/bin/data/calibration.xml", 5 );

    gui.setup("parameters", "settings.xml", 1040, 20);    
    gui.add( krepro.ui );
    gui.add( bAlphaTest.set("kinect testing img", true) );
    gui.loadFromFile( "settings.xml" );

}

void ofApp::update() {
    ofSetWindowTitle("kinect repro test, fps=" + ofToString( ofGetFrameRate()));
    if(krepro.hasNewContours()) contours = krepro.getContours();
}

void ofApp::draw() {

    // GUI
    ofBackground(0);

    if(bAlphaTest){
        ofSetColor(255);
        krepro.drawTestingImage( 0, 0 );        
    }

    for(int i = 0; i < contours.size(); i++) {
        ofSetColor(blobColors[i % 12]);
        contours[i].draw();
    }
    
    gui.draw();
    
}

void ofApp::drawSecondWindow(ofEventArgs &args){
    
    ofBackground(0);

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

