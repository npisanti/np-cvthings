
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


    colors.resize(4);
    
    colors[0] = ofColor( 255, 70, 200);
    colors[1] = ofColor( 255, 200, 70);
    colors[2] = ofColor( 255, 85, 85);
    colors[3] = ofColor( 225, 20, 20 );
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
                
                // older delays have a darker color
                for ( int i=delayop.size()-1; i>=min; --i ){
                    ofSetColor( fillAlphaStart - i*fillStep );  
                    for( auto & cont : delayop.buffer[i] ) {                    
                        drawPolyline(cont.contour);
                    }                 
                }
                ofSetColor(0);
                
            masker.mask.end();
            
            masker.canvas.begin();
                ofClear( 0 );
                ofFill();
                // so each contour has its own color
                for ( int i=delayop.size()-1; i>=0; --i ){
                    for( auto & cont : delayop.buffer[i] ) {   
                        int index = cont.label % colors.size();
                        ofSetColor( colors[index], 255 );
                        ofDrawRectangle( cont.contour.getBoundingBox() );
                    }         
                }

            masker.canvas.end();     
                      
        }

    }

}

//--------------------------------------------------------------
void ofApp::draw() {

    ofBackground(0);  
    
    if(colorizeBackground){
        ofFill();
        ofSetColor( colors[0], backgroundAlpha );
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
                for( auto & cont : delayop.buffer[0] ) {                    
                    drawPolyline(cont.contour);
                }  
            }        

            for( auto & cont : delayop.buffer[0] ) {   
                
                ofPoint center = cont.contour.getCentroid2D();   
                int index = cont.label % colors.size();
                ofSetColor( colors[index] );    
                ofDrawBitmapString( "id " + ofToString(cont.label), center.x, center.y );   
            }  

        }
        
        if( (bDrawContours && i>0) || (i==0 && bDrawFirstContour) ){
            ofNoFill();
            for( auto & cont : delayop.buffer[i] ){
                int index = cont.label % colors.size();
                ofSetColor( colors[index], contoursAlphaStart - i*alphaStep );
                drawPolyline(cont.contour);
            }    
        }
    }    
    
    if(bDrawGui){
        gui.draw();
    }         
}

void ofApp::drawPolylines( const vector<np::CvContour> & contours, int alpha ) {
    
    for( auto & cont : contours ){
        int index = cont.label % colors.size();
        ofSetColor( colors[index], alpha );
        
        ofBeginShape();
            const vector<ofPoint> & vertices =  cont.contour.getVertices();
            int max = vertices.size();
            if(max>0){
                for ( size_t i=0; i<max; ++i ){
                    ofVertex( vertices[i].x, vertices[i].y );
                }
            }
        ofEndShape();
    }    
    
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
