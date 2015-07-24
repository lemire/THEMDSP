#include "THEMFile.h"
#include <iostream>

#include "ChannelSet.h"
#include "WaveletShrinkage.h"
#include "BinaryFileSignal.h"

static const int numberofchannels = 28;
static int channeldef[numberofchannels] = {77,2,2,2,2,2,4,4,4,4,4,4,8,8,8,8,8,16,16,16,16,16,32,32,32,32,32,32};



int main(int arg, char **input) {
	std::cout << "Channel extractor (c) Daniel Lemire 2001" << std::endl;
	std::cout << "http://www.ondelette.com" << std::endl;
		std::cout << "Channel extractor may not work properly. See ConsoleExtractor." << std::endl;
	if(arg < 2) {
		std::cout << "You need to pass a binary file as an argument." << std::endl;
		return 1;
	}
	BinaryFileSignal * signal = NULL;
	//try {
		std::cout << "Reading raw file..." << std::endl;
		signal = new BinaryFileSignal(512,input[1],true);
		std::cout << "Signal has " << signal->getNumberOfFrames() * 1024 << " bytes ("<<signal->getNumberOfFrames()/1024.0 <<" MB)"<<std::endl;
		std::cout << "Channels..." << std::endl;
		//(*signal) >> ChannelSet(numberofchannels,channeldef, false)  >> WaveletShrinkage(1024.0f)>>"channels.txt";
		(*signal) >> ChannelSet(numberofchannels,channeldef, false)  >>"channels.txt";
		std::cout << "Channels ok" << std::endl;
	/*} catch(...) {
		std::cout << "something went wrong!"<<std::endl;
		delete signal;
		signal = NULL;		
		return 1;
	}*/
	delete signal;
	signal = NULL;
	return 0;
}