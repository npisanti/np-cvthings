
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxGui.h"
#include "np-cvthings.h"


#define PROJECTOR_RESOLUTION_X 720
#define PROJECTOR_RESOLUTION_Y 480

class ofApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();
    
    void drawSecondWindow(ofEventArgs& args);
    void exit();
    
    void keyPressed(int key);
    
    ofxPanel                    gui;
    
    np::KinectContoursBuffer krepro;
        
    vector<np::CvContour> contours;
    
    ofParameter<bool> bAlphaTest;
    
};
