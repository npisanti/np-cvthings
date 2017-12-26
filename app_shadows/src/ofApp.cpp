
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
    
    bgFbo.allocate( pw, ph );
	bgFbo.begin();
		ofClear( 0, 0, 0, 0 );
	bgFbo.end();

    sf.setup( pw, ph, kGray, this );
	
    // -------------- gui -------------
    gui.setup("", "settings.xml", ofGetWidth()-220, 20 );
    gui.setName("SETTINGS");

	opui.setName("ombre");
    opui.add( mode.set("mode", 0, 0, 3) );
    opui.add( numOmbre.set("number", 3, 1, 5) );
	gui.add( opui );
	gui.add( delays.ui );

	sf.addToGui( gui );
    gui.minimizeAll();
    gui.loadFromFile("settings.xml");
    
    ofSetWindowTitle("shadows framework");

}

//--------------------------------------------------------------
void ofApp::background() {
	ofClear(0, 0, 0, 0);
}

void ofApp::operations( const np::ThreadedBuffer & buffer, vector<vector<np::CvContour>> & ombre ){
	// makes the operations on the delayed frames
	ombre.resize( numOmbre );
	delays.operations( buffer, ombre );
}

void ofApp::graphics( vector<vector<np::CvContour>> & ombre ) {
    ofClear( palette4.colors[1].r, palette4.colors[1].g, palette4.colors[1].b, 255 ); 
}

void ofApp::performer() {
	ofClear( palette4.colors[0].r, palette4.colors[0].g, palette4.colors[0].b, 255 );
}

//--------------------------------------------------------------
void ofApp::update() {
	bgFbo.begin();
		background();
	bgFbo.end();
	
	sf.ombreMasker.canvas.begin();
		graphics( sf.ombre );
	sf.ombreMasker.canvas.end();   

	sf.performerMasker.canvas.begin();
		performer();
	sf.performerMasker.canvas.end();   

	sf.update( palette4.colors[0], palette4.colors[1] );
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground( 0 );
    ofSetColor(255);
    bgFbo.draw( 0, 0 );
    sf.draw(0, 0);
	
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
    sf.exit();
}
