#include "ofApp.h"

int main() {
    
	ofGLWindowSettings settings;
	settings.width = 640 + 320;
    settings.height = 480 + 240;
    settings.setGLVersion(3,2);
	ofCreateWindow(settings);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
