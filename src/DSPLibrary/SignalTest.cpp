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
#include "SignalTest.h"
#include "SpecialMath.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//
SignalTest::SignalTest()
{

}

//
SignalTest::~SignalTest()
{

}


/**
* Generate a linear signal with the given slope (a line).
* The signal starts at zero.
*
* inputs:
*	freq - frequency of the sine wave
*	amp - amplitude of the sine wave
*	NumberOfFrames - number of frames to generate
*/
void SignalTest::generateSineSignal(double freq, double amp, int NumberOfFrames, Signal& s) {

	for(int k = 0 ; k < NumberOfFrames ; k++) {
		short * currentframe = new(std::nothrow) short[s.getFrameLength()];
		if( currentframe == NULL)
			throw OutOfMemoryException();
		for (int l = 0; l < s.getFrameLength() ; l++) {
			currentframe[l] = SpecialMath::roundToShort(amp * sin((k * s.getFrameLength() + l) * freq));
		}
		s.addNewFrame(currentframe);
		delete[] currentframe;
		currentframe = NULL;
	}	
	
}

/**
* Generate a linear signal with the given slope (a line).
* The signal starts at zero.
*
* inputs:
*	splope - slope of the line
*	NumberOfFrames - number of frames to generate
*/
void SignalTest::generateLinearSignal(double slope, int NumberOfFrames, Signal& s) {

	for(int k = 0 ; k < NumberOfFrames ; k++) {
		short * currentframe = new(std::nothrow) short[s.getFrameLength()];
		if(currentframe == NULL)
			throw OutOfMemoryException();
		for (int l = 0; l < s.getFrameLength() ; l++) {
			currentframe[l] = SpecialMath::roundToShort(slope * ((k * s.getFrameLength() + l)% 10000) );// we set a modulo for safety
		}
		s.addNewFrame(currentframe);
		delete[] currentframe;
	}	
	
}

/**
* add artificial spikes to a signal
* inputs:
*	amplitude - max amplitude of the generated spikes
*	s - the signal to be polluted
*/
void SignalTest::polluteSignalWithSpikes(short amplitude, Signal& s) {
    srand( (unsigned)time( NULL ) );
	s.reset();
	while(s.hasNextFrame()) {
		short * currentframe = s.nextFrame();
		
		int pos = SpecialMath::round((double) rand() / RAND_MAX * (s.getFrameLength() - 1));// position of the spike
		
		short amp = SpecialMath::roundToShort((double) rand() / RAND_MAX * 2 * amplitude  - amplitude);
		currentframe[pos] = (short)(currentframe[pos] + amp);
		currentframe[pos  + 1] = (short)(currentframe[pos  + 1] - amp);
		s.commit();
	}
}