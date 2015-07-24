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

#if !defined(_SIMN_)
#define _SIMN_


#include "SignalProcessingObject.h"
// a baseline correction class
class SIMn : public SignalProcessingObject  
{
		public:
			
			// order =3
			// lateFrameWidth = 48?
			SIMn(const int order, const int lateFrameWidth);
			virtual ~SIMn();
			
			virtual void process(Signal& signal);

			virtual void processFirstFrame(short * firstframe, const int framelength);

			virtual void processNextFrame(short * currentframe);

			// Late frame width must not be bigger than the framw width
			class LateFrameWidthOversizedException{};
			// the order of the scheme must be 2 or 3, algo is unstable at other values
			class IllegalOrderException{};
			// Late frame widht must not be 0 or negative
			class IllegalLateFrameWidthException{};
		protected:
			
			virtual void computeMatrix();
			virtual void setStartingPoint(short *);
			virtual void computeCoefs (short * signal);
			double getY (const int PositionInFrame);
			int mFrameLength;
			int mLateFrameWidth;
			double mStartingPoint;

		private:

			void correctBaseLine(Signal& signal);
			void inverse();
			void luDecompose();
			
			int mOrder;
			double * mLuArray1;
			double * mLuArray2;
			double * mMatrix;
			double * mCoefs;

};

#endif // !defined(AFX_SIMN_H__943EF780_97AC_11D3_A589_00105ADCDFA4__INCLUDED_)
