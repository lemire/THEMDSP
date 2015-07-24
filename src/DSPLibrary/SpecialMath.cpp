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
#include "SpecialMath.h"


//private
SpecialMath::SpecialMath() {}
//private
SpecialMath::~SpecialMath() {}



// compute the correlation
double SpecialMath::correlation (int length, double * v1, double * v2) {
	if(length <= 0)
			throw IllegalSizeException();
    double denom=sqrt(variance(length, v1)*variance(length, v2));
    if(denom!=0)
      return(covariance(length, v1,v2)/denom);
    else {
      if((variance(length, v1)==0) &&(variance(length, v2)==0))
        return(1.0);
      else return(0.0);  // impossible to correlate a null signal with another
    }
}

// compute the variance
double SpecialMath::variance(int length, double * v) {
		if(length <= 0)
			throw IllegalSizeException();
		double m=SpecialMath::mean(length,v);
		double ans=SpecialMath::sumSquares(length,v)/(double)length-m*m;
		return(ans);
}

// compute the variance
double SpecialMath::variance(int length, short * v) {
		if(length <= 0)
			throw IllegalSizeException();
		double m=SpecialMath::mean(length,v);
		double ans=SpecialMath::sumSquares(length,v)/(double)length-m*m;
		return(ans);
}

// compute the sum of all squares
double SpecialMath::sumSquares (int length, double * data) {
		if(length < 0)
			throw IllegalSizeException();
		double ans=0.0;
		for(int k=0;k<length;k++) {
			ans+=data[k]*data[k];
		}
		return(ans);
}


// compute the sum of all squares
double SpecialMath::sumSquares (const int length, short * data) {
		if(length < 0)
			throw IllegalSizeException();
		double ans=0.0;
		for(int k=0;k<length;k++) {
			ans+=data[k]*data[k];
		}
		return(ans);
}

// compute the mean
double SpecialMath::mean(int length, double * v) {
		if(length <= 0)
			throw IllegalSizeException();
 		return(mass(length, v)/(double)length);
}

// compute the mean
float SpecialMath::mean(int length, float * v) {
		if(length <= 0)
			throw IllegalSizeException();
 		return(mass(length, v)/(float)length);
}

short SpecialMath::mean(int length, short * v) {
		if(length <= 0)
			throw IllegalSizeException();
 		return(roundToShort(mass(length, v)/(double)length));
}

// compute the mass (sum of all elements)
double SpecialMath::mass(int length, double * v) {
		if(length < 0)
			throw IllegalSizeException();
		double somme = 0.0;
		for(int k=0;k<length;k++) {
			somme+=v[k];
		}
		return(somme);
}

// compute the mass (sum of all elements)
float SpecialMath::mass(int length, float * v) {
		if(length < 0)
			throw IllegalSizeException();
		float somme = 0.0f;
		for(int k=0;k<length;k++) {
			somme+=v[k];
		}
		return(somme);
}
// compute the mass (sum of all elements)
int SpecialMath::mass(int length, short * v) {
		if(length < 0)
			throw IllegalSizeException();
		int somme=0;
		for(int k=0;k<length;k++) {
			somme+=v[k];
		}
		return(somme);
}
// compute the covariance
double SpecialMath::covariance (int length, double * v1 , double * v2) {
	if(length <= 0)
			throw IllegalSizeException();
    double m1 = mean(length, v1);
    double m2 = mean(length, v2);
    double ans = SpecialMath::scalarProduct(length, v1,v2)/(double)length - m1 * m2;
    return(ans);
}

// compute the scalar product
double SpecialMath::scalarProduct (int length, double * w0,double * w1) {
		if(length < 0)
			throw IllegalSizeException();
		double sortie=0.0;
		for(int k=0;k<length;k++){
			sortie+=w0[k]*w1[k];
		}
		return(sortie);
}

/*
* is the integer a power of 2?
* 
* Arguments
*    x - the integer
*
* Results
*    a boolean indicating whether it is a power of two or not
*/
bool SpecialMath::isPowerOf2(const int x)
{
	int BITS_PER_WORD = 32;
	int i, y;
	
	for(i = 1, y = 2; i < BITS_PER_WORD; i++, y <<= 1)
	{
		if(x == y)			return 1;
	}	
	return 0;
}



void SpecialMath::swap(double * a, const int i, const int j)
{
   double T = a[i];
   a[i] = a[j];
   a[j] = T;
}

void SpecialMath::QuickSortMinToMax(double * a, int lo0, int hi0)
{

	int lo = lo0;
  	int hi = hi0;
  	double mid;

  	if (hi0 > lo0)
  	{

	 	// Arbitrarily establishing partition element as the midpoint of
		// the array.
	 	mid = a[(int) round(( lo0 + hi0 ) / 2.0f)];

		// loop through the array until indices cross
		while (lo <= hi)
	 	{
			// find the first element that is greater than or equal to
			// the partition element starting from the left Index.
			while(( lo < hi0 )  && ( a[lo] < mid ))
	 			++lo;

			// find an element that is smaller than or equal to
			// the partition element starting from the right Index.
	 		while(( hi > lo0 )  && ( a[hi] > mid ))
	 			--hi;

			// if the indexes have not crossed, swap
			if(lo <= hi)
			{
		   		swap(a, lo, hi);
		   		++lo;
		   		--hi;
			}
	 	}

	 	// If the right index has not reached the left side of array
	 	// must now sort the left partition.
		if(lo0 < hi)
			QuickSortMinToMax(a, lo0, hi );

	 	// If the left index has not reached the right side of array
	 	// must now sort the right partition.
	 	if( lo < hi0 )
			QuickSortMinToMax( a, lo, hi0 );

  	}
}


