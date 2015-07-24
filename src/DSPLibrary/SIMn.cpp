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

#include "PrecompHeaders.h"
#include "SIMn.h"
#include "Signal.h"
#include "SpecialMath.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SIMn::SIMn(const int order, const int lateFrameWidth)
{
	if((order != 2) && (order != 3)&& (order != 4))
		throw IllegalOrderException();
	if (lateFrameWidth < 0)
		throw IllegalLateFrameWidthException();
	mOrder = order;
	mLateFrameWidth = lateFrameWidth;
	mFrameLength = 0;// we don't know yet!
	mLuArray1 = new(std::nothrow) double[(mOrder + 3) * (mOrder + 3)];
	mLuArray2 = new(std::nothrow) double[(mOrder + 3)*(mOrder + 3)];
	mMatrix = new(std::nothrow) double[(mOrder + 3)*(mOrder + 3)];
	mCoefs = new(std::nothrow) double[mOrder + 3];
	if((mLuArray1 == NULL) || (mLuArray2 == NULL) || (mMatrix == NULL) || (mCoefs == NULL)) {
		delete[] mLuArray1;
		delete[] mLuArray2;
		delete[] mMatrix;
		delete[] mCoefs;
		throw OutOfMemoryException();
	}
	memset(mLuArray1,0,(mOrder + 3) * (mOrder + 3) * sizeof(double));
	memset(mLuArray2,0,(mOrder + 3) * (mOrder + 3) * sizeof(double));
	memset(mMatrix,0,(mOrder + 3) * (mOrder + 3) * sizeof(double));
	memset(mCoefs,0,(mOrder + 3) * sizeof(double));
}

SIMn::~SIMn()
{
	delete[] mLuArray1;
	mLuArray1 = NULL;
	delete[] mLuArray2;
	mLuArray2 = NULL;
	delete[] mMatrix;
	mMatrix = NULL;
	delete[] mCoefs;
	mCoefs = NULL;
}


// correct the baseline
void SIMn::process(Signal& signal)
{
	if(signal.getFrameLength() < mLateFrameWidth) {
		throw LateFrameWidthOversizedException();
	}
	correctBaseLine(signal);
}




void SIMn::correctBaseLine(Signal& signal) {
	signal.reset();
	// on vérifie qu'il y a au moins un frame
	if(!signal.hasNextFrame())
		return;
	processFirstFrame(signal.nextFrame(),signal.getFrameLength());
	signal.commit();
	while (signal.hasNextFrame()) {
		processNextFrame(signal.nextFrame());
		signal.commit();
	}
}

void SIMn::processFirstFrame(short * firstframe, const int framelength) {
	mFrameLength = framelength;
	computeMatrix ();
	setStartingPoint(firstframe);
	computeCoefs(firstframe);
	// on soustrait la ligne de base
	for(int k = 0 ; k < mFrameLength ; k++) {
		firstframe[k] = SpecialMath::roundToShort(firstframe[k] - getY(k));
	}
	// on enregistre les changements
	
	// mise à jour de StartingPoint
	mStartingPoint = getY(mFrameLength);
}

void SIMn::processNextFrame(short * currentframe) {
		computeCoefs(currentframe);
		for(int k = 0 ; k < mFrameLength ; k++) {
			currentframe[k] = SpecialMath::roundToShort(currentframe[k] - getY(k));
		}
		mStartingPoint = getY(mFrameLength);
}
 

/**
* Returns the LU decomposition of a matrix.
* 
*/
void SIMn::luDecompose() {
		int i,j,k;
		double tmp;
		mLuArray1[0]=1.0;
		for(i=1;i<mOrder + 3;i++)
			mLuArray1[i*(mOrder+3)+i]=1.0;
		for(j=0;j<mOrder + 3;j++) {
			for(i=0;i<=j;i++) {
				tmp=mMatrix[i*(mOrder+3)+j];
				for(k=0;k<i;k++)
					tmp-=mLuArray1[i*(mOrder+3)+k]*mLuArray2[k*(mOrder+3)+j];
					mLuArray2[i*(mOrder+3)+j]=tmp;
				}
				for(i=j+1;i<mOrder + 3;i++) {
					tmp=mMatrix[i*(mOrder+3)+j];
					for(k=0;k<j;k++)
						tmp-=mLuArray1[i*(mOrder+3)+k]*mLuArray2[k*(mOrder+3)+j];
					mLuArray1[i*(mOrder+3)+j]=tmp/mLuArray2[j*(mOrder+3)+j];
				}
		}
	}


	// Compute the inverse matrix using the LU Decomposition
	// (see above)
