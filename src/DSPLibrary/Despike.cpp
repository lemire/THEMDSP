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
#include "Despike.h"
#include "SpecialMath.h"
#include "VFastSymmlet4.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// recommended value for threshold is 100.0f
Despike::Despike(const float threshold, const int offtimewidth, const int ignoreofftimewidth, const int safetymargin, const int numberofiterations)
{
	if(threshold < 0)
		throw NegativeThresholdException();
	if(offtimewidth < 4)
		throw OffTimeWidthTooSmallException();
	if(ignoreofftimewidth < 0)
		throw NegativeIgnoreOffTimeWidthException();
	if(numberofiterations < 0)
		mNumberOfIterations = 2; // ignore value, just set it to default
	else
		mNumberOfIterations = numberofiterations;
	mThreshold = threshold;
	mOffTimeWidth = (offtimewidth >> mNumberOfIterations) << mNumberOfIterations;//make sure it is divisible by 4	
	mIgnoreOffTimeWidth = ignoreofftimewidth; 
	mFrameLength = 0;//defaut value
	if(safetymargin < 0)
		mSafetyMargin = 8;// ignore this, no warning, just set it to default
	else 
		mSafetyMargin = safetymargin;
}

Despike::~Despike()
{

}

// remove the spikes
void Despike::process(Signal& signal) 
{
	signal.reset();
	mFrameLength = signal.getFrameLength();
	if(mFrameLength < mOffTimeWidth + mIgnoreOffTimeWidth)
		throw OffTimeWidthTooLargeException();
	while(signal.hasNextFrame()) {
		short * currentframe = signal.nextFrame();
		filterSpikes(currentframe,mFrameLength);
		signal.commit();
	}

}


void Despike::filterSpikes (short * signal, const int FrameLength) {
	//
	// We need a buffer
	//
	float * buffer = new(std::nothrow) float[mOffTimeWidth];
	if(buffer == NULL)
		throw OutOfMemoryException();
	int OffTimeStart = FrameLength - mOffTimeWidth - mIgnoreOffTimeWidth;

	for(int l = OffTimeStart; l <FrameLength - mIgnoreOffTimeWidth ; l++) {
			buffer[l - OffTimeStart] = (float) signal [l]; 
	}
	VFastSymmlet4::transform(mNumberOfIterations,mOffTimeWidth, buffer);
	
	for(l = mOffTimeWidth >> mNumberOfIterations; l < mOffTimeWidth; l++) {

			buffer[l] = buffer[l] > mThreshold ? 0 : buffer[l];
			buffer[l] = buffer[l] < -mThreshold ? 0 : buffer[l];
	}
	VFastSymmlet4::invTransform(mNumberOfIterations,mOffTimeWidth, buffer);			
	for(l = OffTimeStart + mSafetyMargin; l < FrameLength - mIgnoreOffTimeWidth - mSafetyMargin; l++) {
		signal[l] = SpecialMath::roundToShort(buffer[l - OffTimeStart]); 
	}
	//
	// Garbage collection
	//
	delete[] buffer;
	buffer = NULL;
}
