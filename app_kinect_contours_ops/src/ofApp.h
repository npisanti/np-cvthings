#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "np-cvthings.h"
#include "np-junkrepo.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    void exit();
    
    void drawPolylines( const vector<np::CvContour> & contours, int alpha );
    void drawPolyline( const ofPolyline & line );

    np::KinectContours      kContours;
    np::ContoursBuffer      buffer;
    np::DelayOperations     delayop;  
    np::AlphaMasker         masker;
    
    ofParameterGroup            graphics;
        ofParameter<bool>           bDelayFill;
        ofParameter<int>            numDelays;
        ofParameter<int>            contoursAlphaStart;
        ofParameter<int>            fillAlphaStart;
        ofParameter<bool>           bBlackNow;
        ofParameter<bool>           bDrawContours;
        ofParameter<bool>           bDrawFirstContour;
        ofParameter<bool>           colorizeBackground;
        ofParameter<int>            backgroundAlpha;
        
    ofxPanel    gui;
    bool        bDrawGui;
    
    int     angle;
    int     alphaStep;
    
    vector<ofColor> colors;
    
};
