#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "np-cvthings.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    
    
    ofxKinect kinect;
	int angle;
    
    // contours
    ofxPanel gui;

    ofParameter<int>    delayControl;

    np::ThresholdMasker tMasker;
    np::Contours        cFinder;
    np::ContoursBuffer  cBuffer;
    
    bool                bDrawGui;
    
};
