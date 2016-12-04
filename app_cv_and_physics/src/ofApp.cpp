#include "ofApp.h"

// app_cv_and_physics
// Nicola Pisanti, MIT License, 2015-2016

//--------------------------------------------------------------
void ofApp::setup(){


	camWidth=640;
	camHeight=480;
	contourThreshold = 20;

    circleDensity = 0.06;
	circleBounce = 0.6;


	ofSetFrameRate(30);
	ofDisableAntiAliasing();
	ofSetVerticalSync(true);
	polygonateFlag = true;

	//cv init

    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth, camHeight);

    colorImg.allocate(camWidth, camHeight);
	grayImage.allocate(camWidth, camHeight);
	grayBg.allocate(camWidth, camHeight);
	grayDiff.allocate(camWidth, camHeight);
	maskedImg.allocate(camWidth, camHeight);

	bLearnBakground = true;
	threshold = 30;

	//physics init 
	boxBounds.set(0,0, 640, 480);

	box2d.init();
	box2d.setGravity(0, 50);
	box2d.setFPS(30.0);
	box2d.createBounds(boxBounds);
    box2d.registerGrabbing();

	toTriangulate.clear();

	for(int i = 0; i<3; i++){
        ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
        circle.get()->setPhysics(circleDensity, circleBounce, 0.1);
		circle.get()->setup(box2d.getWorld(), ofRandom(0, 640), ofRandom(0,50), ofRandom(9,14));
		circles.push_back(circle);

	}
	for(int i = 0; i<4; i++){
        ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
        circle.get()->setPhysics(circleDensity, circleBounce, 0.1);
		circle.get()->setup(box2d.getWorld(), ofRandom(0, 640), ofRandom(0,50), ofRandom(9,14));
		circles.push_back(circle);

	}


}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100,100,100);


	//COMPUTER VISION

    bool bNewFrame = false;

    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();

	if (bNewFrame){
   
 		colorImg.setFromPixels(vidGrabber.getPixels().getData(), camWidth, camHeight);
 		colorImg.mirror(false, true);
        
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
		contourFinder.findContours(grayDiff, 10, (640*480)/3, contourThreshold, true);	// find holes
	}

	//BOX2D 
	if(polygonateFlag){
		polygonateCv();
	}
	box2d.update();	

	//pixel by pixel mask, could it be faster with a shader? 
	unsigned char * grayPixels=grayDiff.getPixels().getData();
	unsigned char * colorPixels=colorImg.getPixels().getData();
	int max = camWidth*camHeight;

	for (int i=0; i<max; i++){
		if(*(grayPixels+i)==0){
			*(colorPixels +i*3   ) = 0;
			*(colorPixels +i*3 +1) = 0;
			*(colorPixels +i*3 +2) = 0;
			
		}
	}
	maskedImg.setFromPixels(colorPixels, camWidth, camHeight);

}

