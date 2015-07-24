#include "ExtendedSIMn.h"

#include "SpecialMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ExtendedSIMn::ExtendedSIMn(const int order, const int lateFrameWidth, const int LagrangeOrder) : SIMn(order,lateFrameWidth)
	{
		mLagrangeOrder = LagrangeOrder;
		mLagrangePoints = NULL;
		mExtDelay = 0;
		try {
			mLagrangePoints = new double[mLagrangeOrder+1];
		} catch (std::bad_alloc) {
			// just ignore the error for now, leave the pointer to NULL
		}
}


ExtendedSIMn::~ExtendedSIMn() {
		delete[] mLagrangePoints;
		mLagrangePoints = NULL;
}

//////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////

void ExtendedSIMn::processFirstFrame(short * firstframe, const int framelength) {
	mFrameLength = framelength;
	computeMatrix ();
	setStartingPoint(firstframe);
	computeCoefs(firstframe);
	// on soustrait la ligne de base
	for(int k = 0 ; k < mFrameLength ; k++) {
		firstframe[k] = SpecialMath::roundToShort(firstframe[k] - getY(k));
	}
	mLagrangePointer = 0;
	if (mLagrangePoints != NULL) {
		// just to be safe and make bugs easier to see!
		memset(mLagrangePoints,0,(mLagrangeOrder+1)*sizeof(double));
		mLagrangePoints[0] = getY(mFrameLength - 1 - mLateFrameWidth);
		++mLagrangePointer;
	}

	// mise à jour de StartingPoint
	mStartingPoint = getY(mFrameLength);
}

void ExtendedSIMn::processNextFrame(short * currentframe) {
		if (mLagrangePoints == NULL)
			return;
		computeCoefs(currentframe);
		if((mLagrangePointer < getLagrangeOrder()) || (mLagrangePoints == NULL) ) {
			for(int k = 0 ; k < mFrameLength ; k++) {
				currentframe[k] = SpecialMath::roundToShort(currentframe[k] - getY(k));
			}
			if(mLagrangePoints != NULL) {
				mLagrangePoints[mLagrangePointer++] = getY(mFrameLength - 1 - mExtDelay);
			}
		} else {
			mLagrangePoints[getLagrangeOrder()] = getY(mFrameLength - 1 - mExtDelay);
			for(int x = 0 ; x < mFrameLength ; ++x) {
				currentframe[x] = SpecialMath::roundToShort(currentframe[x] - getLagrangeY(x));
			}
			// memcpy would do, but there isn't much of a performance issue here
			for(int l = 0 ; l < getLagrangeOrder() ; ++l) {
				mLagrangePoints[l] =  mLagrangePoints[l + 1];
			}			
		}
		mStartingPoint = getY(mFrameLength);
}
 

/*
* This code could be optimized quite a bit I suppose.
* No time for now.
*/
double ExtendedSIMn::getLagrangeY(const int x) {
	if( mLagrangePoints == NULL) {
		std::cout << "Something is wrong. I should have a null pointer here." << std::endl;
		return 0.0;
	}
	if( mLagrangePointer != getLagrangeOrder() ) {
		std::cout << "There is a bug in your software man." << std::endl;
		std::cout << "lagrange = " <<getLagrangeOrder()<< std::endl;
		std::cout << mLagrangePointer<< std::endl;
		return 0.0;
	}
	double value = 0.0;
	double valuetemp = 0.0;
	const double xx = x + getLagrangeOrder() * mFrameLength;
	for(int k = 0; k <= getLagrangeOrder() ; ++k) {
		valuetemp = mLagrangePoints[k];
		const double xk = k * mFrameLength + mFrameLength - 1 - mExtDelay;
		for(int l = 0; l <= getLagrangeOrder() ; ++l) {
			if(k != l) {
				const double xl = l * mFrameLength + mFrameLength - 1 - mExtDelay;
				valuetemp *= ( xx - xl )/ ( xk - xl);
			}
		}
		value += valuetemp;
	}
	return value;
}