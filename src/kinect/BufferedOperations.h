
#pragma once

#include "ofMain.h"
#include "np-junkrepo.h"

namespace np{

// this class should be used in the ofApp update() or draw() cycle
// as it use ofRandom() that is not thread safe

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
        ofParameter<bool>   bFullJitter;
        ofParameter<int>    jitX;
        ofParameter<int>    jitY;
        ofParameter<int>    jitXMod;
        ofParameter<int>    jitYMod;
    
private:
    static int  number;

};

}
