// RunningAverage.h: interface for the RunningAverage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_RUNNINGAVERAGE_H_)
#define _RUNNINGAVERAGE_H_

#include "ChannelProcessingObject.h"

class RunningAverage : public ChannelProcessingObject  
{
public:
	RunningAverage(const int WindowsLength);
	virtual ~RunningAverage();

	virtual void processChannelSet(ChannelSet& cs);

protected:
	virtual void RunningAverage::run(float * channel, float * buffer);

private: 
	int mWindowsLength, mChannelLength;

};

#endif // !defined(_RUNNINGAVERAGE_H_)
