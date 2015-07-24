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

#if !defined(_UNICOILFIELDESTIMATOR_)
#define _UNICOILFIELDESTIMATOR_


#include "SignalProcessingObject.h"


/***********
* This signal processing object is meant
* to correct the unicoil field after
* the baseline and anti-spike processing
* have been applied.
****************/
class UnicoilFieldEstimator : public SignalProcessingObject  
{
	public:
		/***************
		* Constructor
		* inputs:
		*	sigma - is the R/L ratio of the unicoil times
		*		a normalization factor (recommended value 0.032)
		*****************/
		UnicoilFieldEstimator(const double sigma);
		// destructor
		virtual ~UnicoilFieldEstimator();
		void process(Signal& signal);
	private:
		double mSigma;
		int mFrameLength;
	
		/****************
		* Attempt to estimate the field
		* values from unicoil data
		* inputs:
		*	currentframe - a frame to be processed
		****************/
		void computeField(short * currentframe);

};

#endif // !defined(AFX_UNICOILFIELDESTIMATOR_H__4E0AAE20_A0F9_11D3_A589_00105ADCDFA4__INCLUDED_)
