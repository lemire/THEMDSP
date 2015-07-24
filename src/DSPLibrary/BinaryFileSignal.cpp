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



#include "BinaryFileSignal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// empty signal
BinaryFileSignal::BinaryFileSignal(const int FrameLength, char * filename, bool readit)
{
	Signal::init(FrameLength);
	mFilename = filename;
	init(readit);
	if(readit) {
		countNumberOfFrames();
	}
}

void BinaryFileSignal::countNumberOfFrames() {
		// only thing to do is to actually
		// read the whole thing to see how much data we have
		short * frame = NULL;
		std::fstream fsin(mFilename,std::ios::binary | std::ios::in);
		
		//std::fstream fsin(mFilename, std::ios::in | std::ios::binary);
		try {
			frame = new short[mFrameLength];
			fsin.read((char *) frame, mFrameLength * sizeof(short));
			while (fsin.gcount() == mFrameLength * sizeof(short)) {
				++mNumberOfFrames;
				fsin.read((char *) frame, mFrameLength * sizeof(short));
			}
			std::cout << "Number of frames detected = " << mNumberOfFrames << std::endl;
		} catch (...) {
			// if something bad happens... welll
		}
		delete[] frame;
		fsin.close();
		
		

}

BinaryFileSignal::~BinaryFileSignal()
{
	mFstream->close();
	delete mFstream;
	mFstream = NULL;
}

//////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////


void BinaryFileSignal::init(bool readit) {	
	if(mFilename == NULL)
		throw FileAccessException();
	// we make sure the file is created
	try {
		if (!readit) 
			mFstream = new std::fstream( mFilename,std::ios::binary |  std::ios::out |  std::ios::in | std::ios::trunc );// mTempFilename,ios::binary | ios::in | ios::out   ) ;
		else 
			mFstream = new std::fstream( mFilename,std::ios::binary |  std::ios::out |  std::ios::in);
	} catch (std::bad_alloc) {
		mFstream = NULL;
	}
	if(mFstream == NULL)
		throw OutOfMemoryException();
	if(mFstream->fail()) {
		std::cout << " could not create file ?" << std::endl;
		std::cout << " Check file " << mFilename << std::endl;
		mFstream->close();
		throw FileAccessException();
	}
	// init
	mNumberOfFrames = 0;
	mOffset = 0;
	reset();
}


void BinaryFileSignal::addNewFrame(short * FrameSignal) {
	if(FrameSignal == 0)
		throw NullSignalException();	
	mFstream->seekp( mNumberOfFrames * mFrameLength * sizeof(short) + mOffset);
	if(mFstream->fail()) {
		std::cout << " Could not position in the file ? " << std::endl;
		mFstream->close();
		throw FileAccessException();
	}
	mFstream->write((char *)FrameSignal,sizeof(short) * mFrameLength);
	if(mFstream->fail()) {
		std::cout << " could not add data ?" << std::endl;
		throw FileAccessException();
	}
	mNumberOfFrames++;
}
		
// return the specified frame 
// commit will record your changes
short * BinaryFileSignal::getFrame(const int k ) {
	if( (k < 0) || ( k >= mNumberOfFrames))
		throw NoSuchFrameException();
	mChangesToBeCommited = true;
	mFstream->seekg(k * mFrameLength * sizeof (short) + mOffset);
	if(mFstream->fail()) {
		mFstream->close();
		throw FileAccessException();
	}
	mFstream->read((char *)mCurrentFrameData,sizeof(short) * mFrameLength);
	assert(!mFstream->fail());
	return(mCurrentFrameData);	
}

void BinaryFileSignal::setFrame(short * FrameSignal , const int k) {
	if(FrameSignal == 0)
		throw NullSignalException();
	if( (k < 0) || ( k > mNumberOfFrames))
		throw NoSuchFrameException();
	if(k == mNumberOfFrames) {
		addNewFrame(FrameSignal);
		return;
	}
	mFstream->seekp(k* mFrameLength * sizeof(short) + mOffset);
	if(mFstream->fail()) {
		std::cout << " could not position into file " << std::endl;
		mFstream->close();
		throw FileAccessException();
	}
	for(int l = 0 ; l < mFrameLength ; l++) {
		mFstream->write((char *) & FrameSignal[l], sizeof(short));

	}
	assert(!mFstream->fail());
	//fs.close();
}

   





