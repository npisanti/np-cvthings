
// Contours.cpp
// Nicola Pisanti, GPLv3 License, 2016

#include "Contours.h"


void np::Contours::setup (int w, int h) {
    setup(w, h, w, h);
}

void np::Contours::setup( int w, int h, int destW, int destH) {
    
    if( w==destW && h==destH ){
        bScale = false;
    }else{
        bScale = true;
        scaleX = static_cast<float>(destW) / static_cast<float>(w);
        scaleY = static_cast<float>(destH) / static_cast<float>(h);
    }
    
    ui.setName( "contours" );
    ui.add( minArea.set("Min area", 10, 1, 100) );
    ui.add( maxArea.set("Max area", 200, 1, 500) );
    ui.add( bSmoothInner.set("smooth", true) );
    ui.add( smoothMode.set("smoothing mode", 0.0f, 0.0f, 1.0f) );
    ui.add( smooth.set("smoothing", 15, 1, 50) );
    ui.add( bOuterContour.set("outer contour", true) );    
    ui.add( smoothOuter.set("smoothing outer", 40, 1, 100) );
    ui.add( contoursSeparation.set("contours separation", 100, 5, 300));
    ui.add( bContourMode.set("outer mode", true));
    
    contourFinder.setThreshold( 0 );
    contourFinder.setFindHoles( false );
    contours.reserve( 10 );
    outer.reserve( 10 );
}

void np::Contours::update( ofImage & tImage ) {
   
    contourFinder.setMinAreaRadius(minArea);
    contourFinder.setMaxAreaRadius(maxArea);
    contourFinder.findContours( tImage );
   
    if(bSmoothInner){
        const vector<ofPolyline> & contLines = contourFinder.getPolylines();
        contours.clear();
        for(size_t i=0; i<contLines.size(); ++i ){
            contours.push_back(contLines[i].getSmoothed( smooth, smoothMode));
        }         
    }else{
        contours = contourFinder.getPolylines();
    }
    
    if(bOuterContour){
        outer.resize(contours.size() );
        
        for(size_t i=0; i<contours.size(); ++i ){
            outer[i].clear();
            vector<ofVec3f> vertices = contours[i].getVertices();
            
            if( bContourMode ){
                ofVec3f center = contours[i].getCentroid2D();
                for (int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++) {
                    ofVec3f vertex = vertices[vertexIndex];  // Get the vertex
                    ofVec3f difference = vertex - center;
                    ofVec3f outside = difference.getNormalized();
                    outside = outside * contoursSeparation; 
                    outer[i].addVertex( vertex + outside );
                }
            }else{
                for (int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++) {
                    ofVec3f vertex = vertices[vertexIndex];  // Get the vertex
                    ofVec3f normal = contours[i].getNormalAtIndex(vertexIndex) * contoursSeparation;  // Scale the normal
                    outer[i].addVertex( vertex-normal );
                }
            }
            
            outer[i].close();
            outer[i] = outer[i].getSmoothed( smoothOuter, smoothMode);
        }
    }else{
        outer.clear();
    }
    
    if(bScale){
        
        for(size_t i=0; i<contours.size(); ++i ){
            for(int v=0; v<contours[i].size(); ++v){
                contours[i][v].x *= scaleX; 
                contours[i][v].y *= scaleY; 
            }
        }
        
        if(bOuterContour){
            for(size_t i=0; i<outer.size(); ++i ){
                for(int v=0; v<outer[i].size(); ++v){
                    outer[i][v].x *= scaleX; 
                    outer[i][v].y *= scaleY; 
                }
            }
        }
    }
    
}


void np::Contours::draw( int x, int y ) {
    ofPushMatrix();
        ofTranslate(x, y);
        for(size_t i=0; i<contours.size(); ++i ){
            contours[i].draw();
            if(bOuterContour) outer[i].draw();
        }
    ofPopMatrix();
}

