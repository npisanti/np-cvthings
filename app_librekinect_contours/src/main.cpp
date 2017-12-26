
#include "ofApp.h"

int main() {
    
    ofGLFWWindowSettings settings;
    
    settings.width = 640 + 240;
    settings.height = 480;
    settings.setPosition(ofVec2f(0, 0));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    shared_ptr<ofApp> mainApp(new ofApp);
    
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
    
}