void SIMn::inverse() {
	int i,j,k, n;
	double tmpL,tmpU;
	//double array[2][mOrder + 3][mOrder + 3];
	double * array0 = new(std::nothrow) double[(mOrder + 3) * (mOrder + 3)];
	double * array1 = new(std::nothrow) double[(mOrder + 3) * (mOrder + 3)];
	if((array0 == NULL) || (array1 == NULL)) {
		delete[] array0;
		delete[] array1;
		throw OutOfMemoryException();
	}
	for(i = 0; i < mOrder + 3; i++) {
		for(j = 0; j < mOrder + 3; j++) {
			array0[i * (mOrder + 3) + j] = 0.0;
			array1[i * (mOrder + 3) + j] = 0.0;
		}
	}
	luDecompose();
	array0[0]=1.0/mLuArray1[0];
	array1[0]=1.0/mLuArray2[0];
	for(i=1;i<mOrder + 3;i++) {
			array0[i * (mOrder + 3) + i]=1.0/mLuArray1[i*(mOrder+3)+i];
			array1[i * (mOrder + 3) + i]=1.0/mLuArray2[i*(mOrder+3)+i];
	}
	for(i=0;i<mOrder + 3 -1;i++) {
			for(j=i+1;j<mOrder + 3;j++) {
					tmpL=tmpU=0.0;
					for(k=i;k<j;k++) {
							tmpL-=mLuArray1[j*(mOrder+3)+k]*array0[k * (mOrder + 3) + i];
							tmpU-=array1[i * (mOrder + 3) + k]*mLuArray2[k*(mOrder+3)+j];
					}
					array0[j * (mOrder + 3) + i]=tmpL/mLuArray1[j*(mOrder+3)+j];
					array1[i * (mOrder + 3) + j]=tmpU/mLuArray2[j*(mOrder+3)+j];
			}
	}
	//multiply array[1] and array[0]
	{for(j=0;j<mOrder + 3;j++) {
			for(k=0;k<mOrder + 3;k++) {
					mMatrix[j*(mOrder+3)+k]=array1[j * (mOrder + 3) + 0]*array0[0 * (mOrder + 3) + k];
					for(n=1;n<mOrder + 3;n++)
							mMatrix[j*(mOrder+3)+k]+=array1[j * (mOrder + 3) + n]*array0[n * (mOrder + 3) + k];
			}
	}}
	delete[] array0;
	array0 = NULL;
	delete[] array1;
	array1 = NULL;
}

void SIMn::computeMatrix() {
	int row, col, k, power;double temp;
	{for(row = 0; row < mOrder + 3; row++) {
		for(col = 0; col < mOrder + 3; col++) {
			mMatrix[row*(mOrder+3)+col] = 0.0;
		}
	}}
	//Lagrange conditions
    for (row = 0 ; row <= mOrder ; row++) {
      for(col = 0 ; col <= mOrder ; col++) {
        //2 * sum (k^(row+col))  where k goes from k1 to kmax
        for(k = mFrameLength - mLateFrameWidth ; k < mFrameLength ; k++) {
          temp = 2.0;
          for(power = 0; power < col + row ; power++) {
            temp *= k / (double) mFrameLength;
          }
          mMatrix[row*(mOrder+3)+col] += temp;
        }
      }
      // lamda1 : -delta (0,row)
      if(row == 0)
        mMatrix[row*(mOrder+3)+mOrder + 1] = - 1.0;
      // lamda2 : - sum (k ^ row) where k goes form 0 to k1
      for(k = 0; k < mFrameLength - mLateFrameWidth ; k++) {
        temp = 1.0;
        for(power = 0; power < row ; power++) {
           temp *= k / (double) mFrameLength;
        }
        mMatrix[row*(mOrder+3)+mOrder + 2] += temp;
      }
      mMatrix[row*(mOrder+3)+mOrder + 2] *= -1;
    }
    //boundary condition
    row = mOrder + 1;
    mMatrix[row*(mOrder+3)+0] = 1;
    //zero mass condition
    // sum(k^col) where k goes from 0 to k1
    row = mOrder + 2;
    for(col = 0 ; col <= mOrder ; col++) {
      for (k = 0; k < mFrameLength /*- mLateFrameWidth*/ ; k++) {
        temp = 1.0;
        for(power = 0; power < col ; power++) {
          temp *= k / (double) mFrameLength;
        }
        mMatrix[row*(mOrder+3)+col] += temp;
      }
    }
	inverse();
}

