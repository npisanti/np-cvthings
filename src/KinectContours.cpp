
#include "KinectContours.h"

np::KinectContours::KinectContours(){
    
    // setup ui
    ui.setName("kinect contours");
    ui.add( nearThreshold.set("nearThresh", 230, 0, 255));
    ui.add( farThreshold.set("farThresh", 10, 0, 255));
    ui.add( minArea.set("minArea", 1000, 0, 5000));
    ui.add( maxArea.set("maxArea", 70000, 15000, 150000));
    ui.add( threshold.set("finder thresh", 15, 1, 100));
    //ui.add( persistence.set("persistence", 15, 1, 100));
    //ui.add( maxDistance.set("maxDistance", 32, 1, 100));       
    ui.add( bHoles.set("find holes", false ));       
    
    toReturn.clear();
    
    msec = 6;
    
    bNew = false;
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
            //contourFinder.getTracker().setPersistence(persistence);
            //contourFinder.getTracker().setMaximumDistance(maxDistance);
            
            // determine found contours
            contourFinder.findContours(grayImage);
            
            if(bExpand){
                contours.resize( contourFinder.size() );
                for(int i = 0; i < contourFinder.size(); i++) {
                    vector<cv::Point> points = contourFinder.getContour(i);
                    contours[i].clear();
                    
                    for (int j=0; j<points.size(); j++) {
                        float x = ofMap(points[j].x, 0, 640, 0, screenW);
                        float y = ofMap(points[j].y, 0, 480, 0, screenH);
                        contours[i].addVertex( x, y );
                    }
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


const vector<ofPolyline> & np::KinectContours::getContours() {
    contoursMutex.lock();
        if(bExpand){
            toReturn = contours;
        }else{
            toReturn = contourFinder.getPolylines();
        }
    contoursMutex.unlock();
    bNew = false;
    return toReturn;
}


void np::KinectContours::drawTestingImage( int x, int y ) {
    contoursMutex.lock();
        grayImage.draw(x, y);
    contoursMutex.unlock();
}

