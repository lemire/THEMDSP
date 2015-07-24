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
#include "VFastSymmlet8.h"


static float * ans;

void VFastSymmlet8::transform( int    length
							, float* v
							, int    last)
{
	int half = last/2;
    for(int k2=0; k2 < half - 3; k2++) {
          ans[k2+half] = v[((k2<<1)+0)]*wavelet[0]+v[((k2<<1)+1)]*wavelet[1]+v[((k2<<1)+2)]*wavelet[2]+v[((k2<<1)+3)]*wavelet[3]+v[((k2<<1)+4)]*wavelet[4]+v[((k2<<1)+5)]*wavelet[5]+v[((k2<<1)+6)]*wavelet[6]+v[((k2<<1)+7)]*wavelet[7];
          ans[k2] = v[((k2<<1)+0)]*scale[0]+v[((k2<<1)+1)]*scale[1]+v[((k2<<1)+2)]*scale[2]+v[((k2<<1)+3)]*scale[3]+v[((k2<<1)+4)]*scale[4]+v[((k2<<1)+5)]*scale[5]+v[((k2<<1)+6)]*scale[6]+v[((k2<<1)+7)]*scale[7];
	}
    ans[last-3] = v[last-6]*wavelet[0]+v[last-5]*wavelet[1]+v[last-4]*wavelet[2]+v[last-3]*wavelet[3]+v[last-2]*wavelet[4]+v[last-1]*wavelet[5]+v[0]*wavelet[6]+v[1]*wavelet[7];
    ans[half-3] = v[last-6]*scale[0]+v[last-5]*scale[1]+v[last-4]*scale[2]+v[last-3]*scale[3]+v[last-2]*scale[4]+v[last-1]*scale[5]+v[0]*scale[6]+v[1]*scale[7];
    ans[last-2] = v[last-4]*wavelet[0]+v[last-3]*wavelet[1]+v[last-2]*wavelet[2]+v[last-1]*wavelet[3]+v[0]*wavelet[4]+v[1]*wavelet[5]+v[2]*wavelet[6]+v[3]*wavelet[7];
    ans[half-2] = v[last-4]*scale[0]+v[last-3]*scale[1]+v[last-2]*scale[2]+v[last-1]*scale[3]+v[0]*scale[4]+v[1]*scale[5]+v[2]*scale[6]+v[3]*scale[7];
    ans[last-1] = v[last-2]*wavelet[0]+v[last-1]*wavelet[1]+v[0]*wavelet[2]+v[1]*wavelet[3]+v[2]*wavelet[4]+v[3]*wavelet[5]+v[4]*wavelet[6]+v[5]*wavelet[7];
    ans[half-1] = v[last-2]*scale[0]+v[last-1]*scale[1]+v[0]*scale[2]+v[1]*scale[3]+v[2]*scale[4]+v[3]*scale[5]+v[4]*scale[6]+v[5]*scale[7];
	memcpy(v,ans,last*sizeof(float));
}

void VFastSymmlet8::transform (const int maxiter,const int length,float * v) {
	if (length < 8) {
		throw SignalSmallerThan8Exception();
	}
	if ((length >> maxiter) < 8) {
		throw SignalTooSmallException();
	}
	int last=length;
	ans = new(std::nothrow) float[last];
	if (ans == NULL)
		throw OutOfMemoryException();
    for (int iter=0;iter<maxiter;iter++) {
      	transform(length,v,last);
		last/=2;
    }
	delete(ans);
	ans = NULL;

}

