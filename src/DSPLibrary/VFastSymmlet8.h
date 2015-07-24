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


#ifndef VFASTSYMMLET8_
#define VFASTSYMMLET8_




	  static const float scale[8] = {0.0322231006040782f,
	                                -0.0126039672622638f,
	                                -0.0992195435769564f,
	                                0.297857795605605f,
	                                0.803738751805386f,
	                                0.497618667632563f,
	                                -0.0296355276459604f,
	                                -0.0757657147893567f};
	  static const float wavelet[8] = {0.0757657147893567f,
	  								-0.0296355276459604f,
	  								-0.497618667632563f,
	  								0.803738751805386f,
	  								-0.297857795605605f,
	  								-0.0992195435769564f,
	  								0.0126039672622638f,
	  								0.0322231006040782f};

/************************************************
* An efficient implementation of
* the Fast Wavelet Transform using
* Symmlet8 wavelets (FBI fingerprints standard)
*************************************************/
class VFastSymmlet8 {
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
		*
		* Arguments:
		*		maxiter - number of iterations
		*		length - length of the signal
		*		v - the actual data
		*/
		static void invTransform (const int maxiter,const int length,float * v);	
		
		/*
		* expand the signal according to the current filter
		*
		* inputs:
		*		length - length of the signal
		*		v - the actual data
		*/
		static void expand (int length, float * v);

		// a minimum length is 8
		class SignalSmallerThan8Exception{};
		// signal must not be too small for the number of iterations
		class SignalTooSmallException{};
		// internal error, lengths should be divisble by two several times according to the number of iterations
		class NotDivisibleBy2Exception{};
	
		

		
	private:


		static void invTransform(const int,float *,const  int);
		static void transform(const int,float *,const  int);
				

};
#endif