// compute the median of the array   
double SpecialMath::median (const int length, double * data)
{
	if(length <= 0)
		throw IllegalSizeException();
	double * copy = new(std::nothrow) double[length];
	if(copy == NULL)
		throw OutOfMemoryException();
	memcpy(copy,data,length * sizeof(double));
	QuickSortMinToMax(copy, 0, length - 1);
	int k = length >> 1;
	double ans = 0;
	if((k << 1) < length)
		ans = copy[k];
	else 
		ans = (copy[k] + copy[k-1])/2.0;
	delete[] copy;
	copy = NULL;
	return(ans);
}

void SpecialMath::sortMinToMax(const int length, short * data) {
	short temp = 0;
	for (int i = 0; i < length; ++i) {
			for (int j = i; (j > 0) && (data[j - 1] - data[ j ] > 0); j--) {
				temp = data[j - 1];
				data[j - 1] = data[j];
				data[j] = temp;
			}
		}
}

// compute the median of the array   
double SpecialMath::median (const int length, short * data, const bool copy)
{
	if(length <= 0)
		throw IllegalSizeException();
	short * copyofdata = NULL;
	if( copy ) {
		copyofdata = new(std::nothrow) short[length];
		if(copyofdata == NULL)
			throw OutOfMemoryException();
		memcpy(copyofdata,data,length * sizeof(short));
	} else {
		copyofdata = data;
	}
	sortMinToMax(length, copyofdata);
	int k = length >> 1;
	double ans = 0;
	if((k << 1) < length) {// 1 -> k = 0, 0 << 1 = 0 < 1

		ans = copyofdata[k];

	} else {// 2 -> k = 1, 1 << 1 = 2 == length = 2
		int sum = copyofdata[k - 1] + copyofdata[k];
		ans = sum / 2 + sum % 2;
		//ans = (copyofdata[k] + copyofdata[k+1])/2.0;
	}
	if( copy) {
		delete[] copyofdata;
		copyofdata = NULL;
	}
	return(ans);
}



void SpecialMath::swap(float * a, const int i, const int j)
{
   float T = a[i];
   a[i] = a[j];
   a[j] = T;
}


void SpecialMath::QuickSortMinToMax(float * a, int lo0, int hi0)
{

	int lo = lo0;
  	int hi = hi0;
  	float mid;

  	if (hi0 > lo0)
  	{

	 	// Arbitrarily establishing partition element as the midpoint of
		// the array.
	 	mid = a[(int) round(( lo0 + hi0 ) / 2.0f)];

		// loop through the array until indices cross
		while (lo <= hi)
	 	{
			// find the first element that is greater than or equal to
			// the partition element starting from the left Index.
			while(( lo < hi0 )  && ( a[lo] < mid ))
	 			++lo;

			// find an element that is smaller than or equal to
			// the partition element starting from the right Index.
	 		while(( hi > lo0 )  && ( a[hi] > mid ))
	 			--hi;

			// if the indexes have not crossed, swap
			if(lo <= hi)
			{
		   		swap(a, lo, hi);
		   		++lo;
		   		--hi;
			}
	 	}

	 	// If the right index has not reached the left side of array
	 	// must now sort the left partition.
		if(lo0 < hi)
			QuickSortMinToMax(a, lo0, hi );

	 	// If the left index has not reached the right side of array
	 	// must now sort the right partition.
	 	if( lo < hi0 )
			QuickSortMinToMax( a, lo, hi0 );

  	}
}

/*
* Return the max value of an array
* 
* Arguments:
*   length: length of the array
*   data : pointer to the array
*
* Results :
*  the maximum
*/
float SpecialMath::max (const int length, float * data) {
	if(length <= 0)
		throw IllegalSizeException();
	float max = data[0];
	for (int k = 0; k < length ; k++) {
		max = max > data[k] ? max : data[k];
	}
	return(max);
}

/*
* Return the min value of an array
* 
* Arguments:
*   length: length of the array
*   data : pointer to the array
*
* Results:
*  the minimum
*/
float SpecialMath::min (const int length, float * data) {
	if(length <= 0)
		throw IllegalSizeException();
	float min = data[0];
	for (int k = 0; k < length ; k++) {
		min = min < data[k] ? min : data[k];
	}
	return(min);
}

/*
* Return the median value of an array
* 
* Arguments:
*   length: length of the array
*   data : pointer to the array
*
* Results: 
*  the median
*/
float SpecialMath::median (const int length, float * data)
{
	if(length <= 0)
		throw IllegalSizeException();
	float * copy = new(std::nothrow) float[length];
	if(copy == NULL)
		throw OutOfMemoryException();
	memcpy(copy,data,length * sizeof(float));
	QuickSortMinToMax(copy, 0, length - 1);
	int k = length >> 1;
	float ans = 0;
	if((k << 1) < length)
		ans = copy[k];
	else 
		ans = (copy[k] + copy[k-1])/2.0f;
	delete[] copy;
	copy = NULL;
	return(ans);
}

float SpecialMath::percentile (const int length, float * data, float p) {
	if(length <= 0)
		throw IllegalSizeException();

	float * copy = new(std::nothrow) float[length];
	if(copy == NULL)
		throw OutOfMemoryException();
	memcpy(copy,data,length * sizeof(float));
	QuickSortMinToMax(copy, 0, length - 1);
	if (p >= 1.0f)
		return copy[length - 1];
	if( p <= 0.0f)
		return copy[0];	
	float ans = copy[(int)floor(length * p)] * (1 - p) + copy[(int)ceil(length * p)] * p;
	delete[] copy;
	copy = NULL;
	return(ans);
}