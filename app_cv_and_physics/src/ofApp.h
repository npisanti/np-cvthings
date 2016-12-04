#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxBox2d.h"

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

		void polygonateCv();

		ofVideoGrabber	 		vidGrabber;
        ofxCvColorImage			colorImg;
        ofxCvColorImage			maskedImg;         


        
        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;

        ofxCvContourFinder 		contourFinder;

		int 					threshold;
		bool					bLearnBakground;
		bool polygonateFlag;

		ofxBox2d                box2d;
		ofPolyline              toTriangulate;
		vector <ofPtr<ofxBox2dPolygon> >	polyShapes;
		ofRectangle 			boxBounds;
		vector <ofPtr<ofxBox2dCircle> >     circles;


		int 		camWidth;
		int 		camHeight;
		int 		contourThreshold;

		float 		circleDensity;
		float		circleBounce;
};


