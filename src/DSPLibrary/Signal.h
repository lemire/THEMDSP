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


#if !defined(_SIGNAL_H_)
#define _SIGNAL_H_



// a signal container used to replace the "short *" pointer
//
// The signal class encapsulates the signal so that
// we don't have to worry so much about it. It will
// write the signal into a temporary file (if we want to)
// and handle a buffer so that we don't have to process the
// signal in separate segments (if we want to). It will
// also allow a "stream-like" processing of the signal which is
// potentially much more extensible (for real-time
// applications for example).
//
// Moreover, it allows to acces the signal through a 
// modern data model and it will lower memory usage.
//
// The current implementation is a reference implementation
// and is not meant to be fast, it is only meant to
// be functionnal.
//
// For example, we can write
//
//		while (signal.hasNextFrame()) {
//			short * frame = signal.nextFrame();
//			// ... modifying frame will modify the signal
//			commit(); // but you need to call commit to commit your changes
//		}
//
//	This allows for real time data acquisition since
//  we don't have to know how many frames will be 
//  acquired beforehand. the "haxNextFrame" methods
//	simply blocks until a new frame is read.
//
// For real-time processing for example, we could
// implement listeners so that each time a new
// frame is read, the SignalProcessingObjects are
// notified and will process.
//
// Uncoupling the data from the processing is key to
// this design. 
//
// We can easily add support for different file format
// all you have to do is generate a Signal object. Then
// you never have to worry about the rest of the code
// because the relevant information (length, length of frames,
// etc.) is packaged and will take care of itself.
//
//
// The Signal class can also reads itself from a 
// binary file and write itself into text files.
//
// You can write a signal to a binary file simply by
// writing
//
//	     OurSignal >> "mybinaryfile.em1";
//
//	Observe that you must call _rmtmp(); at the end of a program
// using this object to clean up the temporary files.
//


class Signal  
{
	public :


		// read the signal from a binary file
		virtual void readFromBinaryFile(char * inputfilename,int BeginningFrame , int EndFrame , int offset );
		// is there another frame?
		virtual bool hasNextFrame();
		// return the frame	
		virtual short * nextFrame();
		// get the total number of frames
		virtual int getNumberOfFrames();
		// get the length of a single frame
		virtual int getFrameLength();
		// commit changes done to the pointer
		// returned by getFrame and nextFrame
		virtual void commit ();

		int getCurrentFrameIndex() const {
			return mCurrentFrameIndex;
		}
		/**
		* reset the signal at a new position
		*
		* Inputs :
		*	newframe - the new position (0 by default)
		*/
		virtual void reset (const int newframe = 0);
		// write the signal to a text file
		virtual void textWrite(char * inputfilename,const int BeginningFrame,const int EndFrame );
		// write the signal to a binary file
		virtual void binaryWrite(char * inputfilename,const int BeginningFrame, const int EndFrame );
		//
		friend Signal& operator >> ( Signal& signal ,  char *);		
		
		// a length should always be positive
		class NegativeSizeException{};
		// if the provided data points to null
		class NullSignalException {};
		//if you request a frame that doesn't exist
		class NoSuchFrameException{};
		//if the given name is not a file
		class IllegalNameException{};
		// if the parameters are incorrect
		class IllegalParameterException{};
		// generic error for file access
		class FileAccessException{};

		//destructor
		virtual ~Signal();
		// constructor
		Signal();
		Signal (int);
		Signal(Signal& signal);

		/////////////////////
		//abstract
		/////////////////////
		// add dynamically the
		// data
		virtual void addNewFrame(short * FrameSignal) = 0;
		// return a copy of the specified frame 
		// your are responsible for the delete call
		virtual short * getFrame (const int) = 0;
		// replace the current frame in the buffer
		virtual void setFrame(short *, const int) = 0;
		// discard the end of the signal (should be overwritten)
		virtual void setNumberOfFrames(const int NumberOfFrames) {
			mNumberOfFrames = NumberOfFrames;
		}

	protected :
		int mCurrentFrameIndex;
		int mNumberOfFrames;
		int mFrameLength;
		short * mCurrentFrameData;
		bool mChangesToBeCommited;

		virtual void init(int);
   
};

#endif // !defined(AFX_SIGNAL_H__2F811A43_9562_11D3_A589_00105ADCDFA4__INCLUDED_)

