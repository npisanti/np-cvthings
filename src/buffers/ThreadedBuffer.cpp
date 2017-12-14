
#include "ThreadedBuffer.h"


// translate mode are: 0 = no translate, 1 = expand, 2 = cv calibrate

np::ThreadedBuffer::ThreadedBuffer(){
    
    // setup ui
    ui.setName("threaded buffer");
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


void np::ThreadedBuffer::setup( int bufferSize, GrayImageSource & gray, int projectorW, int projectorH ) {
    translateMode = 1;
    calculateAspect ( projectorW, projectorH );
    setup( bufferSize, gray );
}

void np::ThreadedBuffer::setup( int bufferSize, GrayImageSource & gray, ofMatrix4x4 & homography ) {
    translateMode = 2;
    hMatrix = &homography;
    calculateAspect ( 640, 480 );
    setup( bufferSize, gray );
}


void np::ThreadedBuffer::setup( int bufferSize, GrayImageSource & gray ) {

	this->gray = &gray;
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
    
    this->gray->setup();

    grayImage.allocate(this->gray->getWidth(), this->gray->getHeight());
    grayThreshNear.allocate(this->gray->getWidth(), this->gray->getHeight());
    grayThreshFar.allocate(this->gray->getWidth(), this->gray->getHeight());
    
    cout<<"starting thread\n";
    startThread();
}

void np::ThreadedBuffer::threadedFunction() {
    
    while(isThreadRunning()) {
        
        gray->update();
        
        if(gray->isFrameNew()) {

            contoursMutex.lock();
            
            // process kinect depth image
            grayImage.setFromPixels( gray->getPixels(), gray->getWidth(), gray->getHeight());
            
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

				//cout<<"translate\n";
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
                            ofPoint hp = ofxHomography::toScreenCoordinates( ofPoint(vertices[vertexIndex].x, vertices[vertexIndex].y), *hMatrix);
                            //ofPoint hp(0, 0);
                            vertices[vertexIndex].x = hp.x;
                            vertices[vertexIndex].y = hp.y;
                        }
                        
                    }
                    break;

                    default: break;
                }

                contours[i].hole = contourFinder.getHole(i);
                contours[i].label = contourFinder.getLabel(i);
                contours[i].velocity = ofxCv::toOf(contourFinder.getVelocity(i));
                ofPoint center = contours[i].contour.getCentroid2D();
                contours[i].center.x = center.x;
                contours[i].center.y = center.y;
            }   
            
            writeIndex = b;        
                      
            contoursMutex.unlock();
        }
        
        sleep( msec );
    }
}    


void np::ThreadedBuffer::exit() {
    contoursMutex.lock();
        waitForThread(true, 800);
    contoursMutex.unlock();
}


bool np::ThreadedBuffer::update() {
    if( readIndex != writeIndex ){
        readIndex = writeIndex;
        return true;
    }else{
        return false;
    }
}

const vector<np::CvContour> & np::ThreadedBuffer::delay( int frames, int i ) const {
	i = (i>0) ? i : 0;
    int n = readIndex - i*frames;
    while( n  < 0 ) n += bufferSize;
    return buffer[n];    
}

const vector<np::CvContour> & np::ThreadedBuffer::now() const {
    return buffer[readIndex];    
}

void np::ThreadedBuffer::drawTestingImage( int x, int y ) {
    contoursMutex.lock();
        grayImage.draw(x, y);
    contoursMutex.unlock();
}

const ofImage & np::ThreadedBuffer::difference() const {
    return differenceBuffer[diffRead];
}   

const vector<float> & np::ThreadedBuffer::differenceCols() const {
    return differenceColumns[diffRead];
}

int np::ThreadedBuffer::getWidth() {
    return projectorW;
}

void np::ThreadedBuffer::calculateAspect( int w, int h ) {
    projectorH = h;
    projectorW = h * (4.0f/3.0f);
    projectorOff = (w - projectorW)/2;
    projectorX0 = projectorOff;
    projectorX1 = projectorOff + projectorW;
}
