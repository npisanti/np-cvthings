
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include <atomic>
#include "CvContour.h"

namespace np{

class KinectContoursBuffer : public ofThread {

public:
    KinectContoursBuffer();
    
    void setup( int bufferSize, int screenW=640, int screenH=480 );
    void exit();
    
    bool                        update();
    const vector<CvContour> &   delay(int i, int frames=1) const; 
    const vector<CvContour> &   now() const;
        
    ofParameterGroup            ui;
    ofxKinect                   kinect;   
    void                        drawTestingImage( int x, int y );

private:
    void threadedFunction();
    
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
    
    ofParameter<bool>           bSimplify;
    ofParameter<float>          simplifyAmount;
     
    std::mutex                  contoursMutex;
    vector<vector<CvContour>>   buffer;
    
    int                         writeIndex; 
    int                         readIndex; 
    int                         bufferSize;     
    
    bool                        bExpand;     
    int                         screenW;
    int                         screenH;
    int                         msec;
};

}
