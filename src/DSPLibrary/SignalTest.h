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

#if !defined(_SIGNALTEST_)
#define _SIGNALTEST_


#include "Signal.h"

//////////////////////
// use to generate fake signals
//
// For example, we can write
//	Signal s(512);
//	SignalTest::generateSineSignal(3.0/(512 * 2),100,128,s);
//
//	where 3.0/(512*2) defines the frequency of the wave.
//  100 is the amplitude and 128 is the number of frames
//
// SignalTest.h: interface for the SignalTest class.
//////////////
class SignalTest  
{
	public:
		
		/**
		* Generate a linear signal with the given slope (a line).
		* The signal starts at zero.
		*
		* inputs:
		*	freq - frequency of the sine wave
		*	amp - amplitude of the sine wave
		*	NumberOfFrames - number of frames to generate
		*/
		static void generateSineSignal(double freq, double amp, int NumberOfFrames, Signal& s);

		/**
		* Generate a linear signal with the given slope (a line).
		* The signal starts at zero.
		*
		* inputs:
		*	splope - slope of the line
		*	NumberOfFrames - number of frames to generate
		*/
		static void generateLinearSignal(double slope, int NumberOfFrames, Signal& s);

		/**
		* add artificial spikes to a signal
		* inputs:
		*	amplitude - max amplitude of the generated spikes
		*	s - the signal to be polluted
		*/
		static void polluteSignalWithSpikes(short amplitude, Signal& s);
	private:
		SignalTest();
		virtual ~SignalTest();

};

#endif // !defined(AFX_SIGNALTEST_H__31750960_99F4_11D3_A589_00105ADCDFA4__INCLUDED_)
