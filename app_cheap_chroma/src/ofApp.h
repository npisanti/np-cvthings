#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		


		ofVideoGrabber	 		vidGrabber;
        ofxCvColorImage			colorImg;
        ofxCvColorImage			maskedImg;         

        //ofImage                 screenshooter;
        ofImage                 finalImg;
        
        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;

        ofxCvContourFinder 		contourFinder;

		int 					threshold;
		bool					bLearnBakground;
        bool                    bSave;
        bool                    bCenteringAids;
        bool                    bSaveOnce;


		int 		camWidth;
		int 		camHeight;
		int 		contourThreshold;
        
        ofFbo fbo;
        
        ofParameter<bool> addContour;
        
};


