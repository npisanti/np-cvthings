
#include "CalibratedKinectContours.h"

np::CalibratedKinectContours::CalibratedKinectContours(){
    
    // setup ui
    ui.setName("kinect contours");
    ui.add(nearThreshold.set("nearThresh", 230, 0, 255));
    ui.add(farThreshold.set("farThresh", 10, 0, 255));
    ui.add(minArea.set("minArea", 1000, 0, 5000));
    ui.add(maxArea.set("maxArea", 70000, 15000, 150000));
    ui.add(threshold.set("threshold", 15, 1, 100));
    ui.add(persistence.set("persistence", 15, 1, 100));
    ui.add(maxDistance.set("maxDistance", 32, 1, 100));       
    
    toReturn.clear();
    
    usec = 250;
}

void np::CalibratedKinectContours::setup( int projectorW, int projectorH, string calibrationXmlPath, int usec) {
    
    this->usec = usec; 
    this->projectorH = projectorH;
    this->projectorW = projectorW;
    
    // set up kinect
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    kpt.loadCalibration( calibrationXmlPath );

    startThread();
}


void np::CalibratedKinectContours::threadedFunction() {
    
    while(isThreadRunning()) {
        
        kinect.update();
        
        if(kinect.isFrameNew()) {
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
            contourFinder.getTracker().setPersistence(persistence);
            contourFinder.getTracker().setMaximumDistance(maxDistance);
            
            // determine found contours
            contourFinder.findContours(grayImage);
            
            contoursMutex.lock();
            
            contours.resize( contourFinder.size() );
                
                for(int i = 0; i < contourFinder.size(); i++) {
                    
                    vector<cv::Point> points = contourFinder.getContour(i);
                    contours[i].clear();
                    
                    for (int j=0; j<points.size(); j++) {
                        ofVec3f worldPoint = kinect.getWorldCoordinateAt(points[j].x, points[j].y);
                        ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);
                        contours[i].addVertex( projectorW * projectedPoint.x, projectorH * projectedPoint.y );
                    }
                }
                
            contoursMutex.unlock();
            
        }
        usleep( usec );
    }
}    


void np::CalibratedKinectContours::exit() {
    stopThread();
}

const vector<ofPolyline> & np::CalibratedKinectContours::getContours() {
    contoursMutex.lock();
    toReturn = contours;
    contoursMutex.unlock();
    return toReturn;
}
