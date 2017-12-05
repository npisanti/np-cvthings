
#include "LibreKGraySource.h"

np::LibreKGraySource::LibreKGraySource() {
	pixels = nullptr;
	bNew = false;
}

np::LibreKGraySource::~LibreKGraySource() {
	delete [] pixels;
}

void np::LibreKGraySource::setup() {
	pixels = new unsigned char [640*480];
	int id = 1;
	vector<ofVideoDevice> devices = kinect.listDevices();

    for(int i = 0; i < devices.size(); i++){	
		if( devices[i].deviceName.compare("Xbox NUI Camera")==0 ){ id=i; }
    }
	setup( id );
}

void np::LibreKGraySource::setup( int id ) {
    kinect.setDeviceID( id );
    kinect.setDesiredFrameRate(60);
    kinect.initGrabber( 640, 480 );
}

void np::LibreKGraySource::update() {
	kinect.update();
    if ( kinect.isFrameNew() ) {
		// maybe we can do better with cv, but it is fast enough ATM
        ofPixels & lkPixels = kinect.getPixels();
        int max =lkPixels.size()/3;
        for(size_t i = 0; i <max; i++){
            pixels[i] = 255 - lkPixels[i*3];
        }
        bNew = true;
    }
}

bool np::LibreKGraySource::isFrameNew() {
	if( bNew ){
		bNew = false;
		return true;
	}else{
		return false;
	}
}

unsigned char * np::LibreKGraySource::getPixels() {
	return pixels;
}

float np::LibreKGraySource::getWidth() {
	return 640.0f;
}

float np::LibreKGraySource::getHeight() {
	return 480.0f;
}
