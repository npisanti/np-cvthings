
#include "KinectContours.h"

np::KinectContours::KinectContours(){
    
    // setup ui
    ui.setName("kinect contours");
    ui.add( bHoles.set("find holes", false ));    
    ui.add( nearThreshold.set("nearThresh", 230, 0, 255));
    ui.add( farThreshold.set("farThresh", 10, 0, 255));
    ui.add( minArea.set("minArea", 1000, 0, 5000));
    ui.add( maxArea.set("maxArea", 70000, 15000, 150000));
    ui.add( threshold.set("finder thresh", 15, 1, 100));
    ui.add( persistence.set("tracker persistence", 15, 1, 100));
    ui.add( maxDistance.set("tracker maxDistance", 32, 1, 100));       
   
    msec = 6;
    
    bNew = false;
    circularMax = 6;
    circularBuffer.resize( circularMax );
    circularIndex = 0;
}

void np::KinectContours::setup( int screenW, int screenH ) {
    
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

void np::KinectContours::threadedFunction() {
    
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
            
            circularIndex++;
            if(circularIndex == circularMax ) circularIndex = 0;
            vector<CvContour> & contours = circularBuffer[circularIndex]; // assigning a reference, no copy
            
            if(bExpand){
                contours.resize( contourFinder.size() );
                for(int i = 0; i < contourFinder.size(); i++) {
                    vector<cv::Point> points = contourFinder.getContour(i);
                    contours[i].contour.clear();
                    contours[i].hole = contourFinder.getHole(i);
                    contours[i].label = contourFinder.getLabel(i);
                    
                    for (int j=0; j<points.size(); j++) {
                        float x = ofMap(points[j].x, 0, 640, 0, screenW);
                        float y = ofMap(points[j].y, 0, 480, 0, screenH);
                        contours[i].contour.addVertex( x, y );
                    }
                }
            }else{
                contours.resize( contourFinder.size() );
                for(int i = 0; i < contourFinder.size(); i++) {
                    contours[i].contour = contourFinder.getPolylines()[i];
                    contours[i].hole = contourFinder.getHole(i);
                    contours[i].label = contourFinder.getLabel(i);
                }                
            }
            
            bNew = true;           
                      
            contoursMutex.unlock();
            
        }
        sleep( msec );
    }
}    


void np::KinectContours::exit() {
    contoursMutex.lock();
        stopThread();
    contoursMutex.unlock();
}


bool np::KinectContours::hasNewContours() {
    return bNew;
}


const vector<np::CvContour> & np::KinectContours::getContours() {
    contoursMutex.lock();
        int index = circularIndex;
    contoursMutex.unlock();
    bNew = false;
    return circularBuffer[index];
}


void np::KinectContours::drawTestingImage( int x, int y ) {
    contoursMutex.lock();
        grayImage.draw(x, y);
    contoursMutex.unlock();
}

