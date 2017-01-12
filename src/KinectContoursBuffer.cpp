
#include "KinectContoursBuffer.h"

np::KinectContoursBuffer::KinectContoursBuffer(){
    
    // setup ui
    ui.setName("buff kinect contours");
    ui.add( bHoles.set("find holes", false ));    
    ui.add( nearThreshold.set("nearThresh", 230, 0, 255));
    ui.add( farThreshold.set("farThresh", 10, 0, 255));
    ui.add( minArea.set("minArea", 1000, 0, 5000));
    ui.add( maxArea.set("maxArea", 70000, 15000, 150000));
    ui.add( threshold.set("finder thresh", 15, 1, 100));
    ui.add( persistence.set("tracker persistence", 15, 1, 100));
    ui.add( maxDistance.set("tracker maxDistance", 32, 1, 100));  
         
    ui.add( bSimplify.set("simplify", false) );
    ui.add( simplifyAmount.set("tolerance", 0.0f, 0.0f, 1.0f) );
    msec = 6;
    
    readIndex = 0;
    writeIndex = 0;
}

void np::KinectContoursBuffer::setup( int bufferSize, int screenW, int screenH ) {

    this->bufferSize = bufferSize;
    buffer.resize( bufferSize );
    
    if(screenW != 640 || screenH != 480){
        bExpand = true;
    }else{
        bExpand = false;
    }
    
    this->screenW = screenW;
    this->screenH = screenH;
    
    // set up kinect
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    startThread();

}

void np::KinectContoursBuffer::threadedFunction() {
    
    while(isThreadRunning()) {
        
        kinect.update();
        
        if(kinect.isFrameNew()) {

            contoursMutex.lock();
            
            // process kinect depth image
            grayImage.setFromPixels(kinect.getDepthPixels().getData(), kinect.width, kinect.height);
            grayThreshNear = grayImage;
            grayThreshFar = grayImage;
            grayThreshNear.threshold(nearThreshold, true);
            grayThreshFar.threshold(farThreshold);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
            grayImage.flagImageChanged();
            
            // set contour tracker parameters
            contourFinder.setMinArea(minArea);
            contourFinder.setMaxArea(maxArea);
            contourFinder.setThreshold(threshold);
            contourFinder.setFindHoles( bHoles ); 
            
            contourFinder.getTracker().setPersistence(persistence);
            contourFinder.getTracker().setMaximumDistance(maxDistance);
            
            // determine found contours
            contourFinder.findContours(grayImage);
            
            int b = writeIndex + 1;
            if( b == bufferSize ) b=0;
            
            vector<CvContour> & contours = buffer[b]; // assigning a reference, no copy

            contours.resize( contourFinder.size() );
            
            for(int i = 0; i < contourFinder.size(); i++) {
                
                contours[i].contour = contourFinder.getPolylines()[i];

                if(bSimplify) contours[i].contour.simplify( simplifyAmount );

                if(bExpand){
                    vector<ofPoint> & vertices = contours[i].contour.getVertices();
                    for (int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++) {
                        vertices[vertexIndex].x = ofMap( vertices[vertexIndex].x, 0, 640, 0, screenW);
                        vertices[vertexIndex].y = ofMap( vertices[vertexIndex].y, 0, 480, 0, screenH);
                    }
                }
                
                contours[i].hole = contourFinder.getHole(i);
                contours[i].label = contourFinder.getLabel(i);
            }   
            
            writeIndex = b;        
                      
            contoursMutex.unlock();
        }
        
        sleep( msec );
    }
}    


void np::KinectContoursBuffer::exit() {
    contoursMutex.lock();
        stopThread();
    contoursMutex.unlock();
}


bool np::KinectContoursBuffer::update() {
    if( readIndex != writeIndex ){
        readIndex = writeIndex;
        return true;
    }else{
        return false;
    }
}

const vector<np::CvContour> & np::KinectContoursBuffer::delay( int i, int frames ) const {
    int n = readIndex - i*frames;
    while( n  < 0 ) n += bufferSize;
    return buffer[n];    
}

const vector<np::CvContour> & np::KinectContoursBuffer::now() const {
    return buffer[readIndex];    
}

void np::KinectContoursBuffer::drawTestingImage( int x, int y ) {
    contoursMutex.lock();
        grayImage.draw(x, y);
    contoursMutex.unlock();
}

