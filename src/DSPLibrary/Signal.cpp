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
#include "Signal.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// empty signal
Signal::Signal(const int FrameLength)
{
	init(FrameLength);
}

void Signal::init(const int FrameLength) {
	if(FrameLength <= 0)
		throw NegativeSizeException();
	mFrameLength = FrameLength;
	mNumberOfFrames = 0;
	mCurrentFrameData = new(std::nothrow) short[mFrameLength];
	if(mCurrentFrameData == NULL)
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
void Signal::readFromBinaryFile(char * inputfilename, const int BeginningFrame , const int EndFrame , const int offset ) {
	if(inputfilename == 0)
		throw IllegalNameException();
	if((BeginningFrame<0)||(EndFrame<0)||(EndFrame<BeginningFrame))
		throw IllegalParameterException();
	if((offset < 0))
		throw NegativeSizeException();
	// read the file
	std::fstream fs( inputfilename, std::ios::in | std::ios::binary );// 
	if(!fs.is_open()) {
		std::cout << "could not open file "<<inputfilename << std::endl;
		throw Signal::FileAccessException();
	}
	// skip
	if(fs.fail())
		throw Signal::FileAccessException();
	fs.seekg(BeginningFrame * sizeof(short) * mFrameLength + offset);
	if(fs.fail()) {
		throw Signal::FileAccessException();
	}
	int pos = 0;
	short * data = new(std::nothrow) short[mFrameLength];
	if (data == NULL) {
		fs.close();
		throw OutOfMemoryException();
	}
	while((pos < EndFrame - BeginningFrame + 1) && !(fs.eof())) {
		fs.read((char *)data,sizeof(short) * mFrameLength);
		if (fs.gcount() == sizeof(short) * mFrameLength)
			addNewFrame(data);
		else
			break;	
		pos++;
	}

	delete[] data;
	data = NULL;
	fs.close();	
	if(pos == 0) {
		throw NullSignalException();
	}
}


void Signal::binaryWrite(char * inputfilename, const int BeginningFrame, const int EndFrame ) {
	std::fstream newfs (inputfilename,std::ios::binary | std::ios::out ) ;

	if(newfs.fail()) {
		std::cout << " could not open output file for binary write ?" << std::endl;
		std::cout << " Check file " << inputfilename << std::endl;
		newfs.close();
		throw FileAccessException();
	}
	reset(BeginningFrame);
	
	while(hasNextFrame() && (mCurrentFrameIndex <= EndFrame)) {
		short * current = this->nextFrame();
		newfs.write((char *)current,mFrameLength * sizeof(short));		
	}
	newfs.close();

}


Signal::Signal() {
	mCurrentFrameData = NULL;
}

Signal::~Signal() {
	delete[] mCurrentFrameData;
	mCurrentFrameData = NULL;
}

Signal::Signal(Signal& signal) {
	mNumberOfFrames = signal.mNumberOfFrames;
	mFrameLength = signal.mFrameLength;
}


void Signal::textWrite(char * inputfilename,const int BeginningFrame, const int EndFrame ) {
	std::fstream newfs (inputfilename,std::ios::out);
	if(newfs.fail()) {
		std::cout << " could not open output file for text write ?" << std::endl;
		std::cout << " Check file " << inputfilename << std::endl;
		newfs.close();
		throw FileAccessException();
	}
	reset(BeginningFrame);
	int pos = BeginningFrame;
	while(hasNextFrame() && (pos <= EndFrame)) {		
		short * currentFrame = nextFrame();
		pos++;	
		for (int k = 0 ; k < getFrameLength(); k++) {
			newfs << currentFrame[k] << "\n";
		}
	}
	newfs.close();
}

Signal& operator >> ( Signal& signal ,  char * filename) {
	signal.binaryWrite(filename,0,signal.getNumberOfFrames());
	return(signal);
}

int Signal::getNumberOfFrames() {
	return(mNumberOfFrames);
}

int Signal::getFrameLength() {
	return(mFrameLength);
}


bool Signal::hasNextFrame() {		
	return(mCurrentFrameIndex < mNumberOfFrames);
}

void Signal::commit() {
	if(mChangesToBeCommited) {
		assert (mCurrentFrameIndex - 1 >= 0);
		setFrame(mCurrentFrameData,mCurrentFrameIndex  - 1);
	}
	mChangesToBeCommited = false;
}


short * Signal::nextFrame() {
	mChangesToBeCommited = true;
	//short * frame = getFrame(mCurrentFrameIndex++);
	mCurrentFrameData = getFrame(mCurrentFrameIndex++);
	return(mCurrentFrameData);
}


/**
* reset the signal at a new position
*
* Inputs :
*	newframe - the new position (0 by default)
*/
void Signal::reset(const int newframe) {
	if((newframe < 0) || (newframe >mNumberOfFrames))
		throw NoSuchFrameException();
	mChangesToBeCommited = false;
	mCurrentFrameIndex = newframe;
}