void VFastSymmlet8::invTransform(int length, float * v, int last) {
	int ResultingLength=2*last;
	memset(ans,0,ResultingLength * sizeof(float));
    for(int k2=0;(k2<<1)+7<ResultingLength;k2++) {
          ans[((k2<<1)+7)]+=scale[7]*v[k2]+wavelet[7]*v[k2+last] ;
          ans[((k2<<1)+6)]+=scale[6]*v[k2]+wavelet[6]*v[k2+last] ;
          ans[((k2<<1)+5)]+=scale[5]*v[k2]+wavelet[5]*v[k2+last] ;
          ans[((k2<<1)+4)]+=scale[4]*v[k2]+wavelet[4]*v[k2+last] ;
          ans[((k2<<1)+3)]+=scale[3]*v[k2]+wavelet[3]*v[k2+last] ;
          ans[((k2<<1)+2)]+=scale[2]*v[k2]+wavelet[2]*v[k2+last] ;
          ans[((k2<<1)+1)]+=scale[1]*v[k2]+wavelet[1]*v[k2+last] ;
          ans[((k2<<1)+0)]+=scale[0]*v[k2]+wavelet[0]*v[k2+last] ;
    }
    ans[ResultingLength-6]+=scale[0]*v[last-3]+wavelet[0]*v[ResultingLength-3] ;
    ans[ResultingLength-5]+=scale[1]*v[last-3]+wavelet[1]*v[ResultingLength-3] ;
    ans[ResultingLength-4]+=scale[2]*v[last-3]+wavelet[2]*v[ResultingLength-3] ;
    ans[ResultingLength-3]+=scale[3]*v[last-3]+wavelet[3]*v[ResultingLength-3] ;
    ans[ResultingLength-2]+=scale[4]*v[last-3]+wavelet[4]*v[ResultingLength-3] ;
    ans[ResultingLength-1]+=scale[5]*v[last-3]+wavelet[5]*v[ResultingLength-3] ;
    ans[0]+=scale[6]*v[last-3]+wavelet[6]*v[ResultingLength-3] ;
    ans[1]+=scale[7]*v[last-3]+wavelet[7]*v[ResultingLength-3] ;
    ans[ResultingLength-4]+=scale[0]*v[last-2]+wavelet[0]*v[ResultingLength-2] ;
    ans[ResultingLength-3]+=scale[1]*v[last-2]+wavelet[1]*v[ResultingLength-2] ;
    ans[ResultingLength-2]+=scale[2]*v[last-2]+wavelet[2]*v[ResultingLength-2] ;
    ans[ResultingLength-1]+=scale[3]*v[last-2]+wavelet[3]*v[ResultingLength-2] ;
    ans[0]+=scale[4]*v[last-2]+wavelet[4]*v[ResultingLength-2] ;
    ans[1]+=scale[5]*v[last-2]+wavelet[5]*v[ResultingLength-2] ;
    ans[2]+=scale[6]*v[last-2]+wavelet[6]*v[ResultingLength-2] ;
    ans[3]+=scale[7]*v[last-2]+wavelet[7]*v[ResultingLength-2] ;
    ans[ResultingLength-2]+=scale[0]*v[last-1]+wavelet[0]*v[ResultingLength-1] ;
    ans[ResultingLength-1]+=scale[1]*v[last-1]+wavelet[1]*v[ResultingLength-1] ;
    ans[0]+=scale[2]*v[last-1]+wavelet[2]*v[ResultingLength-1] ;
    ans[1]+=scale[3]*v[last-1]+wavelet[3]*v[ResultingLength-1] ;
    ans[2]+=scale[4]*v[last-1]+wavelet[4]*v[ResultingLength-1] ;
    ans[3]+=scale[5]*v[last-1]+wavelet[5]*v[ResultingLength-1] ;
    ans[4]+=scale[6]*v[last-1]+wavelet[6]*v[ResultingLength-1] ;
    ans[5]+=scale[7]*v[last-1]+wavelet[7]*v[ResultingLength-1] ;
	memcpy(v,ans,ResultingLength * sizeof(float));
}

void VFastSymmlet8::invTransform (int itermax,int length,float * v) {
	
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
	ans = NULL;
}

void VFastSymmlet8::expand(int length, float * v) {
	int last=length/2;
	int ResultingLength=2*last;
    ans = new(std::nothrow) float[ResultingLength];
	if(ans == NULL)
		throw OutOfMemoryException();
	memset(ans,0,ResultingLength * sizeof(float));
    for(int k2=0; (k2<<1)+7<ResultingLength; k2++) {
          ans[((k2<<1)+7)]+=scale[7]*v[k2];
          ans[((k2<<1)+6)]+=scale[6]*v[k2];
          ans[((k2<<1)+5)]+=scale[5]*v[k2];
          ans[((k2<<1)+4)]+=scale[4]*v[k2];
          ans[((k2<<1)+3)]+=scale[3]*v[k2];
          ans[((k2<<1)+2)]+=scale[2]*v[k2];
          ans[((k2<<1)+1)]+=scale[1]*v[k2];
          ans[((k2<<1)+0)]+=scale[0]*v[k2];
    }
    ans[ResultingLength-6]+=scale[0]*v[last-3];
    ans[ResultingLength-5]+=scale[1]*v[last-3];
    ans[ResultingLength-4]+=scale[2]*v[last-3];
    ans[ResultingLength-3]+=scale[3]*v[last-3];
    ans[ResultingLength-2]+=scale[4]*v[last-3];
    ans[ResultingLength-1]+=scale[5]*v[last-3];
    ans[0]+=scale[6]*v[last-3];
    ans[1]+=scale[7]*v[last-3];
    ans[ResultingLength-4]+=scale[0]*v[last-2];
    ans[ResultingLength-3]+=scale[1]*v[last-2];
    ans[ResultingLength-2]+=scale[2]*v[last-2];
    ans[ResultingLength-1]+=scale[3]*v[last-2];
    ans[0]+=scale[4]*v[last-2];
    ans[1]+=scale[5]*v[last-2];
    ans[2]+=scale[6]*v[last-2];
    ans[3]+=scale[7]*v[last-2];
    ans[ResultingLength-2]+=scale[0]*v[last-1];
    ans[ResultingLength-1]+=scale[1]*v[last-1];
    ans[0]+=scale[2]*v[last-1];
    ans[1]+=scale[3]*v[last-1];
    ans[2]+=scale[4]*v[last-1];
    ans[3]+=scale[5]*v[last-1];
    ans[4]+=scale[6]*v[last-1];
    ans[5]+=scale[7]*v[last-1];
	memcpy(v,ans,ResultingLength*sizeof(float));
	delete[] ans;
	ans = NULL;
}


