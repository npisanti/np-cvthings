
#pragma once

#include "ofMain.h"
#include "../ContoursOperator.h"
#include "np-junkrepo.h"
#include "../../buffers/ThreadedBuffer.h"

namespace np {
namespace ombre {

class Delays {
	
public:
	Delays();
    
    void operations( const np::ThreadedBuffer & buffer, vector<vector<np::CvContour>> & ombre );
    
	ofParameterGroup ui;
		ofParameter<float>  fade;
		ofParameter<int>	frames;
};

}
}
