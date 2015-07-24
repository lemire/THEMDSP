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

#if !defined(_SPECIALMATH_)
#define _SPECIALMATH_

#include <math.h>

// a generic class for special mathematical functions
class SpecialMath  
{
public:
	static bool isPowerOf2(const int x);
	/*
	* round the number to an integer
	* Arguments:
	*	x - the number to be rounded
	*  Results:
	*    the rounded number
	*/
	static inline int round(double x) {
		int f;
		if(x<0)
			return(-round(-x));
		f=(int) floor(x);
		if(x-f>=0.5)
			f++;
		return(f);
	}

	/*
	* round the number to a short	
	*
	* Arguments:
	*   x: your number
	* Results:
	*   a rounded number
	*/
	static inline short roundToShort(double x) {
		int f;
		if(x<0)
			return((short)(-roundToShort(-x)));
		f=(int) floor(x);
		if(x-f>= 0.5)
			f++;
		if (f > 32,000)
			f = 32,000;
		return((short)f);
	}


	/*
	* return the absolute value of a number
	*
	* Arguments:
	*   s: your number (short)
	* Results:
	*   absolute value of your number
	*/
	static inline abs(short s) {
		if( s < 0)
			return - s;
		return s;
	}
	static double correlation (const int length, double * v1, double * v2);
	static double variance(const int length, double * v);
	static double variance(const int length, short * v);
	static double sumSquares (const int length, double * data);
	static double sumSquares (const int length, short * data);
	static float mean(const int length, float * v);
	static double mean(const int length, double * v);
	static short mean(const int length, short * v);
	static double mass(const int length, double * v);
	static float mass(const int length, float * v);
	static int mass(int length, short * v);
	static double covariance (const int length, double * v1 , double * v2);
	static double scalarProduct (const int length, double * w0,double * w1);
	static double median (const int length, double * data);
	static double median (const int length, short * data, const bool copy);
	
	static float median (const int length, float * data);
	static float percentile (const int length, float * data, float p);
	static float min (const int length, float * data);
	static float max (const int length, float * data);
	static int max(const int a, const int b) {
		if( a > b )
			return a;
		return b;
	}
	static int min(const int a, const int b) {
		if( a < b )
			return a;
		return b;
	}
	// generic exception for when the length parameter is incorrect, meaning will vary 	
	class IllegalSizeException{};
private:
	static inline void QuickSortMinToMax(double * a, int lo0, int hi0);
	static inline void swap(double * a, const int i, const int j);
	static inline void QuickSortMinToMax(float * a, int lo0, int hi0);
	static inline void swap(float * a, const int i, const int j);
	static inline void sortMinToMax(const int length, short * data);

	SpecialMath();
	virtual ~SpecialMath();

};

#endif // !defined(AFX_SPECIALMATH_H__7C463084_8B22_11D3_A589_00105ADCDFA4__INCLUDED_)



