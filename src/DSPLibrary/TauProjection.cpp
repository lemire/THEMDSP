// TauProjection.cpp: implementation of the TauProjection class.
//
//////////////////////////////////////////////////////////////////////

#include "TauProjection.h"
#include <iostream>
#include <math.h>
#include "mvvd.h"
#include "gmres.h"
#include "SpecialMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TauProjection::TauProjection()
{
		mNumberOfTauValues = 18;
		for(int k = 0; k < mNumberOfTauValues; ++k) {
			mDefaultTauValues[k] = 0.001 * k ; 
		}
		mTauValues =(double *) & mDefaultTauValues;
		mH = NULL;
		X = NULL;
		try {
			mTauMatrixFactory = new TauMatrixFactory(this);
			mH = new MV_ColMat_double(mNumberOfTauValues,mNumberOfTauValues);// temporary matrix
			(*mH) = 0;
			X = new MV_Vector_double(mNumberOfTauValues);//temporary vector
			(*X) = 0;
			RHS = new MV_Vector_double(mNumberOfTauValues);
			(*RHS) = 0;
		} catch (std::bad_alloc) {
			delete mH;
			delete mTauMatrixFactory;
			delete X;
			delete RHS;
			throw;
		}
}

TauProjection::TauProjection(double * tau, const int numberoftauvalues) {
		mNumberOfTauValues = numberoftauvalues;
		mTauValues = tau; // tau values...
		mH = NULL;
		X = NULL;
		try {
			mTauMatrixFactory = new TauMatrixFactory(this);
			mH = new MV_ColMat_double(mNumberOfTauValues,mNumberOfTauValues);// temporary matrix
			(*mH) = 0;
			X = new MV_Vector_double(mNumberOfTauValues);//temporary vector
			(*X) = 0;
			RHS  = new MV_Vector_double(mNumberOfTauValues);
			(*RHS) = 0;
		} catch (std::bad_alloc) {
			delete mH;
			delete mTauMatrixFactory;
			delete X;
			delete RHS;
			throw;
		}
}


TauProjection::~TauProjection()
{
			delete mH;
			delete mTauMatrixFactory;
			delete X;
			delete RHS;
}

void TauProjection::process(Signal& signal) {
		signal.reset();
		while (signal.hasNextFrame()) {
			short * frame = signal.nextFrame();
			project(frame,signal.getFrameLength());
			signal.commit();
		}		
}

void TauProjection::project(short * framedata, const int FrameLength) { 
	const int onset = computeOnset(framedata,FrameLength);
//	std::cout << "onset = " << onset << std::endl;
	MV_ColMat_double * taumatrix = mTauMatrixFactory->getTauMatrix(FrameLength - onset);
	/*for(int k = 0; k < 18; ++k) {
		for(int l = 0; l < 18; ++l) {
			std::cout << (*taumatrix) (k,l);
		}
		std::cout << std::endl;
	}*/
	if( taumatrix == NULL) {
		std::cout << "Could not allocate matrix! Aborting projection!" << std::endl;
		return;// we abort!
	}
	(*RHS) = 0.0;
	for (int t = 0; t < mNumberOfTauValues; ++t) {
		for(int k = onset; k < FrameLength; ++k) {
			(*RHS)(t) += framedata[k] * exp(-mTauValues[t] * (k - onset));
		}
		
	}
	int iter = mNumberOfTauValues * 100;
	double tol = 0.000000001;
	(*mH) = 0.0;
	(*X) *= -1.0;
	int result = GMRES<MV_ColMat_double,MV_Vector_double,MV_ColMat_double,double>(*taumatrix,*X,*RHS, *mH,iter ,tol);
	//std::cout << "iter = " << iter << std::endl;
	//std::cout << "tol = " << tol << std::endl;
	if(!result) {
		//double error = 0.0;
		//double error2 = 0.0;
		double r = 0.0;
		//std::cout << (*X)(0) << std::endl;
		//std::cout << (*X)(1) << std::endl;
		//std::cout << (*X)(2) << std::endl;
		for(int k = onset; k < FrameLength; ++k) {
			r = 0.0;
			for (int t = 0; t < mNumberOfTauValues; ++t) {
				//if(mTauValues[t] != 0) {
				//	const double ratio = (mTauValues[1] / mTauValues[t] );
				//	r += (*X)(t) * (mTauValues[1] / mTauValues[t] )* ( exp(-mTauValues[t] * (k-onset))  -exp(-mTauValues[t] * (FrameLength - 1 - onset))       );
				//}
				r += (*X)(t) * exp(-mTauValues[t] * (k-onset));
			}
			//error += (framedata[k] - SpecialMath::roundToShort(r)) *(framedata[k] - SpecialMath::roundToShort(r))/(FrameLength - onset);
			//error2 += (framedata[k]) *(framedata[k])/(FrameLength - onset);
			framedata[k] = SpecialMath::roundToShort(r);
		}
	//	std::cout << "RMS = " << sqrt(error)<< std::endl;
	//	std::cout << "RMS = " << sqrt(error2)<< std::endl;
	} else {
		std::cout << "*********GMRES failed! Aborting.************"<< std::endl;
	}
}

int TauProjection::computeOnset(short * data, const int FrameLength) {
	if (FrameLength < 10)
		return 0;// if FrameLength doesn't make sense, return 0!
	// arbitrarily, we set a wide range when the onset (or offset) of the
	// offtime should begin...
	const int begin = FrameLength/10;
	const int end = FrameLength/ 5;
	// we then find the maximum
	short max = SpecialMath::abs(data[begin + 1] - data[begin]);
	int locate = begin;
	for (int k = begin; k < end; ++k) {
		if(SpecialMath::abs(data[k + 1] - data[k]) > max) {
			locate = k;
			max = SpecialMath::abs(data[k]);
		}
	}
	// ok, now locate indicate the last peak... adding 5 to the value
	// should approximate a safe onset value... this is somewhat
	// an ad hoc choice, but it is difficult to predict the
	// exact morphology
	return(locate + 5);

}
	
MV_ColMat_double * TauProjection::computeTauMatrix(const int length) {
	MV_ColMat_double * ans = NULL;
	try {
		ans = new MV_ColMat_double(mNumberOfTauValues,mNumberOfTauValues);
		(*ans) = 0.0;
		{for(int k = 0 ; k < mNumberOfTauValues; ++k) {
			for(int l = 0 ; l <=k; ++l) {
				for(int s = 0; s < length; ++s) {
					(*ans)(k,l) += exp(- (mTauValues[k] + mTauValues[l]) * s ); 
				}
			}
			
		}}
		{for(int k = 0 ; k < mNumberOfTauValues; ++k) {
			for(int l = k ; l < mNumberOfTauValues; ++l) {
				(*ans)(k,l) = (*ans)(l,k);
			}
		}}
	} catch (std::bad_alloc) {
		// no much we can do
	}
	return ans;
}




