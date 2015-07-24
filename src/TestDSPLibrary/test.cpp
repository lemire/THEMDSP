///////////////////////////////////////////////////////////////////
//
// (c) 1999 Daniel Lemire, Ph.D. (Daniel.Lemire@Videotron.ca)
// for THEM Geophysics. Please call if you have any problem
// with this code (514) 524-3517.
//
// This code shouldn't not be used by any others
// but THEM Geophysics without my written consent. You cannot
// remove this notice from the code.
//
// 
////////////////////////////////////////////////////////////////////


#include "PrecompHeaders.h"
#include "DiskCachedSignal.h"
#include "SimpleSignal.h"
#include "SignalProcessingObject.h"
#include "ChannelSet.h"
#include "ChannelProcessingObject.h"
#include "Stacking.h"
#include "SIMn.h"
#include "BiSIMn.h"
#include "SignalTest.h"
#include "Despike.h"
#include "WaveletShrinkage.h"
#include "UnicoilFieldEstimator.h"


// a small test of most of the new design
void main(int arg, char **input) {
	int MAXFRAMES = 1500;
	int BEGIN = 0;
	//int numberofchannels = 7;
	//int channeldef[7] = {128,64,64,64,64,64,64};
	int numberofchannels = 21;
	int channeldef[21] = {80,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};

	if(arg < 2) {
		std::cout << "You need to pass a binary file as an argument." << std::endl;
		return;
	}
	std::cout << "loading file" <<std::endl;
	try {
		DiskCachedSignal s(512,input[1],BEGIN,MAXFRAMES + BEGIN);//,(239 + 120) *sizeof(short));
		std::cout << "read signal and "<< s.getNumberOfFrames() << " frames" << std::endl;
		//SignalTest::generateSineSignal(3.0/(512 * 2),100,128,s);
		//SignalTest::generateSineSignal(100.0/(2*512),128,s);
		//SignalTest::polluteSignalWithSpikes(100,s);
		std::cout << "writing text file" <<std::endl;
		int testlength = 16;
		s.textWrite("test1.txt",0,s.getNumberOfFrames() - 1);
		std::cout << "processing " << std::endl;
		s >> Despike(100,408) >>  SIMn(3,48 );// >> Stacking(6);//;// >>  
		std::cout << "writing processed data" << std::endl;
		s.textWrite("test2.txt",0,testlength);
		std::cout << "writing processed channels" <<std::endl;
		s >> ChannelSet(numberofchannels,channeldef) >>  WaveletShrinkage(1000) ;//>> "channels2.txt";
	} catch (Signal::NullSignalException) {
		std::cout << "unable to read signal" << std::endl;
	}
	std::cout << "Thanks for using this software!" <<std::endl;


}