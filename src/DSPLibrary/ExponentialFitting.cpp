//
// (c) 1999 Daniel Lemire, Ph.D.
//	August 11th 1999 -- updated for the new lib on March 7th, 2001
//
//	(This code is not to be used outside
//	THEM Geophysics software without my written consent.)
//
// General Usage :(OLD DOCUMENTATION)
//
//	This code is meant to be used right after the baseline
// correction and right before the stacking and the rest of
// the processing.
//
// Simply do something like this :
// 
//	result = makeDecayExponential(length, (short *) &signal);
//	if(result == -1) {
//		printf("Processing failed. Aborting...");
//		exit(1);
//	}
//
//	where length is length of the signal and signal is the signal
// 	as an array of shorts.
//
// Don't forget to include "ExponentialFitting.h".
//
// That's it. Only one function to call.
//
// You don't need to process all of the signal at once. You can process
// each frame if you want to. Just make sure you feed a multiple of 512
// samples. That's all.
//
//
//	Comment :
//
//	This code should be robust and simple to use but it might
//	be CPU-intensive in some contexts. Depending on the needed
//	accuracy, some parameters can be ajusted to speed things up.
//	When working with exponentials, one must be very careful
//	so this code was conservative on purpose. 
//
//	Memory consumption is very low.
//
//
//	Support :
//
//		Please write to Daniel.Lemire@Videotron.Ca if you have
//		any question or comments about this code. You can also
//		call me at (514) 524-3517. I'll be glad to help you.
//

#include "ExponentialFitting.h"
#include <math.h>
#include "SpecialMath.h"


  //
  //	For internal use only.
  //
inline double ExponentialFitting::f1 (int length, short * signal, double sigma)  const {
    double sum1 = 0.0;
    double sum2 = 0.0;
    double sum3 = 0.0;
    double sum4 = 0.0;
    for (int k = 0 ; k < length ; k++) {
      sum1 += k * exp(- sigma * k) * signal[k];
    }
    for (k = 0 ; k < length ; k++) {
      sum2 += exp(- 2 * sigma * k);
    }
    for (k = 0 ; k < length ; k++) {
      sum3 += exp(- sigma * k) * signal[k];
    }
    for (k = 0 ; k < length ; k++) {
      sum4 += k * exp(- 2 * sigma * k);
    }
    return(sum1 * sum2 - sum3 * sum4);
  }

  //
  // For internal use only.
  // The error flag will be set to -1 if there is 
  // a problem.
  //
  inline double ExponentialFitting::findSigma(int length, short * signal, int & ErrorFlag)  const {
    double sigma = 0.0;
    double epsilon = 0.0001; 
    double oldvalue = f1(length, signal, sigma);
    double newvalue = oldvalue;
    double derivative, deltaX;

    for (int k = 0 ; k < 50 ; k++) { // a maximum of a hundred iterations 
      oldvalue = newvalue;
      derivative = (f1(length, signal, sigma + epsilon) - oldvalue) / epsilon;
      if(derivative == 0.0)
		  break;
	  deltaX = oldvalue / derivative;
      sigma -= deltaX;
      newvalue = f1(length, signal, sigma);
      if(fabs(newvalue) < epsilon)
        break;
    }
    if(abs(newvalue) > 1000 * epsilon) // check if the result is really bad
        ErrorFlag = -1;
    if(sigma > 3.0 )
		ErrorFlag = -1; // Algorithm unstable at these values
    return(sigma);

  }

  //
  // For internal use only.
  // The error flag will be set to -1 if there is 
  // a problem.
  //
  inline double ExponentialFitting::findMu(int length, short * signal, double Sigma, int & ErrorFlag)  const {
    double sum1 = 0.0;
	double sum2 = 0.0;
    for(int k = 0 ; k < length ; k++) {
      sum1 += signal[k] * exp(- Sigma * k);
    }
    for(k = 0; k < length ; k ++) {
      sum2 += exp(- 2 * Sigma * k);
    }
	if(sum2 !=0)
    	return(sum1 / sum2);
	ErrorFlag = -1;
	return(0.0);
  }

void ExponentialFitting::project(short * framedata, const int FrameLength) { 
	// we cut the offtime into components of 128 samples
	// each, this way, we expect to loose as little information
	// as possible. 128 is arbitrary here, of course and should
	// be an attribute of this object.
	const int increment = mIncrement;
	int beginning = computeOnset(framedata,FrameLength);
	int end = beginning + increment;
	const int signforsigma = 1;// or enable disable sign checking
	int ErrorFlag = 0;
	double mu = makeDecayExponential(end + 1 - beginning, framedata + beginning, signforsigma,ErrorFlag);
	while (end < FrameLength) {
		mu = makeDecayExponential(end + 1 - beginning, framedata + beginning, signforsigma, mu, ErrorFlag);
		beginning += increment;
		end += increment;
	}
	end = FrameLength - 1;
	makeDecayExponential(end + 1 - beginning, framedata + beginning, signforsigma, mu, ErrorFlag);
}

double ExponentialFitting::makeDecayExponential(const int length, short * framedata, const int signforsigma, int& ErrorFlag)  const  {
	double sigma = findSigma(length,framedata,ErrorFlag);
	if(sigma * signforsigma < 0) {
		sigma = 0.0;
	} 
	if(ErrorFlag == -1)
		return 0.0;
	const double mu = findMu(length, framedata, sigma, ErrorFlag);
	for (int l = 0; l < length ; ++l) { 
			framedata [l] = SpecialMath::roundToShort( mu * exp( - sigma * l)); 
	}
  	return(mu * exp( - sigma * length ));
  }

double ExponentialFitting::makeDecayExponential(const int length, short * framedata, const int signforsigma, const double mu, int& ErrorFlag) const {
	double sigma = findSigma(length,framedata,ErrorFlag);
	if(sigma * signforsigma < 0) {
		sigma = 0.0;
	} 
	for (int l = 0; l < length ; ++l) { 
		framedata [l] = SpecialMath::roundToShort( mu * exp( - sigma * l)); 
	}
  	return(mu * exp( - sigma * length));
}
  
// this file is part
// of the exponential fitting algorithm
// by Daniel Lemire for THEM Geophysics

