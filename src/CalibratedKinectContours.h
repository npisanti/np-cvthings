
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxKinectProjectorToolkit.h"
#include <atomic>

namespace np{

class CalibratedKinectContours : public ofThread {

public:

    CalibratedKinectContours();
    void setup( int projectorW, int projectorH, string calibrationXmlPath, int msec=5 );
    void exit();
    
    void threadedFunction();

    ofParameterGroup            ui;
    bool                        hasNewContours();
    const vector<ofPolyline> &  getContours();

    void drawTestingImage( int x, int y );

private:

    ofxKinect                   kinect;    
    ofxKinectProjectorToolkit   kpt;
    
    ofxCvGrayscaleImage         bgImage;
    ofxCvGrayscaleImage         grayImage;
    ofxCvGrayscaleImage         grayThreshNear;
    ofxCvGrayscaleImage         grayThreshFar;

    ofxCv::ContourFinder        contourFinder;

    ofParameter<float>          nearThreshold;
    ofParameter<float>          farThreshold;
    ofParameter<float>          minArea;
    ofParameter<float>          maxArea;
    ofParameter<float>          threshold;
    ofParameter<float>          persistence;
    ofParameter<float>          maxDistance;
    
    vector<ofPolyline>          contours;
    vector<ofPolyline>          toReturn;
    std::mutex                  contoursMutex;
    
    int projectorH;
    int projectorW;
    int msec;
    
    atomic<bool>                bNew;
};

}
