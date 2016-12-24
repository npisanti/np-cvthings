
#pragma once

#include "ofMain.h"

namespace np {
    
class ContoursBuffer {
    
public:    
    void setup( int bufferSize = 1024 );
    void push( const vector<ofPolyline> & newContours );
    const vector<ofPolyline> & delay(int i) const; 
    const vector<ofPolyline> & now() const;
    
    
private:
    vector<vector<ofPolyline>> buffer;
    std::mutex bufferLock;

    int index; 
    int bufferSize;
};    
    
}
