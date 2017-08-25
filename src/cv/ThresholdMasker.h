
#pragma once

#include "ofMain.h"
#include "ofxCv.h"

namespace np{

class ThresholdMasker {

public:    
    void setup (int w, int h);
    void setup( int w, int h, int destW, int destH);
    
    void beginCanvas();
    void endCanvas();
    
    void update( const ofPixels & pixels );
    void draw( int x, int y );
    
    ofParameterGroup ui;
	ofImage tImage;
            
private:
    ofFbo canvas;
    ofFbo mask;
    
    ofParameter<int> t;

    static ofShader masker;
    static bool firstConstruction;
    
    bool bResize;
    
};

}
