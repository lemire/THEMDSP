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

#if !defined(AFX_WAVELETSHRINKAGE_H__943EF783_97AC_11D3_A589_00105ADCDFA4__INCLUDED_)
#define AFX_WAVELETSHRINKAGE_H__943EF783_97AC_11D3_A589_00105ADCDFA4__INCLUDED_


#include "ChannelProcessingObject.h"

/**
* Wavelet de-noising for the channels
* This class might need some improvments
* later on. In particular, it is not very
* good at processing signal which don't
* have a power of two length. It is a stupid
* problem, but I don't have time right now
* to correct...
*/
class WaveletShrinkage : public ChannelProcessingObject  
{
	public:
		/**
		*	Constructor
		*   Inputs:
		*		parameter - the strength of the denoising (between 0 (no denoising) and 1 (full denoising))
		*/
		WaveletShrinkage(const float parameter = 0.9f);
		virtual ~WaveletShrinkage();
		void processChannelSet(ChannelSet&);
		// thrown when trying to set a negative parameter
		class NegativeParameterException{};
	private:
		void  shrink(float * data);
		float mParameter;
		void softThresholding(const int length, float * data);
		float threshold (const int length, float * data, float& median, const float p);
		int mChannelLength;
};

#endif // !defined(AFX_WAVELETSHRINKAGE_H__943EF783_97AC_11D3_A589_00105ADCDFA4__INCLUDED_)
