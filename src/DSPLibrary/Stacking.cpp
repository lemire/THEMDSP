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
#include "Stacking.h"
#include "SpecialMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Stacking::Stacking(const int NumberOfFramesToStack, const bool invertsigns)
{
	if(NumberOfFramesToStack <= 0)
		throw IllegalNumberOfFramesException();
	mNumberOfFramesToStack = NumberOfFramesToStack;
	mInvertSigns = invertsigns;

}

Stacking::~Stacking()
{

}

// stack the signal
void Stacking::process(Signal& signal) {
	signal.reset();
	int FrameLength = signal.getFrameLength();
	short * buffer = new(std::nothrow) short[mNumberOfFramesToStack * FrameLength];
	if (buffer == NULL)
		throw OutOfMemoryException();
	int buffersize = 0;
	while(signal.hasNextFrame()) {
		short * currentFrame = signal.nextFrame();
		if(buffersize + FrameLength < mNumberOfFramesToStack * FrameLength) {
			// fill the buffer and do nothing
			memcpy(buffer + buffersize,currentFrame,FrameLength * sizeof(short));
			buffersize += FrameLength;
		} else  {
			if (buffersize + FrameLength == mNumberOfFramesToStack * FrameLength){
				// fill the buffer one last time
				assert(FrameLength + buffersize == mNumberOfFramesToStack * FrameLength);
				memcpy(buffer + buffersize,currentFrame,FrameLength * sizeof(short));
				buffersize += FrameLength;			
				assert(buffersize == mNumberOfFramesToStack * FrameLength);
			} else {
				// move the buffer around
				assert(buffersize == mNumberOfFramesToStack * FrameLength);
				memcpy(buffer , buffer + FrameLength, (mNumberOfFramesToStack - 1) * FrameLength * sizeof(short));
				memcpy(buffer + (mNumberOfFramesToStack - 1) * FrameLength, currentFrame, FrameLength * sizeof(short));
			}
			// update the frame
			for(int k = 0; k < FrameLength ; k++) {
				double temp = 0.0;
				int sign = 1;
				if((mNumberOfFramesToStack/2) * 2 == mNumberOfFramesToStack)
					sign *= -1;
				for (int l = 0 ; l < mNumberOfFramesToStack ; l++ ) {
					temp += buffer[k + l * FrameLength] * sign;
					if(mInvertSigns)
						sign *= -1;
				}
				
				temp /= mNumberOfFramesToStack;
				currentFrame[k] = SpecialMath::roundToShort( temp );
			}
			signal.commit();
		}
	}
}
