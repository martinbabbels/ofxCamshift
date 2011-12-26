#include "ofxCamshiftMoments.h"

ofxCamshiftMoments::ofxCamshiftMoments(ofPixels *oPixels, bool bSecond=false) {
	m00			= 0.0;
	m10			= 0.0;
	m01			= 0.0;
	m11			= 0.0;
	m20			= 0.0;
	m02			= 0.0;
	mu00		= 0.0;
	mu10		= 0.0;
	mu01		= 0.0;
	mu11		= 0.0;
	mu20		= 0.0;
	mu02		= 0.0;
	invM00		= 0.0;
	
	xc = 0.0;
	yc = 0.0;
	
	int x,y,val;
	for(x = 0; x < oPixels->getWidth(); ++x) {
		for(y = 0; y < oPixels->getHeight(); ++y) {
			val = oPixels->getColor(x,y).r;
			
			m00 += val;
			m01 += y * val;
			m10 += x * val;
			if (bSecond) {
				m11 += x * y * val;
				m02 += y * y * val;
				m20 += x * x * val;
			}
		}
	}
	
	invM00 = 1.0 / m00;
	xc = m10 * invM00;
	yc = m01 * invM00;
	mu00 = m00;
	mu01 = mu10 = 0.0;
	
	if (bSecond) {
		mu20 = m20 - m10 * xc;
		mu02 = m02 - m01 * yc;
		mu11 = m11 - m01 * xc;
	}
}