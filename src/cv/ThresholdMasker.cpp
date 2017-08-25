
// ThresholdMaster.cpp
// Nicola Pisanti, GPLv3 License, 2016

#include "ThresholdMasker.h"


bool np::ThresholdMasker::firstConstruction = true;
ofShader np::ThresholdMasker::masker = ofShader();

void np::ThresholdMasker::setup (int w, int h) {
    setup( w, h, w, h);
}


void np::ThresholdMasker::setup (int w, int h, int destW, int destH ){
    
    ofEnableAlphaBlending();
    
    if( w==destW && h==destH ){
        bResize = false;
    }else{
        bResize = true;
    }

    tImage.allocate( w, h, OF_IMAGE_GRAYSCALE);
    
    if( bResize ){
        //mask.allocate( destW, destH, GL_LUMINANCE );
        mask.allocate( destW, destH );
        mask.begin();
            ofClear(0,0,0,0);
        mask.end();
    }
        
    canvas.allocate( destW, destH );
    canvas.begin();
        ofClear(0,0,0,0);
    canvas.end();
    
    // --------- masker setup ------------
    if(firstConstruction){
        #ifdef TARGET_OPENGLES
        masker.load("shadersES2/shader");
        #else
        if(ofIsGLProgrammableRenderer()){
            masker.load("shadersGL3/shader");
        }else{
            masker.load("shadersGL2/shader");
        }
        #endif
        firstConstruction = false;
    }
    ui.setName("alpha masker");
    ui.add( t.set("threshold", 215, 0, 255) );
    
}



void np::ThresholdMasker::update( const ofPixels & pixels ) {
        tImage.setFromPixels( pixels );
        ofxCv::threshold(tImage, t);
        
		tImage.update();
        tImage.mirror( false, true ); // mirrors image <<---------------------------------
        
        if(bResize){
            mask.begin();
                ofClear(0,0,0,0);
                tImage.draw(0, 0, mask.getWidth(), mask.getHeight() );
            mask.end();
        }
}


void np::ThresholdMasker::beginCanvas() {
    canvas.begin();
}

void np::ThresholdMasker::endCanvas() {
    canvas.end();
}


void np::ThresholdMasker::draw( int x, int y ) {
    
    ofPushMatrix();
        ofTranslate(x, y);
        masker.begin();
            if(bResize){
                masker.setUniformTexture("maskTex", mask.getTexture(), 1 );
            }else{
                masker.setUniformTexture("maskTex", tImage.getTexture(), 1 );
            }
             canvas.draw(0, 0);    
        masker.end(); 
    ofPopMatrix();   

}

