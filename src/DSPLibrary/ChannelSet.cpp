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
#include "ChannelSet.h"
#include "SpecialMath.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
// inputs:
//		NumberOfChannels - number of channels
//		ChannelDefinition - the definition of the channels
// throws:
//		IllegalChannelDefinitionException - if the channel definition is incorrect
//////////////////////////////////////////////////////////////////////
ChannelSet::ChannelSet(int NumberOfChannels, int * ChannelDefinition, const bool AlternateSigns)
{
	if((NumberOfChannels <= 0) || (NumberOfChannels > 1024))// you should never need more than 1024 channels, no matter what!
		throw IllegalNumberOfChannelsException();
	mAlternateSigns = AlternateSigns;
	mNumberOfChannels = NumberOfChannels;
	for(int k = 0; k < NumberOfChannels ; k++) {
		if(ChannelDefinition[k] <= 0)
			throw IllegalChannelDefinitionException();
	}
	mChannelDefinition = ChannelDefinition;
	mNumberOfFrames = 0;
	mNumberOfUsefulChannels = 0;
}

ChannelSet::~ChannelSet()
{
	delete[] mChannelData;
	mChannelData = NULL;
}


// get the channels from a signal
void ChannelSet::processSignal (Signal& signal){
	if(signal.getNumberOfFrames() == 0)
		throw Signal::NullSignalException();
	mChannelData = NULL;
	try {
		mChannelData = new float[signal.getNumberOfFrames() * mNumberOfChannels];
		memset(mChannelData,0,signal.getNumberOfFrames() * mNumberOfChannels * sizeof(float));
		mNumberOfUsefulChannels = 0;
		mNumberOfFrames = signal.getNumberOfFrames();
		{int sum = 0;
		while ((sum < signal.getFrameLength())&&(mNumberOfUsefulChannels < mNumberOfChannels)) {
			sum += mChannelDefinition[mNumberOfUsefulChannels++];
		}}
		int sign = 1;
		for(int fr = 0 ; fr < mNumberOfFrames ; fr++) {
			short * currentFrame = signal.getFrame(fr);
			int ChStart = 0;
			for (int ch = 0 ; ch < mNumberOfUsefulChannels ; ch++) {
				assert(ChStart + mChannelDefinition[ch] < signal.getFrameLength());
				mChannelData[ch * mNumberOfFrames + fr] 
					
					= mean(mChannelDefinition[ch],currentFrame + ChStart);
					//= SpecialMath::median(mChannelDefinition[ch],currentFrame + ChStart, false);
				ChStart += mChannelDefinition[ch];
				if(mAlternateSigns && (sign == -1))
					mChannelData[ch * mNumberOfFrames + fr] *= sign;
			}
			if(mAlternateSigns)
				sign = -sign;
		}
	} catch (...) {
		throw;
	}
}

float ChannelSet::mean(int length, short * data) {
	if(length == 0)
		return 0.0f;
	double mean = 0.0;
	for(int k = 0; k < length; ++k ) {
		mean += data[k];
	}
	mean /= length; 
	return (float) mean;
}

float * ChannelSet::getChannel(const int k) {
	if((k < 0) || (k >= mNumberOfChannels))
		throw NoSuchChannelException();
	return(mChannelData + mNumberOfFrames * k);
}




// get the number of channels
int ChannelSet::getNumberOfChannels () {
	return(mNumberOfChannels);
}

// get the length of the channels
int ChannelSet::getChannelLength () {
	return(mNumberOfFrames);
}

// write the channels into a text file
void ChannelSet::textWrite(char * filename) {
	if(mChannelData == 0)
		return;
	std::fstream newfs (filename,std::ios::out  ) ;
	for (int fr = 0 ; fr < mNumberOfFrames ; fr++) {
		for(int ch = 0; ch < mNumberOfChannels ; ch++) {
			float * channel = getChannel(ch);
			newfs << channel[fr];
			if (ch != mNumberOfChannels - 1)
				newfs << " ";
		}
		newfs << "\n";
	}
	if(newfs.fail()) {
		std::cout << "WARNING : Text file "<< filename <<"might be corrupted." << std::endl;
	}
	newfs.close();

}

ChannelSet& operator >> ( Signal& signal,  ChannelSet& cs) {

	cs.processSignal(signal);
	return(cs);
}

ChannelSet& operator >> ( ChannelSet& cs,  char * filename) {
	cs.textWrite(filename);
	return(cs);
}

ChannelSet::ChannelSet (ChannelSet& cs) {
	std::cout << "What are you doing??? Stop that !" <<std::endl;
	std::cout << "This object cannot be passed by value..."<<std::endl;
	std::cout << "Not implemented yet!"<<std::endl;
}
