// Downsampling.h: interface for the Downsampling class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_DOWNSAMPLING_)
#define _DOWNSAMPLING_


#include "SignalProcessingObject.h"

class Downsampling : public SignalProcessingObject  
{
public:
	Downsampling();
	virtual ~Downsampling();
	/*
	* Process the signal using the typical 6 frames
	* downsampling...
	*/
	void process(Signal& signal);
	
	/*
	* Will stack the data of all the provided frames
	* into a single frame (the first one). It also
	* assumes that the frames alternate (negative, positive...)
	*/
	void makeOneFrameOutOfThese(short * data, const int framelength, const int numberofframes);
	/*
	* Method used by "makeOneFrameOutOfThese" to convert each
	* frame to PPM before averaging (or taking the median). 
	* The amplitude of the signal is defined to be 8 times the
	* standard deviation.
	*
	* This is unusable if we stick with 16 bits precision!
	*/
	void convertIntoPPM(short *frame,  const int framelength);
};

#endif // !defined(_DOWNSAMPLING_)
