
#include "ContoursBuffer.h"

void np::ContoursBuffer::setup( int bufferSize ) {
    this->bufferSize = bufferSize;
    readIndex = 0;
    writeIndex = 0;
    buffer.resize( bufferSize );
    ui.setName( "contours buffer");
    ui.add( bSimplify.set("simplify", false) );
    ui.add( simplifyAmount.set("tolerance", 0.0f, 0.0f, 1.0f) );
}


void np::ContoursBuffer::push( const vector<np::CvContour> & newContours ) {
    writeIndex++;
    writeIndex = (writeIndex==bufferSize) ? 0 : writeIndex;

    buffer[writeIndex] = newContours;  
    
    if( bSimplify ) {
        for( size_t i=0; i<buffer[writeIndex].size(); ++i ) {
            buffer[writeIndex][i].contour.simplify( simplifyAmount );
        }
    }
    readIndex = writeIndex;

}

const vector<np::CvContour> & np::ContoursBuffer::delay(int i) const {
    int n = readIndex - i;
    while( n  < 0 ) n += bufferSize;
    return buffer[n];
}


const vector<np::CvContour> & np::ContoursBuffer::now() const {
    return buffer[readIndex];
}
