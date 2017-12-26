
#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "np-cvthings.h"

class ofApp : public ofBaseApp, public np::ContoursOperator {

public:
	void setup();

	void update();
	void draw();
	
	void keyPressed(int key);
    void exit();
    
    void background();
    void operations( const np::ThreadedBuffer & buffer, vector<vector<np::CvContour>> & ombre ) override;
    void graphics( vector<vector<np::CvContour>> & ombre );
	void performer();

    void fillGraphics();
    
    np::Shadows					sf; // shadows framework
	np::LibreKGraySource		kGray;

    ofxPanel    gui;
    bool        bDrawGui;

	np::Palette4 palette4;
    
    ofFbo bgFbo;
    
    ofParameterGroup            opui;
		ofParameter<int>            numOmbre;    
		ofParameter<int>			mode;
		
	np::ombre::Delays delays;

};
