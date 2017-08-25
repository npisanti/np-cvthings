
// Difference.cpp
// Nicola Pisanti, GPLv3 License, 2016

#include "Difference.h"


void np::Difference::setup( int w, int h ) {
    
    bTrails = true;
    
    image.allocate( w, h, OF_IMAGE_GRAYSCALE);
    ofxCv::imitate(previous, image);

    mask.allocate( w, h );
    mask.begin();
        ofClear(255,255,255,255);
    mask.end();
        
    canvas.allocate( w, h );
    canvas.begin();
        ofClear(0,0,0,0);
    canvas.end();
    
    // --------- masker setup ------------
    #ifdef TARGET_OPENGLES
    masker.load("shadersES2/shader");
    #else
    if(ofIsGLProgrammableRenderer()){
        masker.load("shadersGL3/shader");
    }else{
        masker.load("shadersGL2/shader");
    }
    #endif

    ui.setName("frame difference");
    ui.add(bTrails.set("activate trails", false));
    ui.add(trails.set("trails fading", 50, 0, 255));
    
}


void np::Difference::update( ofPixels & pixels ) {

    // take the absolute difference of prev and cam and save it inside diff
    ofxCv::absdiff(pixels, previous, image);
    image.update();
    
    // like ofSetPixels, but more concise and cross-toolkit
    ofxCv::copy(pixels, previous);
    
    canvas.begin();
    
        ofSetColor(0, trails);
        ofDrawRectangle(-2, -2, canvas.getWidth()+4, canvas.getHeight()+4); // set to right numbers
    
        ofSetColor(255);
    
        masker.begin();
            masker.setUniformTexture("maskTex", image.getTexture(), 1 );
            mask.draw(0, 0);    
        masker.end(); 
    
    canvas.end();

}


void np::Difference::draw( int x, int y ) {
    if(bTrails){
        canvas.draw(x, y);
    }else{
        image.draw(x, y);
    }
}

void np::Difference::draw( int x, int y, int w, int h ) {
    if(bTrails){
        canvas.draw(x, y, w, h);
    }else{
        image.draw(x, y, w, h);
    }
}
