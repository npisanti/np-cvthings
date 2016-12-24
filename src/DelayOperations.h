
#pragma once

#include "ofMain.h"
#include "ContoursBuffer.h"

namespace np{

class DelayOperations {

public:

    DelayOperations();

    void update( const ContoursBuffer & cBuffer, int num, int delay );

    ofParameterGroup ui;
    
    vector<vector<ofPolyline>> buffer;

    const size_t size() const;

    ofParameter<bool>   bExpand;
    ofParameter<int>    expand;

private:
    size_t              t;
    
    ofParameter<bool>   bSmoothPre;
    ofParameter<int>    smoothPre;
    ofParameter<int>    smoothPreMod;

    ofParameter<float>  smoothMode;
    
    ofParameter<int>    expandMod;
    ofParameter<bool>   bExpandMode;

    ofParameter<bool>   bSmoothPost;
    ofParameter<int>    smoothPost;

    ofParameter<bool>   bJitter;
    ofParameter<int>    jitX;
    ofParameter<int>    jitY;
    ofParameter<int>    jitXMod;
    ofParameter<int>    jitYMod;
    
    static int number;
};

}
