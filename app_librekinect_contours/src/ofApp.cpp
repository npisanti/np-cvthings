
// Nicola Pisanti, GPLv3 License, 2016

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofEnableAlphaBlending();
    ofDisableAntiAliasing();
	
	ofSetBackgroundAuto( false );	
    bDrawGui = true;
    
    int pw = 640;
    int ph = 480;
    
	buffer.setup( 128, kGray, pw, ph );
	fbo.allocate( pw, ph );
	
    // -------------- gui -------------
    gui.setup("", "settings.xml", ofGetWidth()-220, 20 );
    gui.setName("SETTINGS");
    gui.add( buffer.ui );
    gui.loadFromFile("settings.xml");
    
    ofSetWindowTitle("librekinect blob detection");
}


//--------------------------------------------------------------
void ofApp::update() {
	if( buffer.update() ) {    
		fbo.begin();
			ofClear( 0, 0, 0, 0);
			ofFill();  
			ofSetColor( 255 );
			for( auto & cont : buffer.now() ){ 
				if ( !cont.hole ) cont.draw(); 
			}  
			ofSetColor( 0 );  
			for( auto & cont : buffer.now() ) {                    
				if ( cont.hole ) cont.draw();
			}   
			ofNoFill();
			ofSetColor( 255, 0, 0 );
			for( auto & cont : buffer.now() ) {                    
				ofVec2f direction = cont.velocity * 10.0f;
				direction += cont.center;
				ofDrawLine(cont.center.x, cont.center.y, direction.x, direction.y );
				ofDrawCircle( cont.center.x, cont.center.y, 10 );
				string info = "label = ";
				info+= ofToString( cont.label );
				ofDrawBitmapString( info, cont.center.x-30, cont.center.y+20);
				ofDrawRectangle( cont.contour.getBoundingBox() );
			}   
		fbo.end();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground( 0 );

	ofSetColor(255);
	fbo.draw(0, 0);
	
    if(bDrawGui){
        gui.draw();
    }         
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
	switch (key) {			
        case 'g' :
            bDrawGui = bDrawGui ? false : true;
            break;            
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
    buffer.exit();
}
