
// app_kinect-cv-tests
// Nicola Pisanti, GPLv3 License, 2016

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofEnableAlphaBlending();
    ofDisableAntiAliasing();

    // --------------------------------------KINECT-------------------------------------
	kinect.setRegistration(true); // enable depth->video image calibration
	kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	kinect.open();		// opens first available kinect

	// tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
    // ----------------------------------------------------------------------------------
    
    tMasker.setup(640, 480, ofGetWidth(), ofGetHeight());
    cFinder.setup(640, 480, ofGetWidth(), ofGetHeight());
    cBuffer.setup( 128 );
    
    // -------------- contour finder setup -------------
    gui.setup("", "settings.xml", 20, 50 );
    gui.setName("SETTINGS");
    gui.add( tMasker.ui );
    gui.add( cFinder.ui );
    gui.add( delayControl.set("delay frames", 12, 1, 100) );
    
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update() {
    
    ofSetWindowTitle( "fps: " + ofToString( ofGetFrameRate() ) );
    
	kinect.update();
    
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {    
        
        tMasker.update(kinect.getDepthPixels());
        cFinder.update( tMasker.tImage );
        cBuffer.push( cFinder.contours );
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw() {
 
    ofBackground(0);  
    
    ofSetColor( 255, 0, 0 );
    for(size_t i=0; i<cFinder.contours.size(); ++i ){
        cFinder.contours[i].draw();
    }

    ofSetColor( 150, 0, 0 );        
    int d = delayControl;
    for(size_t i=0; i<cBuffer.delay(d).size(); ++i ){
        cBuffer.delay(d)[i].draw();
    }

    ofSetColor( 90, 0, 0 );        
    d *= 2;
    for(size_t i=0; i<cBuffer.delay(d).size(); ++i ){
        cBuffer.delay(d)[i].draw();
    }

    if(bDrawGui) gui.draw();
    
}



//--------------------------------------------------------------
void ofApp::keyPressed (int key) {

	switch (key) {			

        case 's':
            ofSaveScreen("screenshot-" + ofToString(ofGetFrameNum()) + ".jpg");
        break;

		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;

        case 'g' :
            bDrawGui = bDrawGui ? false : true;
            break;
	}

}
