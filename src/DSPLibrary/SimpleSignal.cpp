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
#include "SimpleSignal.h"


SimpleSignal::SimpleSignal (SimpleSignal& signal) {
		Signal::init(signal.mFrameLength);
		mCurrentFrameData = new(std::nothrow) short[mFrameLength];
		mSignalBuffer = new(std::nothrow) short[signal.mSignalBufferSize];
		if((mCurrentFrameData == NULL) || (mSignalBuffer == NULL)) {
			delete[] mCurrentFrameData;
			delete[] mSignalBuffer;
			throw OutOfMemoryException();
		}
		memcpy(mCurrentFrameData,signal.mCurrentFrameData,mFrameLength * sizeof(short));		
		mSignalBufferSize = signal.mSignalBufferSize;
		memcpy(mSignalBuffer,signal.mSignalBuffer,mSignalBufferSize * sizeof(short));
		mCurrentFrameIndex = signal.mCurrentFrameIndex;
		mChangesToBeCommited = signal.mChangesToBeCommited;
}


SimpleSignal::SimpleSignal(const int FrameLength,const int NumberOfFrame, short * signal) {
	Signal::init(FrameLength);
	mNumberOfFrames = NumberOfFrame;
	mSignalBufferSize = mNumberOfFrames * mFrameLength;
	mSignalBuffer = new(std::nothrow) short[mSignalBufferSize];
	if(mSignalBuffer == NULL)
		throw OutOfMemoryException();
	memcpy(mSignalBuffer,signal,mSignalBufferSize * sizeof(short));
}

// empty signal
SimpleSignal::SimpleSignal(const int FrameLength)
{
	Signal::init(FrameLength);
	mSignalBufferSize = mDefaultNumberOfFrames * mFrameLength;
	mSignalBuffer = new(std::nothrow) short[mSignalBufferSize];
	if(mSignalBuffer == NULL)
		throw OutOfMemoryException();
}

/*
* construct a signal from a binary file
* assumes a simple 16 bits encoding
* All files are assumed to use Little Endian encoding.
*
*	inputs:
*		FrameLength - size of frames, usually 512 
*		inputfilename - the name of the input file
*		BeginningFrame	- first frame to read, previous frames are ignored
*		EndFrame - last frame to read, following frames are ignored
*		offset - offset from which reading begins (useful to skip a header or handle other problems), notice that the offset is in number of bytes
*/
SimpleSignal::SimpleSignal(const int FrameLength,char * inputfilename, const int BeginningFrame , const int EndFrame , const int offset )
{
	Signal::init(FrameLength);
	mSignalBufferSize = mDefaultNumberOfFrames * mFrameLength;
	mSignalBuffer = new(std::nothrow) short [mSignalBufferSize];
	if(mSignalBuffer == NULL)
		throw OutOfMemoryException();
	readFromBinaryFile(inputfilename, BeginningFrame, EndFrame, offset );
}




SimpleSignal::~SimpleSignal() {
	delete[] mSignalBuffer;
	mSignalBuffer = NULL;
}

void SimpleSignal::addNewFrame(short * FrameSignal) {
	if(FrameSignal == 0)
		throw NullSignalException();	
	mNumberOfFrames++;
	if(mSignalBufferSize < mNumberOfFrames * mFrameLength) {
		mSignalBufferSize += mDefaultNumberOfFrames * mFrameLength;
		short * copy = new(std::nothrow) short[mSignalBufferSize];
		if(copy == NULL)
			throw OutOfMemoryException();
		memcpy(copy,mSignalBuffer,(mSignalBufferSize - mDefaultNumberOfFrames * mFrameLength)* sizeof(short));	
		delete[] mSignalBuffer;
		mSignalBuffer = copy;
	}

	//mSignalBuffer = new short[mSignalBufferSize];
	memcpy(mSignalBuffer + mFrameLength * (mNumberOfFrames - 1),FrameSignal,mFrameLength * sizeof(short));
	
}
		
// return the specified frame 
// commit will record your changes
short * SimpleSignal::getFrame(const int k ) {
	if( (k < 0) || ( k >= mNumberOfFrames))
		throw NoSuchFrameException();
	mChangesToBeCommited = true;
	memcpy(mCurrentFrameData,mSignalBuffer + k * mFrameLength,mFrameLength * sizeof(short));
	return(mCurrentFrameData);		
}

void SimpleSignal::setFrame(short * FrameSignal , const int k) {
	if(FrameSignal == 0)
		throw NullSignalException();
	if( (k < 0) || ( k > mNumberOfFrames))
		throw NoSuchFrameException();
	if(k == mNumberOfFrames) {
		addNewFrame(FrameSignal);
		return;
	}
	memcpy(mSignalBuffer + k * mFrameLength, FrameSignal,mFrameLength * sizeof(short));	
}

   
SimpleSignal & SimpleSignal::operator = ( SimpleSignal& signal )
{	
	
	if(this != &signal)
	{
		mNumberOfFrames = signal.mNumberOfFrames;
		mFrameLength = signal.mFrameLength;
		mCurrentFrameIndex = signal.mCurrentFrameIndex;
		delete[] mCurrentFrameData;
		mCurrentFrameData=signal.mCurrentFrameData;
		delete[] mSignalBuffer;
		mSignalBuffer = signal.mSignalBuffer;
		mChangesToBeCommited = signal.mChangesToBeCommited;
		mSignalBufferSize = signal.mSignalBufferSize;
	}
	return * this;
}

