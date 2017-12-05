
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include <atomic>
#include "GrayImageSource.h"

// kinect on linux using librekinect kernel module

namespace np{

class LibreKGraySource : public GrayImageSource {
	
public:
	LibreKGraySource();
	~LibreKGraySource();
	
	void setup() override;
	void setup( int id );
	void update() override;
	bool isFrameNew()override;
	unsigned char * getPixels() override;
	
	float getWidth() override;
	float getHeight() override;

private:
	ofVideoGrabber kinect;
	unsigned char * pixels;
	atomic<bool> bNew;
	
};
	
}
