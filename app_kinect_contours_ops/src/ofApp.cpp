
// Nicola Pisanti, GPLv3 License, 2016

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofEnableAlphaBlending();
    ofDisableAntiAliasing();

    angle = 0;
    int pw = ofGetWidth();
    int ph = ofGetHeight();
    
    kContours.setup( pw, ph );
    buffer.setup( 400 );
    masker.setup( ofGetWidth(), ofGetHeight() );
    
    // -------------- contour finder setup -------------
    gui.setup("", "settings.xml", ofGetWidth()-220, 20 );
    gui.setName("SETTINGS");
    gui.add( kContours.ui );
    gui.add( buffer.ui );
    gui.add( delayop.ui );
    
    graphics.setName("graphics options");    
    graphics.add( bDelayFill.set("delay fill", false) );
    graphics.add( fillAlphaStart.set("fill alpha start", 255, 0, 255));
    graphics.add( bBlackNow.set("black fill first", false));
    graphics.add( bDrawContours.set("draw del contours", true));
    graphics.add( bDrawFirstContour.set("draw 1st contours", true));
    graphics.add( contoursAlphaStart.set("cont alpha start", 255, 0, 255));
    graphics.add( colorizeBackground.set("colorize background", false) );
    graphics.add( backgroundAlpha.set("background alpha", 0, 0, 255) );
    
    gui.add( graphics );
    gui.minimizeAll();
    gui.loadFromFile("settings.xml");
    
    bDrawGui = true;

 
    color = ofColor( 255, 70, 200);
    
}


//--------------------------------------------------------------
void ofApp::update() {
    
    ofSetWindowTitle( "fps: " + ofToString( ofGetFrameRate() ) );

	// there is a new frame and we are connected
	if(kContours.hasNewContours()) {    
        
        buffer.push( kContours.getContours() );

        delayop.update( buffer );

        alphaStep = contoursAlphaStart / (delayop.size()+1);    

        if( bDelayFill ){
            
            masker.mask.begin();
                ofClear(0, 0, 0, 0);
                ofFill();                    
                
                int min = 1;
                if( bDrawFirstContour ) min = 0;
                int fillStep = fillAlphaStart / (delayop.size()+1);
                for ( int i=delayop.size()-1; i>=min; --i ){
                    ofSetColor( fillAlphaStart - i*fillStep );
                    drawPolylines( delayop.buffer[i] );                      
                }
            masker.mask.end();
            
            masker.canvas.begin();
                ofClear( color );
            masker.canvas.end();     
                      
        }

    }

}

//--------------------------------------------------------------
void ofApp::draw() {

    ofBackground(0);  
    
    if(colorizeBackground){
        ofFill();
        ofSetColor( color, backgroundAlpha );
        ofDrawRectangle( 0, 0, ofGetWidth(), ofGetHeight() );                
    }

    if( bDelayFill ){
        masker.draw(0, 0);
    }

    for ( int i=delayop.size()-1; i>=0; --i ){
        if( i==0){
            if(bBlackNow){
                ofSetColor(0);
                ofFill();
                drawPolylines( delayop.buffer[0] ); 
            }            
        }
        
        if( (bDrawContours && i>0) || (i==0 && bDrawFirstContour) ){
            ofSetColor( color, contoursAlphaStart - i*alphaStep );
            ofNoFill();
            drawPolylines( delayop.buffer[i] );
        }
    }    
    
    if(bDrawGui){
        gui.draw();
    }         
}

void ofApp::drawPolylines( const vector<ofPolyline> & polylines ) {
    
    for( auto & cont : polylines ){
        
        ofBeginShape();
            const vector<ofPoint> & vertices =  cont.getVertices();
            int max = vertices.size();
            if(max>0){
                for ( size_t i=0; i<max; ++i ){
                    ofVertex( vertices[i].x, vertices[i].y );
                }
            }
        ofEndShape();
    }    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {

	switch (key) {			
    
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kContours.kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kContours.kinect.setCameraTiltAngle(angle);
			break;

        case 'g' :
            bDrawGui = bDrawGui ? false : true;
            break;
	}

}

//--------------------------------------------------------------
void ofApp::exit() {
    kContours.exit();
}
