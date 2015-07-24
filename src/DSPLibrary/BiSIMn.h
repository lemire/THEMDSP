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

#if !defined(_BISIMN_)
#define _BISIMN_


#include "SignalProcessingObject.h"

// a baseline correction class
// this class is no longer used! and should
// be considered experimental!
class BiSIMn : public SignalProcessingObject  
{
		public:
			
			// Late frame width must not be bigger than the framw width
			class LateFrameWidthOversizedException{};
			// the order of the scheme must be 2 or 3, algo is unstable at other values
			class IllegalOrderException{};
			// Late frame widht must not be 0 or negative
			class IllegalLateFrameWidthException{};

			// order =3
			// lateFrameWidth = 48?
			BiSIMn(const int order, const int lateFrameWidth) ;
			virtual ~BiSIMn();
			
			void process(Signal& signal) ;
		private:
			void correctBaseLine(Signal&);
			double getY (const int PositionInFrame);
			void computeCoefs (short * earliersignal, short * signal);
			void computeMatrix();
			void inverse();
			void luDecompose();
			void setStartingPoint(short *);
			int mOrder;
			int mLateFrameWidth;
			double mStartingPoint;
			int mFrameLength;
			double * mLuArray1;
			double * mLuArray2;
			double * mMatrix;
			double * mCoefs;

};


#endif // !defined(AFX_BISIMN_H__D3D8E040_A77E_11D3_A589_00105ADCDFA4__INCLUDED_)
