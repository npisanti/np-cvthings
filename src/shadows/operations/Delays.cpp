
#include "Delays.h"

np::ombre::Delays::Delays() {
	ui.setName("ombre delays");
    ui.add( frames.set("delay frames", 12, 0, 60) );
    ui.add( fade.set("fade", 0.5f, 0.0f, 1.0f) );
}

void np::ombre::Delays::operations( const np::ThreadedBuffer & buffer, vector<vector<np::CvContour>> & ombre ) {
	
	for( size_t i=0; i<ombre.size(); ++i ) {
		ombre[i] = buffer.delay( frames, i+1 );
		float alpha = 1.0f - fade * ( static_cast<float>(i) / static_cast<float>(ombre.size()) );
		for( auto & cont : ombre[i] ) {
			cont.alpha = alpha;
		}
	}
}
