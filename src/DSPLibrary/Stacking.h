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

#if !defined(_STACKING_)
#define _STACKING_


#include "SignalProcessingObject.h"


// provides simple stacking in a running average kind of way (frame-wise)
// see the Downsampling class for a similar but maybe better implementation
class Stacking : public SignalProcessingObject  
{
	public:
		/*
		* Constructor
		* Inputs:
		*	NumberOfFramesToStack - number of stack over which to stack the signal
		*	invertsigns - whether you should invert signs every other frame (default = true)
		*/
		Stacking(const int NumberOfFramesToStack, const bool invertsigns = true);
		virtual ~Stacking();
		void process(Signal& signal);

		// this exception is thrown whenever an illegal number of frames is inputed
		class IllegalNumberOfFramesException{};
	private:
		int mNumberOfFramesToStack;
		bool mInvertSigns;

};

#endif // !defined(AFX_STACKING_H__943EF781_97AC_11D3_A589_00105ADCDFA4__INCLUDED_)
