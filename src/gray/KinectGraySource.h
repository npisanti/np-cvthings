
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include <atomic>
#include "GrayImageSource.h"

namespace np{

class KinectGraySource : public GrayImageSource {
	
public:
	void setup() override;
	void update() override;
	bool isFrameNew()override;
	unsigned char * getPixels() override;
	
	float getWidth() override;
	float getHeight() override;
	
	ofxKinect kinect;
	
};
	
}
