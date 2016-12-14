
#pragma once

#include "ofMain.h"

namespace np{

class ContoursOperations {

public:
    ContoursOperations();

    void update( const vector<ofPolyline> & newContours );

    ofParameterGroup ui;
    
    vector<ofPolyline> contours;

    
private:
    
    ofParameter<bool>   bSmoothPre;
    ofParameter<int>    smoothPre;

    ofParameter<float>  smoothMode;
    
    ofParameter<bool>   bExpand;
    ofParameter<int>    expand;
    ofParameter<bool>   bExpandMode;

    ofParameter<bool>   bSmoothPost;
    ofParameter<int>    smoothPost;


    static int number;
};

}
