
// DelayOperations.cpp
// Nicola Pisanti, GPLv3 License, 2016

#include "DelayOperations.h"

int np::DelayOperations::number = 0;

np::DelayOperations::DelayOperations( ) {

    ui.setName( "delay ops n" + ofToString(++number) );

    ui.add( bSmoothPre.set("smooth", true) );
    ui.add( smoothPre.set("smoothing px", 5, 1, 50) );
    ui.add( smoothPreMod.set("smoothing MOD", 0, -50, 50) );
    ui.add( smoothMode.set("smoothing mode", 0.0f, 0.0f, 1.0f) );

    ui.add( bExpand.set("expand", true));    
    ui.add( expand.set("expand px", 40, 0, 600));
    ui.add( expandMod.set("expand MOD", 0, -300, 300));
    ui.add( bExpandMode.set("centroid expand", true));
    
    ui.add( bSmoothPost.set("post smooth", false) );
    ui.add( smoothPost.set("post smoothing px", 15, 1, 100) );
    
    ui.add( bJitter.set("jitter ops", false) );
    ui.add( jitX.set(    "jitter x", 0, 0, 100) );
    ui.add( jitXMod.set( "jit x mod", 0, -100, 100) );
    ui.add( jitY.set(    "jitter y", 0, 0, 100) );
    ui.add( jitYMod.set( "jit y mod", 0, -100, 100) );
    
    buffer.resize( 10 );
    //buffer.clear();

}

void np::DelayOperations::update( const ContoursBuffer & cBuffer, int num, int delay  ) {

    t = num;
    for (int i=0; i<num; ++i){
        buffer[i] = cBuffer.delay( i*delay );
    }

    if( bSmoothPre ){
        for ( size_t n = 0; n<num; ++n ) {
            for (size_t i=0; i<buffer[n].size(); i++) {
                buffer[n][i] = buffer[n][i].getSmoothed( smoothPre + smoothPreMod*n, smoothMode );
            }                
        }
    }
    
    if( bExpand ){
        for ( size_t n=0; n<num; ++n ) {
            for(size_t i=0; i<buffer[n].size(); ++i ){
                
                vector<ofPoint> & vertices = buffer[n][i].getVertices();
                
                if( bExpandMode ){
                    ofPoint center = buffer[n][i].getCentroid2D();
                    for (int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++) {
                        ofPoint difference = vertices[vertexIndex] - center;
                        ofPoint outside = difference.getNormalized();
                        outside = outside * (expand + expandMod*n); 
                        vertices[vertexIndex] = vertices[vertexIndex] + outside;
                    }
                }else{
                    for (int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++) {
                        ofPoint normal = buffer[n][i].getNormalAtIndex(vertexIndex) * (expand + expandMod*n);  // Scale the normal
                        vertices[vertexIndex] = vertices[vertexIndex] + normal;
                    }
                }
                
                if( bSmoothPost ){
                    for (size_t i=0; i<buffer[n].size(); i++) {
                        buffer[n][i] = buffer[n][i].getSmoothed( smoothPost, smoothMode );
                    }        
                }
                

                
            }
        }
    }
    
    if( bJitter ){
        for ( size_t n=0; n<num; ++n ) {
            
            int jx = jitX + jitXMod*n;
            int jy = jitY + jitYMod*n;
            
            for(size_t i=0; i<buffer[n].size(); ++i ){                
                vector<ofPoint> & vertices = buffer[n][i].getVertices();

                for (int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++) {
                    vertices[vertexIndex].x += ofRandom( -jx, jx );
                    vertices[vertexIndex].y += ofRandom( -jy, jy );
                }
            }
        }        
    }    
    
}

const size_t np::DelayOperations::size() const {
    return t;
}
