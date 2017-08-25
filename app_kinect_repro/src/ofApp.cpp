#include "ofApp.h"

void ofApp::setup() {
    ofSetVerticalSync(true);
    ofSetWindowShape(1280, 800);
    
    krepro.setup( 512,  PROJECTOR_RESOLUTION_X, PROJECTOR_RESOLUTION_Y,
            "/home/nick/oF/addons/ofxKinectProjectorToolkit/calibration/bin/data/calibration.xml");

    gui.setup("parameters", "settings.xml", 1040, 20);    
    gui.add( krepro.ui );
    gui.add( bAlphaTest.set("kinect testing img", true) );
    gui.loadFromFile( "settings.xml" );

}

void ofApp::update() {
    ofSetWindowTitle("kinect repro test, fps=" + ofToString( ofGetFrameRate()));
}

void ofApp::draw() {

    // GUI
    ofBackground(0);

    if(bAlphaTest){
        ofSetColor(255);
        krepro.drawTestingImage( 0, 0 );        
    }

    ofSetColor(0, 255, 255);
    for(int i = 0; i < krepro.now().size(); i++) {
        krepro.now()[i].draw();
    }
    
    gui.draw();
}

void ofApp::drawSecondWindow(ofEventArgs &args){
    
    ofBackground(0);
    
    ofFill();
    ofSetColor(0, 255, 255);
    
    for(int i = 0; i < krepro.now().size(); i++) {
        krepro.now()[i].draw();
    }
    
}

void ofApp::exit() {
    krepro.exit();
}

void ofApp::keyPressed(int key) {
    
}

