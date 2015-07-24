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

#include "Downsampling.h"
#include "PrecompHeaders.h"
#include "SpecialMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Downsampling::Downsampling()
{

}

Downsampling::~Downsampling()
{

}

void Downsampling::process(Signal& signal) {
	const int defaultstacking = 6;
	signal.reset();
	int FrameLength = signal.getFrameLength();
	short * buffer = new(std::nothrow) short[defaultstacking * FrameLength];
	if (buffer == NULL)
		throw OutOfMemoryException();

	try {
		int index = 0, stackedframe = 0;
		while(signal.hasNextFrame()) {
			short * currentFrame = signal.nextFrame();
			if(index < defaultstacking - 1) {
				memcpy(buffer + index++ * FrameLength,currentFrame,FrameLength * sizeof(short));
			} else if(index == defaultstacking - 1) {
				memcpy(buffer + index * FrameLength,currentFrame,FrameLength * sizeof(short));
				makeOneFrameOutOfThese(buffer, FrameLength, defaultstacking);
				signal.setFrame(buffer,stackedframe++);
				index = 0;
			} 
		}
		signal.setNumberOfFrames(stackedframe);
	} catch (...) {
		delete[] buffer;
		buffer = NULL;
		throw;

	}
	delete[] buffer;
	buffer = NULL;
}

void Downsampling::convertIntoPPM(short* frame,  const int framelength) {
	//std::cout << "variance..." << std::endl;
	double var = SpecialMath::variance(framelength,frame);
	//std::cout << "variance...ok "<<var << std::endl;
	if(var != 0.0) {
		
		double factor = 1000000/(8*sqrt(var));
		if((factor > -10000) && (factor < 10000)) {
	//	std::cout <<" applying..." << factor << std::endl;
			for(int k = 0; k < framelength; ++k) {
				frame[k] = SpecialMath::roundToShort(frame[k] * factor);
			}
		}
	//	std::cout <<" applying...ok " << factor << std::endl;
	}

}

void Downsampling::makeOneFrameOutOfThese(short * data, const int framelength, const int numberofframes) {
	short * buffer = NULL;
	try {
		buffer = new short[numberofframes];
		// This is unusable if we stick with 16 bits precision!
		/*for(int f = 0; f < numberofframes; ++f) {
			convertIntoPPM(data + framelength * f, framelength);
		}*/
		for(int k = 0; k < framelength; ++k) {
			short sign = 1;
			for(int f = 0; f < 	numberofframes; ++f) {
				buffer[f] = sign * data[f * framelength + k];
				sign = -sign;
			}
			data[k] = SpecialMath::median(numberofframes,buffer,false);
			
			//data[k] = SpecialMath::mean(numberofframes,buffer);
		}
	} catch (std::bad_alloc) {
		std::cout << "Non critical error: could not downsample due to low memory condition. "<<std::endl;
	}
	delete[] buffer;
	buffer = NULL;
}