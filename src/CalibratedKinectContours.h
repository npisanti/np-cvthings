
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxKinectProjectorToolkit.h"

namespace np{

class CalibratedKinectContours : public ofThread {

public:

    CalibratedKinectContours();
    void setup( int projectorW, int projectorH, string calibrationXmlPath, int usec=250 );
    void exit();
    
    void threadedFunction();

    ofParameterGroup    ui;
    ofxKinect           kinect;
    
    const vector<ofPolyline> & getContours();
    
private:

    ofxCv::ContourFinder        contourFinder;
    ofxKinectProjectorToolkit   kpt;

    ofxCvGrayscaleImage         bgImage;
    ofxCvGrayscaleImage         grayImage;
    ofxCvGrayscaleImage         grayThreshNear;
    ofxCvGrayscaleImage         grayThreshFar;

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
    int usec;
};

}
