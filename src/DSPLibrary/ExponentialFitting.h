//
// (c) 1999 Daniel Lemire, Ph.D.
//	August 11th 1999 -- modified on March 7th 2001
//
//	(This code is not to be used outside
//	THEM Geophysics software without my written consent.)
//
//

#ifndef EXPFITTING
#define EXPFITTING

#include "TauProjection.h"

/*
* Experimental class, not very useful.
*/
class ExponentialFitting : public TauProjection
{
	public:
		ExponentialFitting(const int increment = 128) {
			mIncrement = increment;
		}
		virtual ~ExponentialFitting() {}

		virtual void project(short * framedata, const int FrameLength);

	protected:

		virtual double makeDecayExponential(const int length, short * framedata, const int signforsigma, int& ErrorFlag) const ;
		virtual double makeDecayExponential(const int length, short * signal, const int signforsigma, const double mu, int& ErrorFlag) const ;
	private:
		double f1 (int length, short * signal, double sigma) const ;
		double findSigma(int length, short * signal, int & ErrorFlag) const ;
		double findMu(int length, short * signal, double Sigma, int & ErrorFlag) const ;

		int mIncrement;
};

#endif

// this file is part
// of the exponential fitting algorithm
// by Daniel Lemire for THEM Geophysics

