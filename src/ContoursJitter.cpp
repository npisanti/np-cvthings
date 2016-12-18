
// ContoursJitter.cpp
// Nicola Pisanti, GPLv3 License, 2016

#include "ContoursJitter.h"

int np::ContoursJitter::number = 0;

np::ContoursJitter::ContoursJitter( ) {

    ui.setName( "contours jit n" + ofToString(++number) );

    ui.add( bActivate.set("activate", true) );
    ui.add( jitter.set("jitter px", 5, 0, 100) );
    
    contours.reserve( 16 );

}

void np::ContoursJitter::update(  const vector<ofPolyline> & newContours  ) {

    contours = newContours;

    if( bActivate ){
        for(size_t i=0; i<contours.size(); ++i ){
            vector<ofPoint> & vertices = contours[i].getVertices();
            for (int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++) {
                vertices[vertexIndex].x += ofRandom(-jitter, jitter);
                vertices[vertexIndex].y += ofRandom(-jitter, jitter);
            }
        }
    }
    
}

