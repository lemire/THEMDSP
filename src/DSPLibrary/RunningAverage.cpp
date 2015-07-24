#include "RunningAverage.h"
#include <math.h>
#include "PrecompHeaders.h"
#include "SpecialMath.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RunningAverage::RunningAverage(const int WindowsLength)
{
	mWindowsLength = WindowsLength;
	mChannelLength = 0;
}

RunningAverage::~RunningAverage()
{

}

void RunningAverage::processChannelSet(ChannelSet& cs) {
	mChannelLength = cs.getChannelLength();
	float * buffer = NULL;
	try {
		buffer = new float[mChannelLength];
		for(int k = 0; k < cs.getNumberOfChannels() ;k++) {
			run(cs.getChannel(k), buffer);
		}
	} catch (...) {
		delete[] buffer;
		buffer = NULL;
		throw;
	}
	delete[] buffer;
	buffer = NULL;
}



void RunningAverage::run(float * channel, float * buffer) {
	
	for(int k = 0 ; k < mChannelLength; ++k) {
		const int begin = SpecialMath::max(k - mWindowsLength, 0);
		const int end = SpecialMath::min(k + mWindowsLength, mChannelLength - 1);
		buffer[k] = SpecialMath::mean(end + 1 - begin, channel + begin);
	}
	memcpy(channel,buffer,mChannelLength * sizeof(float));
}