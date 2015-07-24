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

#ifndef VFASTSYMMLET4_
#define VFASTSYMMLET4_




	  static const float scale4[4] = {0.482962913144534f,
									0.836516303737808f,
									0.224143868042013f,
									-0.12940952255126f};
	  static const float wavelet4[4] = {-0.12940952255126f,
	  								-0.224143868042013f,
									0.836516303737808f,
									-0.482962913144534f};
/************************************************
* An efficient implementation of
* the Fast Wavelet Transform using
* Symmlet4 wavelets
*************************************************/
class VFastSymmlet4 {
	public:
		/*
		* transform the signal
		* inputs:
		*		maxiter - number of iterations
		*		length - length of the signal
		*		v - the actual data
		*/	
		static void transform (const int maxiter,const int length,float * v);		

		/*
		* inverse transform the signal
		* inputs:
		*		maxiter - number of iterations
		*		length - length of the signal
		*		v - the actual data
		*/
		static void invTransform (const int maxiter,const int length,float * v);	

		/*
		* expand the signal according to the current filter
		* inputs:
		*		length - length of the signal
		*		v - the actual data
		*/
		static void expand (const int length, float * v);
		// a minimum length is 8
		class SignalSmallerThan8Exception{};
		// signal must not be too small for the number of iterations
		class SignalTooSmallException{};
		// internal error, lengths should be divisble by two several times according to the number of iterations
		class NotDivisibleBy2Exception{};
	
		

		
	private:

		
		static void invTransform(const int,float *, const int);
		static void transform(const int,float *, const int);

		
};
#endif
