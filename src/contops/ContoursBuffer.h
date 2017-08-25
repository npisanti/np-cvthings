
#pragma once

#include "ofMain.h"
#include "np-junkrepo.h"

namespace np {
    
class ContoursBuffer {
    
public:    
    void setup( int bufferSize = 1024 );
    void push( const vector<CvContour> & newContours );
    const vector<CvContour> & delay(int i) const; 
    const vector<CvContour> & now() const;
    
    ofParameterGroup ui;
    
    
private:
    ofParameter<bool> bSimplify;
    ofParameter<float> simplifyAmount;
    
    vector<vector<CvContour>> buffer;
    std::mutex bufferLock;

    int writeIndex;
    int readIndex; 
    int bufferSize;
};    
    
}
