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



#if !defined(_SIGNALPROCESSINGOBJECT_)
#define _SIGNALPROCESSINGOBJECT_


#include "Signal.h"


//
// a generic signal processing object (base class)
//
// The SignalProcessingObject class is the counterpart
// to the Signal class. You can easily extend by inheritance
// the SignalProcessingObject. It will also allow
// a "stream-like" processing of the signal which is
// potentially much more extensible (for real-time
// applications for example).
//
// Moreover, it allows to acces the signal through a 
// modern data model and it will lower memory usage.
//
// For example, we can write
//
//		signal.reset();
//		while (signal.hasNextFrame()) {
//			short * frame = signal.nextFrame();
//			process(frame);		
//			signal.commit(); // commit the changes you've made
//		}
//
//	However, there are two common errors : forgetting
//	to commit and forgetting to reset the signal. These
//	two lines (reset and commit) are vital.
//
// Once this is done, we overload the operator so
// that processing the signal will look like
// 
//	RawSignal >> SignalProcessor(parm1);
//
// and optionnally, you can write the signal to 
// a file simply by...
//
//	RawSignal >> SignalProcessor(parm1) >> "data.bin";
//
//
// This base class just does nothing to the signal.
//
class SignalProcessingObject  {
	public:
		SignalProcessingObject();
		virtual ~SignalProcessingObject();
		virtual void process(Signal& signal);
		friend Signal& operator >> ( Signal& signal , SignalProcessingObject& processor);		

	private:



};

#endif // !defined(AFX_SIGNALPROCESSINGOBJECT_H__2F811A47_9562_11D3_A589_00105ADCDFA4__INCLUDED_)
