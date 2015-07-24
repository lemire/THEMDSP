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


#if !defined(_DESPIKE_)
#define _DESPIKE_



  static const float spikescale[4] = {0.482962913144534f,
								0.836516303737808f,
								0.224143868042013f,
								-0.12940952255126f};
  static const float spikewavelet[4] = {-0.12940952255126f,
								-0.224143868042013f,
								0.836516303737808f,
								-0.482962913144534f};
						

#include "SignalProcessingObject.h"

//a class used to remove spikes in a signal
class Despike : public SignalProcessingObject  
{
public:
	/*
	* Arguments:
	*	recommended values are threshold = 100, offtimewidth = 408
	*/
	Despike(const float threshold = 100.0f, const int offtimewidth = 408, const int ignoreofftimewidth = 0, const int safetymargin = 8, const int numberofiterations = 2);
	virtual ~Despike();
	void process(Signal& signal);

	void filterSpikes (short * signal, const int FrameLength);

	// the threshold should not be negative
	class NegativeThresholdException{};
	// the OffTimeWidth should be large enough to accomodate the FWT 
	class OffTimeWidthTooSmallException{};
	// if the the OffTimeWidth is larger then the frame width
	class OffTimeWidthTooLargeException{};
	// "ignore off time width" must be positive or zero
	class NegativeIgnoreOffTimeWidthException{};
private:
	

	
	float mThreshold;
	int mFrameLength;
	int mOffTimeWidth;
	int mIgnoreOffTimeWidth;
	int mNumberOfIterations;
	int mSafetyMargin;
};

#endif // !defined(AFX_DESPIKE_H__943EF782_97AC_11D3_A589_00105ADCDFA4__INCLUDED_)
