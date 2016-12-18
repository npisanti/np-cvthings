
#include "ContoursBuffer.h"

void np::ContoursBuffer::setup( int bufferSize ) {
    this->bufferSize = bufferSize;
    index = 0;
    buffer.resize( bufferSize );
}


void np::ContoursBuffer::push( const vector<ofPolyline> & newContours ) {
    index++;
    index = (index==bufferSize) ? 0 : index;
    buffer[index] = newContours;
}

const vector<ofPolyline> & np::ContoursBuffer::delay(int i) {
    int n = index - i;
    while( n  < 0 ) n += bufferSize;
    return buffer[n];
}


const vector<ofPolyline> & np::ContoursBuffer::now() {
    return buffer[index];
}
