
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofConstants.h"
#include <atomic>

namespace np{

class GrayImageSource {
	
public:
	
	virtual void setup() = 0;
	virtual void update() = 0;
	virtual bool isFrameNew() = 0;
	virtual unsigned char * getPixels() = 0;
	
	virtual float getWidth() = 0;
	virtual float getHeight() = 0;
	
};
	
}
