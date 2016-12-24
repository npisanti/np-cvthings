
#pragma once

#include "ofMain.h"
#include "ofxCv.h"

namespace np{

class Contours {

public:
    void setup (int w, int h);
    void setup( int w, int h, int destW, int destH);

    void update( ofImage & tImage );
    void draw( int x, int y );

    ofParameterGroup ui;
    
    ofParameter<bool> bOuterContour;    
    vector<ofPolyline> contours;
    vector<ofPolyline> outer;
    
private:
    ofxCv::ContourFinder contourFinder;
    ofParameter<float> minArea, maxArea;
    ofParameter<int> smooth;
    ofParameter<int> smoothOuter;
    ofParameter<float> smoothMode;

    ofParameter<bool> bSmoothInner;
    ofParameter<bool> bContourMode;
    ofParameter<int> contoursSeparation;
    ofParameter<bool> bFindHoles;

    float scaleX;
    float scaleY;
    bool bScale;
    
};

}
