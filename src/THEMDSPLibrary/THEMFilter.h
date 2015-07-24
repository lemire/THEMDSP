// THEMFilter.h: interface for the THEMFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_THEMFILTER_)
#define _THEMFILTER_

#include "Signal.h"
#include "BinaryFileSignal.h"
#include "SIMn.h"
#include "Downsampling.h"
#include "Despike.h"
#include "TauProjection.h"

class THEMFilter : public BinaryFileSignal
{
public:
	/*
	* 
	* Arguments:
	*	FrameLength : usually 512 
	*	filename : name of the binary file to be read or created
	*	readit : should I read the data in the file? if false, will overwrite the file
	*	baseline : should baseline be corrected?
	*	downsampling : number of frames over which you want to stack (0 or 1 = disable the downsampling)
	*	DespikeCoefficient : "wavelet shrinkage" to remove peaks
	*/
	THEMFilter(const int FrameLength, char * filename, const bool readit, const bool baselinecorrection = true, const int downsampling = 1,const float DespikeCoefficient = 100.0f, const bool tau = true, const bool integrate = false);
	virtual ~THEMFilter();
	/*
	* if the downsampling is activated, the frames will be stored
	* on a local buffer.
	*/
	virtual void addNewFrame(short * FrameSignal);


	int getDownsamplingSize() const {
		return mDownsamplingSize;
	}
private:

	SIMn * mSIMn;
	Downsampling * mDownsampling;
	Despike * mDespike;
	TauProjection * mTauProjection;
	short * mDownsamplingBuffer;
	int mDownsamplingSize, mDownsamplingIndex;
};

#endif // !defined(_THEMFILTER_)
