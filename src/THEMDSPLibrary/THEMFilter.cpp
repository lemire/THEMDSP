
#include "THEMFilter.h"
#include "ExtendedSIMn.h"
#include "Downsampling.h"
#include "ExponentialFitting.h"
#include "IntegratedTauProjection.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

THEMFilter::THEMFilter(const int FrameLength, char * filename, const bool readit, const bool baselinecorrection, const int downsampling, const float DespikeCoefficient, const bool tau, const bool integrate) : BinaryFileSignal( FrameLength, filename, readit)
{
	mSIMn = NULL;
	mDownsampling = NULL;
	mDownsamplingBuffer = NULL;
	mDownsamplingSize = downsampling;
	mDownsamplingIndex = 0;
	mTauProjection = NULL;
	if(tau) {
		try {
			//mTauProjection = new TauProjection();
			if(integrate)
				mTauProjection = new IntegratedTauProjection();
			else 
				mTauProjection = new TauProjection();
			//mTauProjection = new ExponentialFitting(256);
		} catch (...) {
		}
	}
	if(baselinecorrection) {
		try {
			mSIMn = new SIMn(3,48);
		} catch(...) {
			std::cout << "Unable to initialize baseline correction." <<std::endl;
		}
	}

	if(mDownsamplingSize > 1) {
		try {
			mDownsamplingBuffer = new short[mDownsamplingSize * mFrameLength];
			mDownsampling = new Downsampling();
		} catch (...) {
			std::cout << "Unable to initialize downsampling." <<std::endl;
			delete[] mDownsamplingBuffer;
			mDownsamplingBuffer = NULL;
		}
	}
	mDespike = NULL;
	if(DespikeCoefficient > 0) {
		try {
			mDespike = new Despike(DespikeCoefficient);
		} catch (...) {
		}

	}
	
}

void THEMFilter::addNewFrame(short * FrameSignal) {
	/*if( false ) {// this can be used to reverse signs... whatever
		for(int k = 0; k < mFrameLength ; ++k ) {
			FrameSignal[k] = - FrameSignal[k];
		}
	}*/
	/*
	* before everything else, we have to remove the
	* big spikes!
	*/
	if(mDespike != NULL) {
		mDespike->filterSpikes (FrameSignal, mFrameLength);
	}

	/*
	* First we try to correct the baseline.
	* Notice that it is a stateful algorithm.
	*/
	if(mSIMn != NULL) {
		if(getNumberOfFrames() == 0) {
			mSIMn->processFirstFrame(FrameSignal, mFrameLength);
		} else {
			mSIMn->processNextFrame(FrameSignal);
		}
	}

	if(mTauProjection) {
		//std::cout << "tau projection..." << std::endl;
		mTauProjection->project(FrameSignal, mFrameLength);
		//std::cout << "tau projection...ok" << std::endl;
	}
	/*
	* Then we downsample...
	*/	

	if(mDownsampling != NULL) {
		//std::cout << " downsampling... "<< std::endl;
		if( mDownsamplingIndex < mDownsamplingSize - 1 ) {
			memcpy(mDownsamplingBuffer + mDownsamplingIndex * mFrameLength, FrameSignal , mFrameLength * sizeof(short));
			++mDownsamplingIndex;
		} else if (mDownsamplingIndex == mDownsamplingSize - 1) {
			memcpy(mDownsamplingBuffer + mDownsamplingIndex * mFrameLength, FrameSignal , mFrameLength * sizeof(short));
			mDownsampling->makeOneFrameOutOfThese(mDownsamplingBuffer, mFrameLength, mDownsamplingSize);
			BinaryFileSignal::addNewFrame(mDownsamplingBuffer);
			mDownsamplingIndex = 0;
		}
		//std::cout << " downsampling... ok"<< std::endl;
	} else {
		BinaryFileSignal::addNewFrame(FrameSignal);
	}
}

THEMFilter::~THEMFilter()
{
	delete mSIMn;
	mSIMn = NULL;
	delete mDownsampling;
	mDownsampling = NULL;
	delete[] mDownsamplingBuffer;
	mDownsamplingBuffer = NULL;


}

