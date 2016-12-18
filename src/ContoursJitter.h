
#pragma once

#include "ofMain.h"

namespace np{

class ContoursJitter {

public:

    ContoursJitter();

    void update( const vector<ofPolyline> & newContours );

    ofParameterGroup ui;
    
    vector<ofPolyline> contours;

private:
    
    ofParameter<bool>   bActivate;
    ofParameter<int>    jitter;


    static int number;
};

}