/**
* The first n+1 coefficients are the
* coefficients defining the polynomial
* and the following 2 coefficients are
* Lagrange multipliers.
* You must provide the frame of the signal
* (512 samples) together with the
* starting point (mStartingPoint).
*/
 void SIMn::computeCoefs (short * signal) {
  	int row, col, k, power;

    double * vector = new(std::nothrow) double[mOrder + 3];
	if(vector == NULL)
		throw OutOfMemoryException();
	for(k = 0; k < mOrder + 3 ; k++) {
		vector[k] = 0.0;
	}//memset
//	memset(vector,0,(mOrder + 3) * sizeof(double));
    for (row = 0; row <= mOrder ; row++) {
      for( k = mFrameLength - mLateFrameWidth; k < mFrameLength ; k++) {
		  int temp = signal[k] << 1;
		  double essai = temp;
	//	assert (essai < 10000);
	//	assert (essai > -10000);
        for(power = 0; power < row ; power++) {
          essai *= k / (double) mFrameLength;
        }
        vector[row] += essai;
      }
    }
    vector[mOrder + 1] = mStartingPoint;
    for(k = 0; k < mFrameLength /*- mLateFrameWidth*/ ; k++) {
      vector[mOrder + 2] += signal[k];
    }
	// compute matrix * vector
	for(row = 0; row < mOrder + 3; row ++) {
		mCoefs[row] = 0.0;
		for (col = 0; col < mOrder + 3; col++) {
			mCoefs[row] += mMatrix[row*(mOrder+3)+col] * vector[col];
		}
	}
	delete[] vector;
	vector = NULL;
	//std::cout << "mCoefs[mOrder + 1] = "<<mCoefs[mOrder + 1]<<std::endl;
	//std::cout << "mCoefs[mOrder + 2] = "<<mCoefs[mOrder + 2]<<std::endl;
  }
  
 double SIMn::getY (const int PositionInFrame) {
	assert(PositionInFrame >= 0);
	assert(PositionInFrame < 2000);
  	int k, power; double temp;
    double ans = 0;
	double x = PositionInFrame / (double) mFrameLength;
    for(k = 0 ; k < mOrder + 1 ; k++) {
      temp = mCoefs[k];
      for (power = 0; power < k ; power++) {
        temp *= x;
      }
      ans += temp;
    }
    return(ans);
}
  
  
	/**
	* The Starting point was estimated roughly
	* in previous versions.
	* We are now more careful and we use a
	* statistical approach.
	*
	*/

 void SIMn::setStartingPoint(short * firstframe) {
	assert(mFrameLength > 0);
	int StartingPointWidth = (mFrameLength / 4);
	assert(StartingPointWidth <= mFrameLength);
	double * EndOfFirstFrame = new(std::nothrow) double[StartingPointWidth];
	double * XValues = new(std::nothrow) double[StartingPointWidth]; 
	if((EndOfFirstFrame == NULL) || (XValues == NULL)) {
		delete[] EndOfFirstFrame;
		delete[] XValues;
		throw OutOfMemoryException();
	}
	for (int k = 0 ; k < StartingPointWidth ; k++) {
		EndOfFirstFrame [k] = firstframe[k + mFrameLength - StartingPointWidth];
		XValues [k] = k + mFrameLength - StartingPointWidth;
	}
	double slope = SpecialMath::correlation(StartingPointWidth,EndOfFirstFrame,XValues);
	slope *= sqrt(SpecialMath::variance(StartingPointWidth, EndOfFirstFrame));
	slope /= sqrt(SpecialMath::variance(StartingPointWidth, XValues));
	mStartingPoint = SpecialMath::mean(StartingPointWidth,EndOfFirstFrame) + slope * (0 - SpecialMath::mean(StartingPointWidth,XValues));
	delete[] EndOfFirstFrame;
	EndOfFirstFrame = NULL;
	delete[] XValues;
	XValues = NULL;

 }