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

#if !defined(_DISKCACHEDSIGNAL_INCLUDED_)
#define _DISKCACHEDSIGNAL_INCLUDED_

#include "Signal.h"

class DiskCachedSignal : public Signal  
{

	public :
		// copy constructor
		DiskCachedSignal (DiskCachedSignal& signal);
		// create and empty signal
		DiskCachedSignal(const int FrameLength);
		// create a signal from a binary file
		DiskCachedSignal(const int FrameLength,char * inputfilename ,const int BeginningFrame = 0, const int EndFrame = 99999, const int offset = 0);
		//destructor
		virtual ~DiskCachedSignal();
		// add dynamically the
		// data
		void addNewFrame(short * FrameSignal);
		// return a copy of the specified frame 
		// your are responsible for the delete call
		short * getFrame (const int);
		// replace the current frame in the buffer
		void setFrame(short *, const int);

		// better be safe than sorry, C++ is unsafe IMHO :-)
		DiskCachedSignal& operator = ( DiskCachedSignal& signal );


		
	private :

		void init();

		std::fstream * mFstemp;
		char * mTempFilename;


};

#endif // #if !defined(_DISKCACHEDSIGNAL_INCLUDED_)
