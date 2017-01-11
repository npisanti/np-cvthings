
#include "ContoursBuffer.h"

void np::ContoursBuffer::setup( int bufferSize ) {
    this->bufferSize = bufferSize;
    index = 0;
    buffer.resize( bufferSize );
    ui.setName( "contours buffer");
    ui.add( bSimplify.set("simplify", false) );
    ui.add( simplifyAmount.set("tolerance", 0.0f, 0.0f, 1.0f) );
}


void np::ContoursBuffer::push( const vector<ofPolyline> & newContours ) {
    index++;
    index = (index==bufferSize) ? 0 : index;

    buffer[index] = newContours;  
    
    if( bSimplify ) {
        for( size_t i=0; i<buffer[index].size(); ++i ) {
            buffer[index][i].simplify( simplifyAmount );
        }
    }

}

const vector<ofPolyline> & np::ContoursBuffer::delay(int i) const {
    int n = index - i;
    while( n  < 0 ) n += bufferSize;
    return buffer[n];
}


const vector<ofPolyline> & np::ContoursBuffer::now() const {
    return buffer[index];
}
