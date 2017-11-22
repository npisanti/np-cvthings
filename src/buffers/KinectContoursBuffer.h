
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include <atomic>
#include "np-junkrepo.h"
#include "ofxKinectProjectorToolkit.h"

namespace np{

class KinectContoursBuffer : public ofThread {

public:
    KinectContoursBuffer();
    
    void setup( int bufferSize );
    void setup( int bufferSize, int projectorW, int projectorH );
    void setup( int bufferSize, int projectorW, int projectorH, string calibrationXmlPath  );
    
    void exit();
    
    bool                        update();
    const vector<CvContour> &   delay(int i, int frames=1) const; 
    const vector<CvContour> &   now() const;
        
    ofParameterGroup            ui;
    ofxKinect                   kinect;   
    void                        drawTestingImage( int x, int y );

    const ofImage &             difference() const;      
    const vector<float> &       differenceCols() const;      
    int                         getWidth();    
    
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
    ofParameter<bool>           bMirror;
     
    std::mutex                  contoursMutex;
    vector<vector<CvContour>>   buffer;
    
    int                         writeIndex; 
    int                         readIndex; 
    int                         bufferSize;     
       
    int                         projectorW;
    int                         projectorH;
    int                         projectorOff;
    int                         projectorX0;
    int                         projectorX1;
    void                        calculateAspect( int w, int h );
    
    int                         msec;

    vector<ofImage>             differenceBuffer;
    vector<vector<float>>       differenceColumns;
    ofPixels                    previous;
    static const int            framediffMax = 5;
    int                         diffRead;
    int                         diffWrite;
    cv::Mat                     columnMean;    

    int                         translateMode;
    ofxKinectProjectorToolkit   kpt;
    
};

}
