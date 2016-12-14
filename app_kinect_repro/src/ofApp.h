
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxGui.h"
#include "np-cvthings.h"


#define PROJECTOR_RESOLUTION_X 1024
#define PROJECTOR_RESOLUTION_Y 768

class ofApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();
    
    void drawSecondWindow(ofEventArgs& args);
    void exit();
    
    void keyPressed(int key);
    
    ofxPanel                    gui;
    ofColor                     blobColors[12];
    
    np::CalibratedKinectContours krepro;
    
};
