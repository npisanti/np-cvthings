
#pragma once

#include "np-junkrepo.h"
#include "../buffers/ThreadedBuffer.h"

namespace np{
	
class ContoursOperator {
public: 
	virtual void operations( const np::ThreadedBuffer & buffer, vector<vector<np::CvContour>> & ombre ) { 
		ombre.resize(1);
		ombre[0] = buffer.now(); 
	};
};
	
}
