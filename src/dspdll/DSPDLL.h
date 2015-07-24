/*
*	WARNING: Starting in 2001, the DLL as it is now is no longer
*	supported by Daniel Lemire. A more appropriate framework is
*	needed and should be designed with the requirements of the rest
*	of the software team in mind.
*
*		Thanks!
*/







//
// Convention and usage
//		Negative or NULL return values
//		mean something bad happened.
//		0 means something good happened.
//	
//		Please always test whether returned pointers are null
//		and whether returned integers are negative.
//
//		Make sure that the cleanup function is call once for
//		each call to DSPDLL.
//
//		Also, it is important to understand that getSignal
//		actually gives you a copy of the internal buffer.
//		Severe memory leaks used to occur because the cleanup
//      function didn't	even attempt to cleanup the pointers you
//		get from getSignal (nor should it). To avoid this, the
//		library	has been made more fool-proof, that is, now
//		the pointer	will be cleaned along with the cleanup call. 
//		*However* you only have one such buffer and it will be
//		set to NULL	after you call the cleanup function, so make copies!
//
//		Obviously, then, you are responsible for cleaning up your
//		copies...
//

#if !defined(_DSPDLL_INCLUDED_)
#define _DSPDLL_INCLUDED_
extern "C" {
	//
	// No matter what, you must call the DSPDLL
	// functions first. (One only.) After each call
	// you must, eventually, call cleanup.
	//
	// create a signal from a binary file
	// The input must be either the EM1 or EM2 signal
	_declspec(dllexport) int __cdecl DSPDLL(const int FrameLength,char * inputfilename ,const int BeginningFrame = 0, const int EndFrame = 99999, const int offset = 0);
	// create a signal from a pointer

	_declspec(dllexport) int __cdecl DSPDLLPointer(const int FrameLength,const int NumberOfFrame, short * signal);

	//
	//
	// no matter what
	// you program must call the cleanup function
	//
	_declspec(dllexport) void __cdecl cleanup();
	//
	//
	//
	///////////////////
	// Output
	////////////////////
	//
	//
	//
	// write the signal to a binary file (recommended)
	_declspec(dllexport) int __cdecl binaryWrite(char* outputfilename,const int BeginningFrame, const int EndFrame);	
	// write the signal to a text file
	_declspec(dllexport) int __cdecl textWrite(char * outputfilename,const int BeginningFrame, const int EndFrame);
	// recover the signal as a pointer
	// WARNING : you used to have to clean this yourself,
	// now this will be cleaned along with everything else
	// by calling cleanup *HOWEVER* you have only one such
	// buffer available, so make copies!!!
	_declspec(dllexport) short * __cdecl getSignal() ;
	//
	//
	//
	/////////////////////
	// Processing
	/////////////////////
	//
	//
	//
	// anti-spike algorithm
	// ignoreofftimewidth is the part of the offtime that we may safely ignore
	// (last bit) and was introduced following Chris' comments.
	// the threshold is a denoising parameter, the lower it gets, the more aggressive the denoising gets
	// safetymargin is the "boundary" margin where no denoising is done, lowering this value can lead to various artifacts
	// numberofiterations specify the cutoff frequency as in (1 / 2 ^ numberofiterations) * sampling frequency
	// notice that offtimewidth is divisible by 2 ^numberofiterations (rounded off to the lowest value
	// the window of processing goes from 512 - ignoreofftimewidth - offtimewidth to 512 - offtimewidth, not considering the safetymargin margin parameter
	_declspec(dllexport) int __cdecl applyDespike (const float threshold = 100, const int offtimewidth = 408, const int ignoreofftimewidth = 0, const int safetymargin = 8, const int numberofiterations = 2) ;
	// baseline correction
	_declspec(dllexport) int __cdecl applySIMn(const int order = 3, const int lateFrameWidth = 48) ;
	// stack the signal using a running average algorithm
	// warning : may lead to sign inversion in some cases
	_declspec(dllexport) int __cdecl applyStacking (const int NumberOfFramesToStack = 6) ;
	
	
	// 
	// Channel processing bit
	//
	// call this function first
	_declspec(dllexport) int __cdecl computeChannels(const int NumberOfChannels, int * ChannelDefinitions, const float WaveletShrinkageCoefs = 1024.0f) ;
	// get a specific channel
	_declspec(dllexport) float * __cdecl getChannel(const int k) ;
	// write channels to a text file
	_declspec(dllexport) int __cdecl textWriteChannels(char * outputfilename) ;

}
#endif // #if !defined(_DSPDLL_INCLUDED_)
