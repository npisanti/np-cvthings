
#include "Shadows.h"

void np::Shadows::setup( int pw, int ph, np::GrayImageSource & source, ContoursOperator * op ) {
	
	this->op = op;
	ombre.reserve(24);
	
    fbo.allocate( pw, ph );
    fbo.begin();
        ofClear(0, 0, 0, 0);
    fbo.end();
    
    ombreMasker.setup( pw, ph );
    performerMasker.setup( pw, ph );
    
	homography.setup( pw, ph );
	homography.originalCorners[0].set( 0, 0 );
	homography.originalCorners[1].set( source.getWidth(), 0 );
	homography.originalCorners[2].set( source.getWidth(), source.getHeight() );
	homography.originalCorners[3].set( 0, source.getHeight() );
	homography.calculateHomography();

    //buffer.setup( 9000, source, pw, ph );
    buffer.setup( 9000, source, homography.homography );
    
    graphics.setName("alpha");    
    graphics.add( bOmbre.set("ombre", true) );
    graphics.add( ombreAlpha.set("ombre alpha start", 255, 0, 255));
    graphics.add( ombreContourAlpha.set("ombre cont alpha", 255, 0, 255));
    graphics.add( bPerformer.set("performer", true) );
    graphics.add( performerAlpha.set("performer alpha", 255, 0, 255));
    graphics.add( performerContourAlpha.set("performer cont alpha", 255, 0, 255));

    performerOp.ui.setName("performers operations");
        	
}

void np::Shadows::addToGui( ofxPanel & gui ) {

    gui.add( graphics );
    gui.add( performerOp.ui );   
    gui.add( buffer.ui );
    gui.add( homography.ui );
    
}

void np::Shadows::update( const ofColor & performerColor, const ofColor & contoursColor ) {

	// there is a new contour and we are connected
	if( buffer.update() ) {    
                
        if( bPerformer ) performerOp.update( 0, buffer.now() );
        
        if( bOmbre ) op->operations( buffer, ombre );
 
        // ---------- code for masking the fill ----------
		if( bOmbre ) {
			ombreMasker.mask.begin();
				ofClear(0, 0, 0, 0);
				ofFill();                    
				
				// older delays have a darker color
				for ( int i=ombre.size()-1; i>=0; --i ) {
					for( auto & cont : ombre[i] ) {
						ofSetColor( ombreAlpha * cont.alpha );               
						if ( !cont.hole ) cont.draw();
					}                 
				}
				for ( int i=ombre.size()-1; i>=0; --i ){
					ofSetColor( 0 );  
					for( auto & cont : ombre[i] ) {                    
						if ( cont.hole ) cont.draw();
					}                 
				}
			ombreMasker.mask.end();
		}

		if( bPerformer ) {
			performerMasker.mask.begin();
				ofClear(0, 0, 0, 0);
				ofFill();  
				ofSetColor( 255, performerAlpha );
				for( auto & cont : performerOp.buffer[0] ){ 
					if ( !cont.hole ) cont.draw(); 
				}  
				ofSetColor( 0 );  
				for( auto & cont : performerOp.buffer[0] ) {                    
					if ( cont.hole ) cont.draw();
				}   
			performerMasker.mask.end();						
		}
        // ---------------- finished fill masking ----------
    }

	fbo.begin();
		ofClear(0, 0, 0, 0);
		
		if( bOmbre ){
			ombreMasker.draw(0, 0);

			for ( int i=ombre.size()-1; i>=0; --i ){ 
				ofNoFill();
				for( auto & cont : ombre[i] ){
					ofSetColor( contoursColor, ombreContourAlpha * cont.alpha );
					cont.draw();
				}    
			}    			
		} 
		
		if( bPerformer ) {
			performerMasker.draw(0, 0);

			ofNoFill();
			ofSetColor( performerColor, performerContourAlpha );
			for( auto & cont : performerOp.buffer[0] ){ cont.draw(); }    						
		}
	fbo.end();
    
}

void np::Shadows::draw( int x, int y ) {
	if( bOmbre || bPerformer ) fbo.draw( x, y );
}

void np::Shadows::exit() {
	buffer.exit();
}
