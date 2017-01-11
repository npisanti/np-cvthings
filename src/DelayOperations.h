
#pragma once

#include "ofMain.h"
#include "ContoursBuffer.h"
#include "CvContour.h"

namespace np{

class DelayOperations {

public:
    DelayOperations();

    void update( const ContoursBuffer & cBuffer );
    
    vector<vector<CvContour>>  buffer;
    const size_t size()         const;

    ofParameterGroup    ui;
        ofParameter<int>    delayFrames;
        ofParameter<int>    numDelays;
        
        ofParameter<bool>   bSmoothPre;
        ofParameter<int>    smoothPre;
        ofParameter<int>    smoothPreMod;

        ofParameter<float>  smoothMode;

        ofParameter<bool>   bExpand;
        ofParameter<int>    expand;    
        ofParameter<int>    expandMod;
        ofParameter<bool>   bExpandMode;

        ofParameter<bool>   bSmoothPost;
        ofParameter<int>    smoothPost;

        ofParameter<bool>   bJitter;
        ofParameter<int>    jitX;
        ofParameter<int>    jitY;
        ofParameter<int>    jitXMod;
        ofParameter<int>    jitYMod;
    
private:
    size_t      t;
    static int  number;

};

}
