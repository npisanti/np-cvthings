
// ContoursOperations.cpp
// Nicola Pisanti, GPLv3 License, 2016

#include "ContoursOperations.h"

int np::ContoursOperations::number = 0;

np::ContoursOperations::ContoursOperations( ) {

    ui.setName( "contours op n" + ofToString(++number) );

    ui.add( bSmoothPre.set("smooth", true) );
    ui.add( smoothPre.set("smoothing px", 5, 1, 50) );
    ui.add( smoothMode.set("smoothing mode", 0.0f, 0.0f, 1.0f) );

    ui.add( bExpand.set("expand", true));    
    ui.add( expand.set("expand px", 40, 5, 300));
    ui.add( bExpandMode.set("centroid expand", true));
    
    ui.add( bSmoothPost.set("post smooth", false) );
    ui.add( smoothPost.set("post smoothing px", 15, 1, 100) );
    
    contours.reserve( 16 );

}

void np::ContoursOperations::update(  const vector<ofPolyline> & newContours  ) {

    contours = newContours;

    if( bSmoothPre ){
        for (size_t i=0; i<contours.size(); i++) {
            contours[i] = contours[i].getSmoothed( smoothPre, smoothMode );
        }        
    }
    
    if( bExpand ){

        for(size_t i=0; i<contours.size(); ++i ){
            
            vector<ofPoint> & vertices = contours[i].getVertices();
            
            if( bExpandMode ){
                ofPoint center = contours[i].getCentroid2D();
                for (int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++) {
                    ofPoint difference = vertices[vertexIndex] - center;
                    ofPoint outside = difference.getNormalized();
                    outside = outside * expand; 
                    vertices[vertexIndex] = vertices[vertexIndex] + outside;
                }
            }else{
                for (int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++) {
                    ofPoint normal = contours[i].getNormalAtIndex(vertexIndex) * expand;  // Scale the normal
                    vertices[vertexIndex] = vertices[vertexIndex] + normal;
                }
            }
            if( bSmoothPost ){
                for (size_t i=0; i<contours.size(); i++) {
                    contours[i] = contours[i].getSmoothed( smoothPost, smoothMode );
                }        
            }
        }
        
    }
    
}

