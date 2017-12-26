
#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "np-cvthings.h"

class ofApp : public ofBaseApp, public np::ContoursOperator {

public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
    void exit();
    
    np::ThreadedBuffer          buffer;
	np::LibreKGraySource		kGray;

    ofxPanel    gui;
    bool        bDrawGui;

	ofFbo fbo;

};
