// TauProjection.h: interface for the TauProjection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_TAUPROJECTION_H_)
#define _TAUPROJECTION_H_

#include <memory.h>
#include <new>
#include <iostream>
#include "SignalProcessingObject.h"
#include "mvvd.h"
#include "mvmd.h"


/*
* This class project the data to a tau space.
*/
class TauProjection : public SignalProcessingObject  
{
public:
	// constructor using default tau values...
	TauProjection() ;

	// constructor specifying tau values
	// (class does not own the pointer!)
	TauProjection(double * tau, const int numberoftauvalues);

	virtual ~TauProjection() ;

	virtual void process(Signal& signal);
	
	static int computeOnset(short * data, const int FrameLength);

	virtual void project(short * framedata, const int FrameLength);

	/*
	* Computing a Tau matrix is fairly expensive.
	* So we construct a factory which will compute
	* new matrices only when needed.
	*/ 
	class TauMatrixFactory {
		public:
			TauMatrixFactory(TauProjection * owner) {
				mOwner = owner;
				mTauMatrix = NULL;
				mLength = 512;// we are fairly generous...
				mTauMatrix = new MV_ColMat_double*[mLength];
				for(int k = 0; k < mLength; ++k)
					mTauMatrix[k] = NULL;
			}
			
			MV_ColMat_double * getTauMatrix(int length) {
				if(length <= 0)
					return NULL;
				if(length > mLength) {
					try {
						MV_ColMat_double ** temp = new MV_ColMat_double*[length];
						memcpy(temp,mTauMatrix,mLength * sizeof(MV_ColMat_double*));
						for (int k = mLength; k < length; ++k) {
							temp = NULL;
						}
						delete[] mTauMatrix;
						mTauMatrix = temp;
					} catch (std::bad_alloc) {
						return NULL;//couldn't allocated memory
					}

				}
				if(mTauMatrix[length] == NULL) {
					if(mOwner != NULL)
						mTauMatrix[length] = mOwner->computeTauMatrix(length);
				}
				return(mTauMatrix[length]);
			}

			virtual ~TauMatrixFactory() {
				for(int k = 0; k < mLength; ++k)
					delete mTauMatrix[k];
				delete[] mTauMatrix;
			}
		private:
			MV_ColMat_double ** mTauMatrix;
			int mLength;
			TauProjection * mOwner;
	};


	MV_ColMat_double * computeTauMatrix(const int FrameLength);

protected:


	double * mTauValues; // tau values...
	int mNumberOfTauValues;
	double mDefaultTauValues[18];
	MV_ColMat_double * mH;// temporary matrix
	MV_Vector_double * X;//temporary vector
	MV_Vector_double * RHS;//temporary vector
	TauMatrixFactory * mTauMatrixFactory;
};

#endif // !defined(_TAUPROJECTION_H_)
