
// app_vangelian_lead
// Nicola Pisanti, MIT License, 2016

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	camWidth=640;
	camHeight=480;
	contourThreshold = 20;
    
    addContour = true;
    bCenteringAids = false;
	bLearnBakground = true;
	threshold = 30;
    
	ofSetFrameRate(30);
	ofDisableAntiAliasing();
	ofSetVerticalSync(true);
    ofEnableAlphaBlending();


    vidGrabber.setVerbose(true);    
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(30);    
    vidGrabber.initGrabber(camWidth, camHeight);

    colorImg.allocate(camWidth, camHeight);
	grayImage.allocate(camWidth, camHeight);
	grayBg.allocate(camWidth, camHeight);
	grayDiff.allocate(camWidth, camHeight);
	maskedImg.allocate(camWidth, camHeight);
    
    finalImg.allocate(camWidth, camHeight, OF_IMAGE_COLOR_ALPHA);

    fbo.allocate(camWidth, camHeight, GL_RGBA);
    
    fbo.begin();
        ofClear(0, 0, 0, 0);
    fbo.end();
    
    ofSetWindowTitle( "cheap chroma" );
}

//--------------------------------------------------------------
void ofApp::update(){
	
    ofBackground( 0, 0, 0 );
    
    bool bNewFrame = false;

    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();

	if (bNewFrame){
   
 		colorImg.setFromPixels(vidGrabber.getPixels().getData(), camWidth, camHeight);
 		//colorImg.mirror(false, true);

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
        
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 10, (camWidth*camHeight)/3, contourThreshold, true);	// find holes
	
    }


	ofPixels & grayPixels  = grayDiff.getPixels();
	ofPixels & colorPixels = colorImg.getPixels();
	    
    for (int x=0; x<camWidth; x++){
        for( int y=0; y<camHeight; y++ ){
            int i = x + y*camWidth;
            if( grayPixels[i]==0 ){
                finalImg.setColor(x, y, ofColor(255, 0, 0, 0));
            }else{
                finalImg.setColor(x, y, ofColor( colorPixels[i*3], colorPixels[i*3 +1], colorPixels[i*3 +2], 255 ) );
            }
        }
    }
    finalImg.update();
   
   
    if( addContour ){
        fbo.begin();
       
            ofClear(0, 0, 0, 0);
            
            ofSetColor( 255);
            finalImg.draw(0, 0);

            ofNoFill();
            for (int i = 0; i < contourFinder.nBlobs; i++){
                ofBeginShape();
                for( ofPoint & point : contourFinder.blobs[i].pts ){
                    
                    ofVertex (point.x, point.y);
                }
                ofEndShape();
            }
            
        fbo.end();  
        
        ofPixels pixels;
        fbo.readToPixels( pixels );
        finalImg.setFromPixels( pixels );
    }
    
    // SAVE IMAGE HERE! ------------------
    
    if( bSave || bSaveOnce ){
        string name = "frame_";
        name+= ofToString(ofGetFrameNum());
        name += ".png";
        finalImg.save( name );
        bSaveOnce = false;
    }


}

//--------------------------------------------------------------
void ofApp::draw(){

    // INFOS --------------------------------------
	ofSetColor(255, 95, 95);

	string info = "FPS: " + ofToString(ofGetFrameRate()) + "\n";
	info +=  "cam threshold: " + ofToString(threshold) + " (up/down to change)\n";
    info += "press spacebar to ";
    if(bSave){
        info += "stop saving\n";
    }else{
        info += "start saving\n";
    }
    info += "press s to save one frame\n";
    info += "press a to ";
    if(bCenteringAids){
        info += "remove centering aids\n";
    }else{
        info += "add centering aids\n";
    }
	ofDrawBitmapString(info, 20, ofGetHeight() - 60 );

	ofDrawBitmapString("webcam", 20, 150);
	ofDrawBitmapString("background \n(press b to set)", 200, 150);
	ofDrawBitmapString("difference", 380, 150);
    
    info = "contour\n(press c to ";
    if(addContour){
        info += "remove)";
    }else{
        info += "add)";
    }
	ofDrawBitmapString(info, 560, 150);

    // MONITORS -----------------------------------------------
	// draw the bg and the thresholded difference
	ofSetHexColor(0xffffff);
    colorImg.draw(20, 20, 160, 120);
	grayBg.draw(200, 20, 160, 120);
	grayDiff.draw(380, 20, 160, 120);

	//draw contour
	ofPushMatrix();
	ofTranslate(560,20);
	ofScale(0.25,0.25);
	ofFill();
	ofSetHexColor(0x333333);
	ofDrawRectangle(0,0,camWidth,camHeight);
	ofSetHexColor(0xffffff);
    
    ofNoFill();
    for (int i = 0; i < contourFinder.nBlobs; i++){
        
        ofBeginShape();
        
        for( ofPoint & point : contourFinder.blobs[i].pts ){
            
            ofVertex ( point );
        }
        ofEndShape();

    }
    ofPopMatrix();
    
 
    // FINAL FRAME -----------------------------------------
    ofPushMatrix();	
    ofTranslate(20, 180);
    
    ofSetColor( 255 );
    finalImg.draw(0, 0);
    
    if(bCenteringAids){
        ofSetColor(255, 0, 0);
        ofDrawLine( 0, camHeight/2, camWidth, camHeight/2);
        ofDrawLine( camWidth/2, 0, camWidth/2, camHeight );
    }
    
    ofPopMatrix();
    ofFill();
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key){
        
        case ' ':
            bSave = bSave ? false : true;
            break;

		case 'b':
			bLearnBakground = true;
			break;

		case 'a':
			bCenteringAids = bCenteringAids ? false : true;
			break;

		case OF_KEY_UP:
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		
		case OF_KEY_DOWN:
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
       
        case 'c':
            addContour = addContour ? false : true;
            break;
        
        case 's':
            bSaveOnce = true;

    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
