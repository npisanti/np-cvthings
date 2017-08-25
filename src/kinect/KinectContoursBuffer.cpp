
#include "KinectContoursBuffer.h"


// translate mode are: 0 = no translate, 1 = expand, 2 = cv calibrate

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
    ui.add( bMirror.set("mirror", false ) );
    msec = 6;
    
    readIndex = 0;
    writeIndex = 0;
    
    translateMode = 0;
    projectorOff = 0;
    
    contourFinder.setTargetColor( ofColor(255) );
    
}

void np::KinectContoursBuffer::setup( int bufferSize, int projectorW, int projectorH ) {
    translateMode = 1;
    calculateAspect ( projectorW, projectorH );
    setup( bufferSize );
}

void np::KinectContoursBuffer::setup( int bufferSize, int projectorW, int projectorH, string calibrationXmlPath  ) {
    translateMode = 2;
    this->projectorH = projectorH;
    this->projectorW = projectorW;
    kpt.loadCalibration( calibrationXmlPath );
    setup( bufferSize );
}


void np::KinectContoursBuffer::setup( int bufferSize  ) {

    this->bufferSize = bufferSize;
    buffer.resize( bufferSize );
    
    // frame difference buffer
    differenceBuffer.resize( framediffMax );
    differenceColumns.resize( framediffMax );
    for( size_t i=0; i<framediffMax; ++i ){
        differenceBuffer[i].allocate( 640, 480, OF_IMAGE_GRAYSCALE);
        differenceColumns[i].resize( projectorW );
    }
    ofxCv::imitate(previous, differenceBuffer[0]); 
    diffRead = 0;
    diffWrite = 0;
    
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
            
            if( bMirror ) grayImage.mirror( false, true );
            
            grayThreshNear = grayImage;
            grayThreshFar = grayImage;
            grayThreshNear.threshold(nearThreshold, true);
            grayThreshFar.threshold(farThreshold);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
            grayImage.flagImageChanged();
            
            
            diffWrite++;
            if(diffWrite==framediffMax) diffWrite = 0;
            
            ofxCv::absdiff(grayImage, previous, differenceBuffer[diffWrite]);
            differenceBuffer[diffWrite].update();
            ofxCv::copy(grayImage, previous);
            
            columnMean = ofxCv::meanCols( differenceBuffer[diffWrite] );
            
            
            /*
            for(int k = 0; k < projectorW; k++) {
                int i = ofMap( k, 0, projectorW, 0, 640 );
                differenceColumns[diffWrite][i] = ofMap( columnMean.at<uint8_t>(i, 0), 0, UINT8_MAX, 0.0f, 1.0f, true );            
            }
            */
            for(int i = 0; i < 640; i++) {
                differenceColumns[diffWrite][i] = ofMap( columnMean.at<uint8_t>(i, 0), 0, UINT8_MAX, 0.0f, 1.0f, true );            
            }
            
            diffRead = diffWrite;
            
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

                switch( translateMode ){
                    case 0: break; // no translation
                    
                    case 1: // only translate, no calibration
                    {
                        vector<ofPoint> & vertices = contours[i].contour.getVertices();
                        for (int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++) {
                            vertices[vertexIndex].x = ofMap( vertices[vertexIndex].x, 0, 640, projectorX0, projectorX1);
                            vertices[vertexIndex].y = ofMap( vertices[vertexIndex].y, 0, 480, 0, projectorH);
                        }
                    }
                    break;
                    
                    case 2: // calibrated translate
                    {
                        vector<ofPoint> & vertices = contours[i].contour.getVertices();
                        for (int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++) {
                            ofVec3f worldPoint = kinect.getWorldCoordinateAt(vertices[vertexIndex].x, vertices[vertexIndex].y);
                            ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);
                            vertices[vertexIndex].x = projectorW * projectedPoint.x;
                            vertices[vertexIndex].y = projectorH * projectedPoint.y;
                        }
                    }
                    break;

                    default: break;
                }

                contours[i].hole = contourFinder.getHole(i);
                contours[i].label = contourFinder.getLabel(i);
                contours[i].velocity = ofxCv::toOf(contourFinder.getVelocity(i));
            }   
            
            writeIndex = b;        
                      
            contoursMutex.unlock();
        }
        
        sleep( msec );
    }
}    


void np::KinectContoursBuffer::exit() {
    contoursMutex.lock();
        waitForThread(true, 800);
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

const ofImage & np::KinectContoursBuffer::difference() const {
    return differenceBuffer[diffRead];
}   

const vector<float> & np::KinectContoursBuffer::differenceCols() const {
    return differenceColumns[diffRead];
}

int np::KinectContoursBuffer::getWidth() {
    return projectorW;
}

void np::KinectContoursBuffer::calculateAspect( int w, int h ) {
    projectorH = h;
    projectorW = h * (4.0f/3.0f);
    projectorOff = (w - projectorW)/2;
    projectorX0 = projectorOff;
    projectorX1 = projectorOff + projectorW;
}
