#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("contours operations");
    gui.setup("", "settings.xml");
    gui.add( contop.ui );
}

//--------------------------------------------------------------
void ofApp::update(){
    contop.update( contours );
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);
        
    gui.draw();
    
    ofSetColor( 255, 0, 0);
    for( auto & cont : contours) cont.draw();
    
    ofSetColor( 255, 90, 90 );
    for( auto & cont : contop.contours) cont.draw();
    
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case ' ': contours.clear();
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    index = contours.size();
    contours.resize(index+1);
    contours[index].clear();
    contours[index].addVertex(x, y);
    
}

void ofApp::mouseDragged(int x, int y, int button){
    contours[index].addVertex(x, y);
}

void ofApp::mouseReleased(int x, int y, int button){
    contours[index].close();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){ }
//--------------------------------------------------------------
