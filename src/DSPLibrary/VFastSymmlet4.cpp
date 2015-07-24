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
#include "VFastSymmlet4.h"


static float * ans;

void VFastSymmlet4::transform( const int    length
							, float* v
							, const int    last)
{
    
    int half = last/2;
    int DeuxFoisK2;
	for(int k2=0; k2 < half - 1; k2++) {
          ans[k2+half] = v[((DeuxFoisK2 = (k2<<1))+0)]*wavelet4[0]+v[(DeuxFoisK2+1)]*wavelet4[1]+v[(DeuxFoisK2+2)]*wavelet4[2]+v[(DeuxFoisK2+3)]*wavelet4[3];
          ans[k2] = v[(DeuxFoisK2+0)]*scale4[0]+v[(DeuxFoisK2+1)]*scale4[1]+v[(DeuxFoisK2+2)]*scale4[2]+v[(DeuxFoisK2+3)]*scale4[3];
	}
    ans[last-1] = v[last-2]*wavelet4[0]+v[last-1]*wavelet4[1]+v[0]*wavelet4[2]+v[1]*wavelet4[3];
    ans[half-1] = v[last-2]*scale4[0]+v[last-1]*scale4[1]+v[0]*scale4[2]+v[1]*scale4[3];
	memcpy(v,ans,last*sizeof(float));
	
}

void VFastSymmlet4::transform (const int maxiter,const int length,float * v) {
	if (length < 8) {
		throw SignalSmallerThan8Exception();
	}
	if ((length >> maxiter) < 8) {
		throw SignalTooSmallException();
	}
	int last=length;
	ans = new(std::nothrow) float [last];
	if(ans == NULL)
		throw OutOfMemoryException();
    for (int iter=0;iter<maxiter;iter++) {
      	transform(length,v,last);
		last/=2;
    }
	delete(ans);
	ans = NULL;

}

void VFastSymmlet4::invTransform(const int length, float * v, const int last) {
	int ResultingLength = last << 1;
	memset(ans,0,ResultingLength * sizeof(float));
	int DeuxFoisk2;
    for(int k2=0;(DeuxFoisk2=(k2<<1))+3<ResultingLength;k2++) {
          ans[(DeuxFoisk2+3)]+=scale4[3]*v[k2]+wavelet4[3]*v[k2+last] ;
          ans[(DeuxFoisk2+2)]+=scale4[2]*v[k2]+wavelet4[2]*v[k2+last] ;
          ans[(DeuxFoisk2+1)]+=scale4[1]*v[k2]+wavelet4[1]*v[k2+last] ;
          ans[(DeuxFoisk2+0)]+=scale4[0]*v[k2]+wavelet4[0]*v[k2+last] ;
    }
    ans[ResultingLength-2]+=scale4[0]*v[last-1]+wavelet4[0]*v[ResultingLength-1] ;
    ans[ResultingLength-1]+=scale4[1]*v[last-1]+wavelet4[1]*v[ResultingLength-1] ;
    ans[0]+=scale4[2]*v[last-1]+wavelet4[2]*v[ResultingLength-1] ;
    ans[1]+=scale4[3]*v[last-1]+wavelet4[3]*v[ResultingLength-1] ;
	memcpy(v,ans,ResultingLength * sizeof(float));
}

void VFastSymmlet4::invTransform (const int itermax,const int length,float * v) {
	if (length<8) {
		throw SignalSmallerThan8Exception();
	}
	if ((length>>itermax)<8) {
		throw SignalTooSmallException();
	}
    int last=length>>itermax;
	ans = new(std::nothrow) float [length];
	if (ans == NULL)
		throw OutOfMemoryException();
    for (int iter=0;iter<itermax;iter++) {
      invTransform(length, v,last);
	  last*=2;
    }
	delete(ans);
}

// devrait être synchronisé sur invtrans... mais moins important
// faire du copier / coller
void VFastSymmlet4::expand(const int length, float * v) {
	int last = length >> 1;
	int ResultingLength = last << 1;
	memset(ans,0,ResultingLength * sizeof(float));
	int DeuxFoisk2;
    for(int k2=0;(DeuxFoisk2=(k2<<1))+3<ResultingLength;k2++) {
          ans[(DeuxFoisk2+3)]+=scale4[3]*v[k2];
          ans[(DeuxFoisk2+2)]+=scale4[2]*v[k2];
          ans[(DeuxFoisk2+1)]+=scale4[1]*v[k2];
          ans[(DeuxFoisk2+0)]+=scale4[0]*v[k2];
    }
    ans[ResultingLength-2]+=scale4[0]*v[last-1];
    ans[ResultingLength-1]+=scale4[1]*v[last-1];
    ans[0]+=scale4[2]*v[last-1];
    ans[1]+=scale4[3]*v[last-1];
	memcpy(v,ans,ResultingLength * sizeof(float));
}