//--------------------------------------------------------------
void ofApp::draw(){

	
	//infos 
	ofSetColor(0);

	string info = "FPS: " + ofToString(ofGetFrameRate()) + "\n";
	info +=  "cam threshold: " + ofToString(threshold) + " (up/down to change)";
	ofDrawBitmapString(info, 400, ofGetHeight() - 40);

	ofDrawBitmapString("background \n(spacebar to set)", 20, 150);
	ofDrawBitmapString("difference", 200, 150);
	ofDrawBitmapString("blobs detection", 380, 150);
	ofDrawBitmapString("physical frame", 560, 150);

	info  = "box2d world\n";
	info += "(press t to stop camera acquisition)\n";
	ofDrawBitmapString(info, 20, ofGetHeight() - 40);

	info  = "press 1 add a circle\n";
	info += "press c to clear all circles\n";
	ofDrawBitmapString(info, 360, 430);

	// draw the bg and the thresholded difference
	ofSetHexColor(0xffffff);

	grayBg.draw(20, 20, 160, 120);
	grayDiff.draw(200, 20, 160, 120);

	//draw blobs
	ofPushMatrix();
	ofTranslate(380,20);
	ofScale(0.25,0.25);
	ofFill();
	ofSetHexColor(0x333333);
	ofRect(0,0,640, 480);
	ofSetHexColor(0xffffff);
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(0,0);
		// draw over the centroid if the blob is a hole
		ofSetColor(255);
		if(contourFinder.blobs[i].hole){
			ofDrawBitmapString("hole",
				contourFinder.blobs[i].boundingRect.getCenter().x,
				contourFinder.blobs[i].boundingRect.getCenter().y);
		}
    }
    ofPopMatrix();


    //triangulation
	ofPushMatrix();
	ofTranslate(560,20);
	ofScale(0.25,0.25);
	ofFill();
    ofSetColor(0);
    ofRect(0, 0, 640, 480);
    ofNoFill();
    ofSetColor(0, 255, 0);
    for (int i=0; i<polyShapes.size(); i++) {
		polyShapes[i].get()->draw();
        
        ofCircle(polyShapes[i].get()->getPosition(), 3);
	}	
	ofPopMatrix();

 
    // FINAL FRAME
    ofTranslate( 50, 180);

    if(polygonateFlag){
    	
    	ofSetColor(0, 255, 0);
    	maskedImg.draw(0,0);
    	ofFill();

    }else{
    	
    	ofFill();
    	ofSetColor(0);
	    ofRect(0, 0, 640, 480);
	    ofNoFill();
   		ofSetColor(0, 255, 0);
    	for (int i=0; i<polyShapes.size(); i++) {
			polyShapes[i].get()->draw();
        	ofCircle(polyShapes[i].get()->getPosition(), 3);
		}	
    }


   
    
  	ofNoFill();
 	ofSetColor(0, 255, 0);
	for (int i=0; i<circles.size(); i++) {
		//getting radius, position and rotation you can draw your own shapes
		ofVec2f pos = circles[i]->getPosition();
		float radius = circles[i]->getRadius();
		float theta =  ofDegToRad(circles[i]->getRotation());  //you get thet angle in degree, convert to radians
		ofCircle(pos.x, pos.y, radius);
		ofLine(pos.x,pos.y, pos.x + cos(theta)*radius, pos.y + sin(theta)*radius);

	}


}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key){
		case '1':
			if(mouseX>20 && mouseX<720 && mouseY>180 && mouseY<420){
		        ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
		        circle.get()->setPhysics(circleDensity, circleBounce, 0.1);
				circle.get()->setup(box2d.getWorld(), mouseX-20, mouseY-180, ofRandom(9,14));
				circles.push_back(circle);
			}else{
		        ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
		        circle.get()->setPhysics(circleDensity, circleBounce, 0.1);
				circle.get()->setup(box2d.getWorld(), ofRandom(0,690), ofRandom(0,50), ofRandom(9,14));
				circles.push_back(circle);

			}
			break;
		case ' ':
			bLearnBakground = true;
			break;

		case OF_KEY_UP:
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		
		case OF_KEY_DOWN:
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		
		case 't':
			if(polygonateFlag){
				polygonateFlag = false;
			}else{
				polygonateFlag = true;
			}
			break;
		
		case 'c':
			circles.clear();
			break;
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


void ofApp::polygonateCv(){
	
	polyShapes.clear();

	for (int i = 0; i < contourFinder.nBlobs; i++){
		ofSetColor(255);

		if(! contourFinder.blobs[i].hole){
			//get a pointer to this blob contour points vector
			vector <ofPoint> * pCountourPoints = & contourFinder.blobs[i].pts;
			
			for(int j=0; j<pCountourPoints->size(); j++ ){
				toTriangulate.addVertex(pCountourPoints->at(j).x, pCountourPoints->at(j).y);
			}
			        // This is the manual way to triangulate the shape
	        // you can then add many little triangles
	        
	        // first simplify the shape
	        toTriangulate.simplify();
	        
	        // save the outline of the shape
	        ofPolyline outline = toTriangulate;
	        
	        // resample shape
	        ofPolyline resampled = toTriangulate.getResampledBySpacing(contourThreshold);
	        
	        // trangleate the shape, return am array of traingles
	        vector <TriangleShape> tris = triangulatePolygonWithOutline(resampled, outline);
	        
	        // add some random points inside
	        addRandomPointsInside(toTriangulate, 255);
	        
	        // now loop through all the trainles and make a box2d triangle
	        for (int k=0; k<tris.size(); k++) {            
	            ofPtr<ofxBox2dPolygon> triangle = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	            if(getTriangleRadius(tris[k].a, tris[k].b, tris[k].c)>1.5){
		            triangle.get()->addTriangle(tris[k].a, tris[k].b, tris[k].c);
		            triangle.get()->setPhysics(1.0, 0.3, 0.3);
		            triangle.get()->create(box2d.getWorld());
		            //the error is here, ofBox2dPolygon calls ofPath.tessellate()
		            // tofTessellator::performTessellation is the final call that gives the error
		            polyShapes.push_back(triangle);
	            }

	        }
	        toTriangulate.clear();					
		}
    }
}
