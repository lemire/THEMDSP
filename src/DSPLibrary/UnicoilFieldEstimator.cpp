///////////////////////////////////////////////////////////////////
//
// (c) 1999, 2000 Daniel Lemire, Ph.D. (Daniel.Lemire@Videotron.ca)
// for THEM Geophysics. Please call if you have any problem
// with this code (514) 524-3517.
//
// http://www.techelements.com/
// http://www.ondelette.com/
//
// This code shouldn't not be used for any other purpose but
// to process THEM Geophysics signals without my consent.
// 
////////////////////////////////////////////////////////////////////

#include "PrecompHeaders.h"
#include "UnicoilFieldEstimator.h"
#include "SpecialMath.h"
#include "Signal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UnicoilFieldEstimator::UnicoilFieldEstimator(const double sigma)
{
	mSigma = sigma;
}

UnicoilFieldEstimator::~UnicoilFieldEstimator()
{

}




/**
* process the given signal
* inputs:
*	signal - the actual signal 
*/
void UnicoilFieldEstimator::process(Signal& signal)
{
	signal.reset();
	mFrameLength = signal.getFrameLength();
	while (signal.hasNextFrame()) {
		short * frame = signal.nextFrame();
		computeField(frame);
		signal.commit();
	}

}




/****************
* Attempt to estimate the field
* values from unicoil data
* inputs:
*	currentframe - a frame to be processed
****************/
void UnicoilFieldEstimator::computeField(short * currentframe) {
	int somme = 0;
	for ( int k = 0; k < mFrameLength ; k++) {
		somme += currentframe[k];
		currentframe[k] = SpecialMath::roundToShort (somme * mSigma + currentframe[k]);
//		currentframe[k] = SpecialMath::roundToShort ((currentframe[k] + currentframe[k - 1]) / 2.0 + mSigma * (currentframe[k] - currentframe[k - 1]));
	}
//	currentframe[0] = currentframe[1];
}
