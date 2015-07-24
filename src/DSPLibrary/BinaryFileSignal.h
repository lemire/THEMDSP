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

#if !defined(_BINARYFILESIGNAL_)
#define _BINARYFILESIGNAL_

#include "PrecompHeaders.h"
#include "Signal.h"


/*
* This class allows you to store a signal
* into a permanent binary file (which will
* remains after the object is destroyed). 
* The idea is that you build the signal
* then leave the user with a new "processed"
* file containing the data.
*
* Arguments:
*	readit : should the content of the file be read as the signal?
* TODO: Should implement assignment operator and 
* copy constructor.
*/
class BinaryFileSignal : public Signal    
{
public:
	BinaryFileSignal(const int FrameLength, char * filename, bool readit);
	virtual ~BinaryFileSignal();
	// add dynamically the
	// data
	virtual void addNewFrame(short * FrameSignal);
	// return a copy of the specified frame 
	// your are responsible for the delete call
	virtual short * getFrame (const int);
	// replace the current frame in the buffer
	virtual void setFrame(short *, const int);


	/*
	* Apply an offset to the signal...
	* (this particular offset implementation
	* doesn't modify the content of the underlying
	* file). Note that it will automatically subtract
	* one frame from the total number of frames
	* if the offset is positive. Note also that
	* " % (mFrameLength * sizeof(short))" is applied first to
	* make sure the offset if valid.
	*
	* Arguments
	*	offset : offset en bytes
	*/
	void applyOffset(const int offset) {
		mOffset = offset % (mFrameLength * sizeof(short));
		if (mOffset > 0)
			--mNumberOfFrames;
	}
		


	private :
		void init(bool readit);
		void countNumberOfFrames();

		std::fstream * mFstream;
		char * mFilename;
		int mOffset;



};

#endif // #if !defined(_DISKCACHEDSIGNAL_INCLUDED_)
