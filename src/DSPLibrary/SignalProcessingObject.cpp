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
#include "SignalProcessingObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SignalProcessingObject::SignalProcessingObject()
{

}

SignalProcessingObject::~SignalProcessingObject()
{

}

/**
* process the given signal
* inputs:
*	signal - the actual signal 
*/
void SignalProcessingObject::process(Signal& signal)
{
	signal.reset();
	while (signal.hasNextFrame()) {
	//	short * frame = signal.nextFrame();
	//	signal.commit();
	}
	std::cout << "no processing " << std::endl;
	// do something here
}



Signal& operator >> ( Signal& signal ,  SignalProcessingObject& processor) {
	processor.process(signal);
	return(signal);
}
