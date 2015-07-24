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

#if !defined(_SIMPLESIGNAL_INCLUDED_)
#define _SIMPLESIGNAL_INCLUDED_

#include "PrecompHeaders.h"
#include "Signal.h"


class SimpleSignal : public Signal  
{

	public :
		// copy constructor
		SimpleSignal (SimpleSignal& signal);
		// create and empty signal
		SimpleSignal(const int FrameLength);
		// create a signal from a binary file
		SimpleSignal(const int FrameLength,char * inputfilename ,const int BeginningFrame = 0, const int EndFrame = 99999, const int offset = 0);
		// create a signal from a pointer
		SimpleSignal(const int FrameLength,const int NumberOfFrame, short * signal);
		//destructor
		virtual ~SimpleSignal();
		// add dynamically the
		// data
		void addNewFrame(short * FrameSignal);
		// return a copy of the specified frame 
		// your are responsible for the delete call
		short * getFrame (const int);
		// replace the current frame in the buffer
		void setFrame(short *, const int);

		// better be safe than sorry, C++ is unsafe IMHO :-)
		SimpleSignal& operator = ( SimpleSignal& signal );
		

		/*
		* return a copy of the signal as a pointer
		*
		*  Results
		*     a copy of the internal buffer
		*/
		short * getSignal() {
			short * copy = new(std::nothrow) short[mSignalBufferSize];
			if(copy == NULL)
				throw OutOfMemoryException();
			memcpy(copy,mSignalBuffer,mNumberOfFrames * mFrameLength * sizeof(short));
			return(copy);
		}
	private :
		short * mSignalBuffer;
		int mSignalBufferSize;
		enum {
			mDefaultNumberOfFrames = 32//arbitrary
		};
};

#endif // #if !defined(_SIMPLESIGNAL_INCLUDED_)

