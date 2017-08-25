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
    void exit();
    

    void fillGraphics();
    
    const ofColor & getLabelColor( const np::CvContour & cont ) const;

    np::KinectContoursBuffer    kBuffer;
    np::BufferedOperations      ops;
    np::AlphaMasker             masker;
    
    ofParameter<int>            delayFrames;
    ofParameter<int>            numDelays;
    
    ofParameterGroup            graphics;
        ofParameter<bool>           bDelayFill;

        ofParameter<int>            contoursAlphaStart;
        ofParameter<int>            fillAlphaStart;
        ofParameter<bool>           bBlackNow;
        ofParameter<bool>           bDrawContours;
        ofParameter<bool>           bDrawFirstContour;
        ofParameter<bool>           colorizeBackground;
        ofParameter<float>          backgroundAlpha;
        
    ofxPanel    gui;
    bool        bDrawGui;
    
    int     angle;
    int     alphaStep;
    
    vector<ofColor> colors;
    
    ofFbo   fbo;
    
};
