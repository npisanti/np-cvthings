
#pragma once

#include "ofMain.h"
#include "np-junkrepo.h"
#include "../buffers/ThreadedBuffer.h"
#include "../contops/BufferedOperations.h"
#include "ContoursOperator.h"

namespace np{
	
class Shadows {

public:
	void setup( int pw, int ph, np::GrayImageSource & source, ContoursOperator * op );
	void addToGui( ofxPanel & gui );
		
	void update( const ofColor & performerColor, const ofColor & contoursColor );

	void draw( int x, int y );
	
	void exit();
	
    np::AlphaMasker             ombreMasker;
    np::AlphaMasker             performerMasker;
    
	vector<vector<CvContour>>   ombre;
    
    
    ofParameterGroup            graphics;
        ofParameter<int>            ombreAlpha;
        ofParameter<int>            ombreContourAlpha;
        ofParameter<int>            performerAlpha;
        ofParameter<int>            performerContourAlpha;
		ofParameter<bool>			bPerformer;
		ofParameter<bool>			bOmbre;


    np::HomographyHelper    homography;    

    np::ThreadedBuffer          buffer;
    np::BufferedOperations      performerOp;


private:
    ContoursOperator * op;
    
    int     alphaStep;

    ofFbo   fbo;

};
	
}
