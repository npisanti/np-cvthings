
#include "KinectGraySource.h"


void np::KinectGraySource::setup() {
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
}

void np::KinectGraySource::update() {
	kinect.update();
}

bool np::KinectGraySource::isFrameNew() {
	return kinect.isFrameNew();
}

unsigned char * np::KinectGraySource::getPixels() {
	return kinect.getDepthPixels().getData();
}

float np::KinectGraySource::getWidth() {
	return kinect.getWidth();
}

float np::KinectGraySource::getHeight() {
	return kinect.getHeight();
}
