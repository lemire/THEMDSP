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
#include "DiskCachedSignal.h"


DiskCachedSignal::DiskCachedSignal (DiskCachedSignal& signal) {
		Signal::init(signal.mFrameLength);
		memcpy(mCurrentFrameData,signal.mCurrentFrameData,mFrameLength * sizeof(short));
		init();
		for(int k = 0; k < signal.mNumberOfFrames ; k++) {
			short * frame = signal.getFrame(k);
			addNewFrame(frame);
		}
		mCurrentFrameIndex = signal.mCurrentFrameIndex;
		mChangesToBeCommited = signal.mChangesToBeCommited;
}

// empty signal
DiskCachedSignal::DiskCachedSignal(const int FrameLength)
{
	Signal::init(FrameLength);
	init();
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
DiskCachedSignal::DiskCachedSignal(const int FrameLength,char * inputfilename, const int BeginningFrame , const int EndFrame , const int offset )
{
	Signal::init(FrameLength);
	init();
	readFromBinaryFile(inputfilename, BeginningFrame, EndFrame, offset );
}




void DiskCachedSignal::init() {
	// if the next line won't compile
	// try tmpname instead of _tempnam
	if( ( mTempFilename = _tempnam("c:\\windows\\TEMP\\them", "them")) == NULL )
      throw FileAccessException();	
	// we make sure the file is created
	try {
		mFstemp = new std::fstream();// mTempFilename,ios::binary | ios::in | ios::out   ) ;
	} catch (std::bad_alloc) {
		mFstemp = NULL;
	}
	if(mFstemp == NULL)
		throw OutOfMemoryException();
	mFstemp->open( mTempFilename,std::ios::binary | std::ios::in | std::ios::out   | std::ios::trunc  ) ;
	if(mFstemp->fail()) {
		std::cout << " could not create file ?" << std::endl;
		std::cout << " Check file " << mTempFilename << std::endl;
		mFstemp->close();
		throw FileAccessException();
	}
	// init
	mNumberOfFrames = 0;
	reset();
}

DiskCachedSignal::~DiskCachedSignal()
{
	(* mFstemp).close();
	delete(mFstemp);
	remove(mTempFilename);
}


void DiskCachedSignal::addNewFrame(short * FrameSignal) {
	if(FrameSignal == 0)
		throw NullSignalException();
	
	
	mFstemp->seekp( mNumberOfFrames * mFrameLength * sizeof(short) );
	if(mFstemp->fail()) {
		std::cout << " Could not position in the file ? " << std::endl;
		mFstemp->close();
		throw FileAccessException();
	}
	mFstemp->write((char *)FrameSignal,sizeof(short) * mFrameLength);
	if(mFstemp->fail()) {
		std::cout << " could not add data ?" << std::endl;
		throw FileAccessException();
	}
	mNumberOfFrames++;
}
		
// return the specified frame 
// commit will record your changes
short * DiskCachedSignal::getFrame(const int k ) {
	if( (k < 0) || ( k >= mNumberOfFrames))
		throw NoSuchFrameException();
	mChangesToBeCommited = true;
	mFstemp->seekg(k * mFrameLength * sizeof (short));
	if(mFstemp->fail()) {
		mFstemp->close();
		throw FileAccessException();
	}
	for(int l = 0 ; l < mFrameLength ; l++) {
		mFstemp->read((char *) & mCurrentFrameData[l], sizeof(short));
	}
	assert(! mFstemp->fail());
	return(mCurrentFrameData);	
}

void DiskCachedSignal::setFrame(short * FrameSignal , const int k) {
	if(FrameSignal == 0)
		throw NullSignalException();
	if( (k < 0) || ( k > mNumberOfFrames))
		throw NoSuchFrameException();
	if(k == mNumberOfFrames) {
		addNewFrame(FrameSignal);
		return;
	}
	mFstemp->seekp(k * mFrameLength * sizeof(short));
	if( mFstemp->fail()) {
		std::cout << " could not position into file " << std::endl;
		mFstemp->close();
		throw FileAccessException();
	}
	for(int l = 0 ; l < mFrameLength ; l++) {
		mFstemp->write((char *) & FrameSignal[l], sizeof(short));

	}
	assert(! mFstemp->fail());
	//fs.close();
}

   
DiskCachedSignal & DiskCachedSignal::operator = ( DiskCachedSignal& signal )
{	
	
	if(this != &signal)
	{
		delete[] mTempFilename;
		mTempFilename = signal.mTempFilename;
		mNumberOfFrames = signal.mNumberOfFrames;
		mFrameLength = signal.mFrameLength;
		mCurrentFrameIndex = signal.mCurrentFrameIndex;
		delete[] mCurrentFrameData;
		mCurrentFrameData=signal.mCurrentFrameData;
		mChangesToBeCommited = signal.mChangesToBeCommited;
	}
	return * this;
}





