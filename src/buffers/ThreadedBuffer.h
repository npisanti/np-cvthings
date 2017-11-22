
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include <atomic>
#include "np-junkrepo.h"
#include "ofxKinectProjectorToolkit.h"
#include "GrayImageSource.h"
#include "ofxHomography.h"

namespace np{

class ThreadedBuffer : public ofThread {

public:
    ThreadedBuffer();
    
    void setup( int bufferSize, GrayImageSource & gray );
    void setup( int bufferSize, GrayImageSource & gray, int projectorW, int projectorH );
    void setup( int bufferSize, GrayImageSource & gray, ofMatrix4x4 & homography );
    
    void exit();
    
    bool                        update();
    const vector<CvContour> &   delay( int frames, int i=1 ) const; 
    const vector<CvContour> &   now() const;
    
    ofParameterGroup            ui;
    void                        drawTestingImage( int x, int y );

    const ofImage &             difference() const;      
    const vector<float> &       differenceCols() const;      
    int                         getWidth();    
    
private:
	ofMatrix4x4 * hMatrix;

    void threadedFunction();
    
    GrayImageSource * 			gray;
    
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
    
};

}
