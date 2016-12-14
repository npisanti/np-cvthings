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
    
    void canvasRoutine();
    
    ofxKinect kinect;
	int angle;
    
    // contours
    ofxPanel gui;
    
    np::ThresholdMasker tMasker;
    np::Contours        cFinder;
    np::Difference      difference;
    
    int selector;
    
    ofShader masker;
    
    bool                bDrawGui;
    
    cv::Mat columnMean;
    
};
