
#pragma once

#include "ofMain.h"
#include "ofxCv.h"

namespace np{

class Difference{

public:
    void setup( int w, int h );
    
    void update( ofPixels & pixels );

    void draw( int x, int y );
    void draw( int x, int y, int w, int h );

    ofParameterGroup ui;

    ofFbo       canvas;
    ofImage     image;
    
private:
    ofParameter<bool> bTrails;
    ofParameter<int> trails;

    ofPixels previous;
    
    ofFbo       mask;
    ofShader    masker;
};

}
