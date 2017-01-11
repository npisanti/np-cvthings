
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include <atomic>
#include "CvContour.h"

namespace np{

class KinectContours : public ofThread {

public:

    KinectContours();
    void setup( int screenW=640, int screenH=480 );
    void exit();
    
    void threadedFunction();

    ofParameterGroup            ui;
    bool                        hasNewContours();
    const vector<CvContour> &   getContours();

    void drawTestingImage( int x, int y );

    ofxKinect                   kinect;   


private:

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
    ofParameter<bool>           bHoles;
    
    
    vector<vector<CvContour>>   circularBuffer;
    size_t                      circularIndex;
    size_t                      circularMax;

    std::mutex                  contoursMutex;
    
    int screenW;
    int screenH;
    int msec;
    
    bool                        bExpand;
    atomic<bool>                bNew;
};

}
