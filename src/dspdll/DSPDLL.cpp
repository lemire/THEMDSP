
#include "DSPDLL.h"
#include "SimpleSignal.h"
#include "Despike.h"
#include "SIMn.h"
#include "Stacking.h"
#include "ChannelSet.h"
#include "WaveletShrinkage.h"

SimpleSignal * mSimpleSignal = NULL;
ChannelSet * mChannelSet = NULL;
short * mCopyOfSignalBuffer;

int __cdecl DSPDLL(const int FrameLength,char * inputfilename ,const int BeginningFrame, const int EndFrame, const int offset)
{
	if(mSimpleSignal!=NULL)
		return(-10);
	try {
		mSimpleSignal = new SimpleSignal(FrameLength,inputfilename,BeginningFrame,EndFrame,offset);
	} catch (Signal::NullSignalException) {
		return(-1);
	} catch (Signal::IllegalParameterException) {
		return(-2);
	} catch (Signal::IllegalNameException) {
		return(-3);
	} catch (Signal::NegativeSizeException) {
		return(-4);
	} catch (Signal::FileAccessException) {
		return(-5);
	} catch (OutOfMemoryException) {
		return(-1000);
	} catch (std::bad_alloc) {
		return(-2000);
	}
	mChannelSet = NULL;
	if ( mSimpleSignal == NULL )
		return(-6);
	return(0);
}

int __cdecl DSPDLLPointer(const int FrameLength,const int NumberOfFrame, short * signal)
{
	if(mSimpleSignal!=NULL)
		return(-10);	
	try {
		mSimpleSignal = new SimpleSignal(FrameLength, NumberOfFrame, signal);
	} catch (Signal::NullSignalException) {
		return(-1);
	} catch (Signal::IllegalParameterException) {
		return(-2);
	} catch (Signal::IllegalNameException) {
		return(-3);
	} catch (Signal::NegativeSizeException) {
		return(-4);
	} catch (Signal::FileAccessException) {
		return(-5);
	} catch (OutOfMemoryException) {
		return(-1000);
	} catch (std::bad_alloc) {
		return(-2000);
	}
	if ( mSimpleSignal == NULL )
		return(-6);
	mChannelSet = NULL;
	return(0);
}

int __cdecl binaryWrite(char* outputfilename,const int BeginningFrame, const int EndFrame) {
	if(mSimpleSignal == NULL)
		return(-1);
	try {
		mSimpleSignal->binaryWrite(outputfilename,BeginningFrame,EndFrame);
	} catch (Signal::FileAccessException) {
		return(-2);
	} catch (OutOfMemoryException) {
		return(-1000);
	}
	return(0);
}

int __cdecl textWrite(char * outputfilename,const int BeginningFrame, const int EndFrame) {
	if(mSimpleSignal == NULL)
		return(-1);
	try {
		mSimpleSignal->textWrite(outputfilename,BeginningFrame,EndFrame);
	} catch (Signal::FileAccessException) {
		return(-2);
	} catch (OutOfMemoryException) {
		return(-1000);
	}
	return(0);
}
	
/***
* you are responsible for cleaning the
* generated pointer
*/
short * __cdecl getSignal() {
	if(mSimpleSignal == NULL)
		return(NULL);
	try {
		if(mCopyOfSignalBuffer != NULL) {
			delete[] mCopyOfSignalBuffer;
			mCopyOfSignalBuffer = NULL;
		}
		mCopyOfSignalBuffer = mSimpleSignal->getSignal();
		return(mCopyOfSignalBuffer);
	} catch (OutOfMemoryException) {
		return(NULL);
	}
}
	

void __cdecl cleanup() {
	delete(mSimpleSignal);
	mSimpleSignal = NULL;
	delete(mChannelSet);
	mChannelSet = NULL;
	delete[] mCopyOfSignalBuffer;
	mCopyOfSignalBuffer = NULL;
}

/*
* Arguments:
*	recommended values are offtimewidth = 408, threshold = 100
*/
int __cdecl applyDespike (const float threshold, const int offtimewidth, const int ignoreofftimewidth, const int safetymargin, const int numberofiterations) {
	if(mSimpleSignal == NULL) 
		return(-1);
	try {
		*mSimpleSignal >> Despike(threshold,offtimewidth,ignoreofftimewidth, safetymargin, numberofiterations);
	} catch (Despike::NegativeThresholdException) {
		return(-2);
	} catch (Despike::OffTimeWidthTooLargeException) {
		return(-3);
	} catch (Despike::OffTimeWidthTooSmallException) {
		return(-4);
	} catch (Despike::NegativeIgnoreOffTimeWidthException) {
		return(-5);
	} catch(SimpleSignal::NoSuchFrameException) {
		return(-99);
	} catch (OutOfMemoryException) {
		return(-1000);
	}
	mChannelSet = NULL;
	return(0);
}

int __cdecl applySIMn(const int order, const int lateFrameWidth) {
	if(mSimpleSignal == NULL) 
		return(-1);
	try {
		*mSimpleSignal >> SIMn(order,lateFrameWidth);
	} catch(SIMn::IllegalLateFrameWidthException) {
		return(-2);
	} catch(SIMn::IllegalOrderException) {
		return(-3);
	} catch(SimpleSignal::NoSuchFrameException) {
		return(-99);
	} catch (OutOfMemoryException) {
		return(-1000);
	}
	mChannelSet = NULL;
	return(0);
	
}

int __cdecl applyStacking (const int NumberOfFramesToStack) {
	if(mSimpleSignal == NULL) 
		return(-1);
	try {
		*mSimpleSignal >> Stacking(NumberOfFramesToStack);
	} catch (Stacking::IllegalNumberOfFramesException) {
		return(-2);
	} catch(SimpleSignal::NoSuchFrameException) {
		return(-99);
	} catch (OutOfMemoryException) {
		return(-1000);
	}
	mChannelSet = NULL;
	return(0);
}

/**
* Channel Definition defines
* channels simply with a series of
* integer whose sum should be 
* the FrameLength, each integer is
* the width of the corresponding channel
*/
int __cdecl computeChannels(const int NumberOfChannels, int * ChannelDefinitions, const int WaveletShrinkageCoefs) {
	if(mSimpleSignal == NULL) 
		return(-1);
	try {
		mChannelSet = new ChannelSet(NumberOfChannels,ChannelDefinitions);
		mChannelSet->processSignal(*mSimpleSignal);
		WaveletShrinkage ws(WaveletShrinkageCoefs);
		ws.processChannelSet(*mChannelSet);
		
	} catch (ChannelSet::IllegalChannelDefinitionException) {
		return(-2);
	} catch (ChannelSet::IllegalNumberOfChannelsException) {
		return(-3);
	} catch(SimpleSignal::NoSuchFrameException) {
		return(-99);
	} catch (OutOfMemoryException) {
		return(-1000);
	} catch (std::bad_alloc) {
		return(-2000);
	}
	return(0);
}



float * __cdecl getChannel(const int k) {
	if(mChannelSet == NULL)
		return(NULL);
	try {
		return(mChannelSet->getChannel(k));
	} catch (ChannelSet::NoSuchChannelException) {
		return(NULL);
	}
}

int __cdecl textWriteChannels(char * outputfilename) {
	if(mChannelSet == NULL)
		return(-1);
	if(outputfilename == NULL)
		return(-2);
	mChannelSet->textWrite(outputfilename);
	return(0);
}
