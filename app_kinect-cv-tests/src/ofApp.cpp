
// app_kinect-cv-tests
// Nicola Pisanti, GPLv3 License, 2016

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofEnableAlphaBlending();
    ofDisableAntiAliasing();
    selector = 0;

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
    difference.setup( 640, 480 ); 
    
    
    // -------------- contour finder setup -------------
    gui.setup("", "settings.xml", 20, 50 );
    gui.setName("SETTINGS");
    gui.add( tMasker.ui );
    gui.add( cFinder.ui );
    gui.add( difference.ui );
    
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update() {
    
    ofSetWindowTitle( "fps: " + ofToString( ofGetFrameRate() ) );
    
	kinect.update();
    
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {    
        
        tMasker.update(kinect.getDepthPixels());
        
        switch( selector ) {
            case 1:
                canvasRoutine();        
            break;
            
            case 2:
                cFinder.update( tMasker.tImage );
            break;
            
            case 3:
                difference.update( kinect.getPixels() );
                columnMean = ofxCv::meanCols( difference.image );
            break;
            
            case 4:
                difference.update( tMasker.tImage.getPixels() );
            break;
        }
        
    }
    
}

//--------------------------------------------------------------
void ofApp::canvasRoutine() {
    
    tMasker.beginCanvas();
        ofClear(255, 255, 255, 0);
        
        ofSetColor(0);
        ofFill();
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        
        ofNoFill();
        ofSetColor(255, 95, 95);
    
        for( int i=0; i<100; ++i){
            int y = ofRandom(0, ofGetHeight());
            ofDrawLine(0, y, ofGetWidth(), y);
        }
    tMasker.endCanvas();

}

//--------------------------------------------------------------
void ofApp::draw() {
    
    switch(selector){
        
        case 0:
            ofBackground(0); 
            ofSetColor(255);
            tMasker.tImage.draw(320, 0);
            ofNoFill();
            ofDrawRectangle( 319, 1, 640, 480 );
            if(bDrawGui) ofDrawBitmapString( "thresholded image", 20, 20);
        break;
        
        case 1:
            ofBackground(60); 
            ofSetColor(255, 95, 95);  
            tMasker.draw( 0, 0 );
            if(bDrawGui) ofDrawBitmapString( "alpha mask from threshold", 20, 20);
        break;
        
        case 2:  
            ofBackground(0);  
            
        	//cFinder.draw(0, 0);
            for(size_t i=0; i<cFinder.contours.size(); ++i ){
                ofFill();
                ofSetColor(60, 20, 20);
                ofDrawRectangle( cFinder.contours[i].getBoundingBox() );
                
                ofSetColor(255, 0 , 0);
                ofDrawCircle( cFinder.contours[i].getCentroid2D(), 10 );
                
                ofSetColor(255, 95, 95);
                cFinder.contours[i].draw();
                if(cFinder.bOuterContour) cFinder.outer[i].draw();
            }
            if(bDrawGui) ofDrawBitmapString( "contours processing", 20, 20);
        break;

        case 3:    
        {
            ofBackground(0); 
            ofSetColor(ofColor::red); 
            difference.draw(0, 0, ofGetWidth(), ofGetHeight());
            
            ofPushMatrix();
                ofTranslate( 0, ofGetHeight() );
                ofFill();
                ofBeginShape();
                    int max = columnMean.rows;
                    for(int i = 0; i < max; i++) {
                        ofVertex( ofMap(i, 0, max, 0, ofGetWidth()), -columnMean.at<uint8_t>(i, 0) );
                    }
                ofEndShape();
            ofPopMatrix();
            if(bDrawGui) ofDrawBitmapString( "frame difference columns", 20, 20);
        }
        break;
        
        case 4:    
            ofBackground(0); 
            ofSetColor( 255, 80, 80 ); 
            difference.draw(0, 0, ofGetWidth(), ofGetHeight());
            if(bDrawGui) ofDrawBitmapString( "thresholded image frame difference with trails", 20, 20);
        break;
        
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
            
        case OF_KEY_TAB :
            selector++;
            if(selector==5) selector = 0;
            break;
            
        case 'g' :
            bDrawGui = bDrawGui ? false : true;
            break;
	}

}
