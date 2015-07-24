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
#include "WaveletShrinkage.h"
#include "VFastSymmlet8.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
* Arguments
*    parameter: level of denoising
*/
WaveletShrinkage::WaveletShrinkage(const float parameter)
{
	if(parameter < 0.0f)
		throw NegativeParameterException();
	mParameter = parameter;
}

WaveletShrinkage::~WaveletShrinkage()
{

}

/**
* process the channel set
* Inputs:
*	cs - the channel set which will be modified
*/
void WaveletShrinkage::processChannelSet(ChannelSet& cs) {
	mChannelLength = cs.getChannelLength();
	if(mChannelLength < 128)
		return;//nothing to do!!! Please don't bother
	for(int k = 0; k < cs.getNumberOfChannels() ;k++) {
		shrink(cs.getChannel(k));
	}
}


/*
* compute the threshold for the shrinkage 
* Arguments:
*	length - length of the array
*	data - actual data to threshold
* Results:
*   the threshold
*/
float WaveletShrinkage::threshold (int length, float * data, float& median, const float p)
{
	float * mediansubtracted = new(std::nothrow) float[length];
	if (mediansubtracted == NULL)
		throw OutOfMemoryException();
	memcpy(mediansubtracted,data,length*sizeof(float));
	median = SpecialMath::median(length, mediansubtracted);
	for (int k = 0; k < length; k++)
	{
		mediansubtracted[k] = (float) fabs(mediansubtracted[k] - median);
		
	}
//	std::cout << " p = " << p <<std::endl;
	float answer = SpecialMath::percentile(length, mediansubtracted,p);
	delete[] mediansubtracted;
	mediansubtracted = NULL;
	return(answer);
}
/*
* Soft thresholding is the best way to go !
* Inputs:
*	length - length of the pointer
*	data - pointer to denoise
*/
void WaveletShrinkage::softThresholding(int length, float * data)
{
	float median = 0.0f;
	const float Threshold =  threshold(length, data, median,mParameter);
	int perc = 0;
	for(int k = 0 ; k < length; ++k)
	{
		if(data[k]  > Threshold) {
			data[k] -= Threshold;
			//data[k] = 0.0f;
		} else if (data[k] < -Threshold) {
			data[k] += Threshold;
			//data[k] = 0.0f;
		} else {
			data[k] = 0.0f;
			perc++;
		}
	}
//	std::cout << " perc / length * 100 = " <<perc / (float)length * 100 << std::endl;
}


/*
* 256 is the recommended value for the parameter
*
*	Recommended range of values for the parameter : 0 (deactivated) to ...
*	Negative values are not a good idea.
*
*	Note : it is not recommended to use this processing
*	on signals shorter than 1024 samples or on signals
* 	having anomalies at the very beginning or the very
*	end of the signal.
*
*	The current scheme is still being tested. There could
*	be bugs.
*
*   Inputs:
*		data - the frame to shrink
*/
void  WaveletShrinkage::shrink(float * data)
{
	int maxiter = 0;
	while ((mChannelLength >> maxiter) > 32) 	
		maxiter++;
	int scalewidth = mChannelLength >> maxiter;
	// following two lines are stupid
	while((scalewidth << maxiter) < mChannelLength)
		scalewidth++;
	int buffersize = scalewidth << maxiter;
	float * buffer = new(std::nothrow) float[buffersize];
	if(buffer == NULL)
		throw OutOfMemoryException();
	assert(buffersize >= mChannelLength);
	if( buffersize - mChannelLength > 0)
		memset(buffer + mChannelLength,0,(buffersize - mChannelLength) * sizeof(float) );
	memcpy(buffer,data,mChannelLength * sizeof(float));
	VFastSymmlet8::transform(maxiter, buffersize,buffer);
	int last = buffersize >> 1; 
	while(last >= scalewidth)
	{
		softThresholding(last, buffer + last);
		last >>= 1; 
	}
	VFastSymmlet8::invTransform(maxiter,buffersize,buffer);
	int safetyzone = scalewidth;
	for(int k = safetyzone; k < mChannelLength - safetyzone ; k++)
		data[k] = buffer[k];
	delete[] buffer;
	buffer = NULL;
}


