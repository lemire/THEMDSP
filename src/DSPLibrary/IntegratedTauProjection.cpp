// IntegratedTauProjection.cpp: implementation of the IntegratedTauProjection class.
//
//////////////////////////////////////////////////////////////////////

#include "IntegratedTauProjection.h"
#include <iostream>
#include <math.h>
#include "mvvd.h"
#include "gmres.h"
#include "SpecialMath.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IntegratedTauProjection::IntegratedTauProjection()
{
		mNumberOfTauValues = 18;
		for(int k = 0; k < mNumberOfTauValues; ++k) {
			mDefaultTauValues[k] = 0.001 * k ; 
		}
		mTauValues =(double *) & mDefaultTauValues;

}

IntegratedTauProjection::IntegratedTauProjection(double * tau, const int numberoftauvalues) : TauProjection(tau, numberoftauvalues) {
}


IntegratedTauProjection::~IntegratedTauProjection()
{

}


void IntegratedTauProjection::project(short * framedata, const int FrameLength) { 
	const int onset = computeOnset(framedata,FrameLength);
	int * integral = new(std::nothrow) int[FrameLength];
	if(integral == NULL)
		return;
	// memset would do
	{for(int k = 0; k < FrameLength; k++) {
		integral[k] = 0;
	}}
	{for(int k = FrameLength - 2; k >= 0; --k) {
		integral[k] = integral[k + 1] + SpecialMath::round((framedata[k + 1] + framedata[k])/ 2.0);
	}}
	if(mTauValues == NULL) {
		for(int k = onset; k < FrameLength; ++k) {
			framedata[k] = integral[k];
		}
		return;
	}
	MV_ColMat_double * taumatrix = mTauMatrixFactory->getTauMatrix(FrameLength - onset);
	if( taumatrix == NULL) {
		std::cout << "Could not allocate matrix! Aborting projection!" << std::endl;
		delete[] integral;
		return;// we abort!
	}

	(*RHS) = 0.0;
	for (int t = 0; t < mNumberOfTauValues; ++t) {
		for(int k = onset; k < FrameLength; ++k) {
			(*RHS)(t) += integral[k] * exp(-mTauValues[t] * (k - onset));
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


