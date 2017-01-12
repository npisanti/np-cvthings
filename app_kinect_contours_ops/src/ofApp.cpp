
// Nicola Pisanti, GPLv3 License, 2016

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofEnableAlphaBlending();
    ofDisableAntiAliasing();

    angle = 0;
    int pw = ofGetWidth();
    int ph = ofGetHeight();
    
    fbo.allocate( pw, ph );
    fbo.begin();
        ofClear(0, 0, 0, 0);
    fbo.end();
    
    kBuffer.setup( 512, pw, ph );
    masker.setup( ofGetWidth(), ofGetHeight() );
    
    // -------------- contour finder setup -------------
    gui.setup("", "settings.xml", ofGetWidth()-220, 20 );
    gui.setName("SETTINGS");
    gui.add( delayFrames.set("delay frames", 12, 1, 60) );
    gui.add( numDelays.set("num delays", 3, 1, 5) );

    gui.add( kBuffer.ui );
    gui.add( ops.ui );
    
    graphics.setName("app graphics");    
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


    colors.resize(4);
    
    colors[0] = ofColor( 255, 70, 200);
    colors[1] = ofColor( 255, 200, 70);
    colors[2] = ofColor( 255, 85, 85);
    colors[3] = ofColor( 225, 20, 20 );
}


//--------------------------------------------------------------
void ofApp::update() {
    
    ofSetWindowTitle( "fps: " + ofToString( ofGetFrameRate() ) );

	// there is a new contour and we are connected
	if( kBuffer.update() ) {    
        
        for( size_t i=0; i<numDelays; ++i ){
            ops.update( i, kBuffer.delay(i, delayFrames) );
        }
        
        // -------------------- UPDATE YOUR GRAPHICS HERE -----------------
        alphaStep = contoursAlphaStart / (numDelays+1);    

        if( bDelayFill ){
            
            masker.mask.begin();
                ofClear(0, 0, 0, 0);
                ofFill();                    
                
                int min = 1;
                if( bDrawFirstContour ) min = 0;
                int fillStep = fillAlphaStart / (numDelays+1);
                
                // older delays have a darker color
                for ( int i=numDelays-1; i>=min; --i ){
                    ofSetColor( fillAlphaStart - i*fillStep );  
                    for( auto & cont : ops.buffer[i] ) {                    
                        drawPolyline(cont.contour);
                    }                 
                }
                ofSetColor(0);
                
            masker.mask.end();
            
            masker.canvas.begin();
                ofClear( 0 );
                ofFill();
                // so each contour has its own color
                for ( int i=numDelays-1; i>=0; --i ){
                    for( auto & cont : ops.buffer[i] ) {   
                        ofSetColor( getLabelColor(cont), 255 );
                        ofDrawRectangle( cont.contour.getBoundingBox() );
                    }         
                }

            masker.canvas.end();     
        }

        fbo.begin();
            ofClear(0, 0, 0, 255);
            
            if(colorizeBackground){
                ofFill();
                ofSetColor( colors[0], backgroundAlpha );
                ofDrawRectangle( 0, 0, ofGetWidth(), ofGetHeight() );                
            }

            if( bDelayFill ){
                masker.draw(0, 0);
            }

            for ( int i=numDelays-1; i>=0; --i ){
                
                if( i==0){
                    if(bBlackNow){
                        ofSetColor(0);
                        ofFill();
                        for( auto & cont : ops.buffer[0] ) {                    
                            drawPolyline(cont.contour);
                        }  
                    }        

                    for( auto & cont : ops.buffer[0] ) {   
                        
                        ofPoint center = cont.contour.getCentroid2D();   
                        ofSetColor( getLabelColor(cont) );    
                        ofDrawBitmapString( "id " + ofToString(cont.label), center.x, center.y );   
                    }  

                }
                
                if( (bDrawContours && i>0) || (i==0 && bDrawFirstContour) ){
                    ofNoFill();
                    for( auto & cont : ops.buffer[i] ){
                        ofSetColor( getLabelColor(cont), contoursAlphaStart - i*alphaStep );
                        drawPolyline(cont.contour);
                    }    
                }
            }    
        fbo.end();

    }

}

const ofColor & ofApp::getLabelColor( const np::CvContour & cont ) const {
    int index = cont.label % colors.size();
    return colors[index];
}

void ofApp::drawPolyline( const ofPolyline & line ) {
    
    ofBeginShape();
        const vector<ofPoint> & vertices =  line.getVertices();
        int max = vertices.size();
        if(max>0){
            for ( size_t i=0; i<max; ++i ){
                ofVertex( vertices[i].x, vertices[i].y );
            }
        }
    ofEndShape();
    
}

//--------------------------------------------------------------
void ofApp::draw() {

    ofBackground(0);  
    fbo.draw(0, 0);
    
    if(bDrawGui){
        gui.draw();
    }         
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {

	switch (key) {			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kBuffer.kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kBuffer.kinect.setCameraTiltAngle(angle);
			break;

        case 'g' :
            bDrawGui = bDrawGui ? false : true;
            break;
	}

}

//--------------------------------------------------------------
void ofApp::exit() {
    kBuffer.exit();
}
