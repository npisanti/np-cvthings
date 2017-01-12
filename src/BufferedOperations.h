
#pragma once

#include "ofMain.h"
#include "ContoursBuffer.h"
#include "CvContour.h"

namespace np{

class BufferedOperations {

public:
    BufferedOperations();

    void update( int index, const vector<CvContour> contours );
    
    vector<vector<CvContour>>   buffer;
    const size_t                size() const;

    ofParameterGroup    ui;        
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
    static int  number;

};

}
