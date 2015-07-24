#include "THEMFile.h"
#include <iostream>

#include "SIMn.h"
#include "Despike.h"
#include "ChannelSet.h"
#include "WaveletShrinkage.h"
#include "Stacking.h"
#include "RunningAverage.h"

static const int NumberOfDataTypes = 4;
static char* DataType[NumberOfDataTypes] = {"EM1","EM2","EM3","EM4"};
static const int numberofchannels = 27;
static int channeldef[numberofchannels] = {75, 2,2,2,2,2, 2,2,2,2,2, 16,16,16,16, 16,16,16,16, 16,16,16,16, 16,16,16,16};



void usage() {
	std::cout <<"USAGE"<<std::endl;
	std::cout <<"ConsoleExtractor.exe inputdatfile EMi outputbinfile "<<std::endl;
	std::cout <<"	firsthalfcycle lasthalfcycle baselinecorrection downsampling"<<std::endl;
	std::cout <<"	despikecoef tau outputtextfile integrate"<<std::endl;
	std::cout <<"Example:"<<std::endl;
	std::cout <<"ConsoleExtractor.exe d:\bob3.dat EM2 test.bin 1 3001 1 1 100 0 rraw.txt"<<std::endl;
	std::cout <<"ConsoleExtractor.exe d:\bob3.dat EM2 test.bin 1 3001 1 6 100 1 rraw.txt"<<std::endl;
	std::cout <<"ConsoleExtractor.exe d:\bob3.dat EM2 test.bin 1 3001"<<std::endl;
	std::cout << " see documentation for further information "<<std::endl;
}

int main(int arg, char **input) {
	std::cout << "Console extractor (c) Daniel Lemire 2001" << std::endl;
	std::cout << "http://www.ondelette.com" << std::endl;
	if(arg < 2) {
		usage();
		
		return 1;
	}
	char * filename = input[1];
	std::cout << "Input (DAT) File : "<< filename << std::endl;
	std::fstream fsin(filename,std::ios::in | std::ios::binary);
	if (fsin.fail()) {
		fsin.close();
		std::cout << "Unable to open the file properly." << std::endl;
		return 56;
	}
	std::cout << "File has been open and will be in read-only mode. " << std::endl;
	try {
		THEMFile themfile(fsin);
		std:: cout << "Frame size: "<<themfile.getFrameSize() << std::endl;
		
		//themfile.getEMFramesPerBlock();
		//;
		//themfile.getNumberOfDifferentTypes();
		//themfile.getSizeOfBlocks();
		//themfile.getVersion();
		//themfile.getNumberOfEMChannels();
		//themfile.getRepRate();
		

		if(arg < 3) {
			fsin.close();
			std::cout << "You need to specify a type of data (EM1, EM2...)." << std::endl;
			return 1;
		}	
		char * TypeStr = input[2];
		std::cout << "Data type : "<<TypeStr << std::endl;
		int type = -1;
		for (int k = 0; k < NumberOfDataTypes; ++k) {
			if(strcmp(TypeStr,DataType[k]) == 0) {
				type = k;
				break;
			}
		}
		if( type == -1) {
			fsin.close();
			std::cout << "Your type of data isn't recognized." << std::endl;
			return 2;
		}
		if(arg < 4) {
			fsin.close();
			std::cout << "You need to specify the output binary file." << std::endl;
			return 3;
		}
		char * outputfilename = input[3];
		std::cout << "Output file name : " <<outputfilename << std::endl;
		if(arg < 6) {
			fsin.close();
			std::cout << "You specify a first and a last frame." << std::endl;
			return 4;
		}
		const int FirstFrame = atoi(input[4]);
		const int LastFrame = atoi(input[5]);
		bool base = true;
		if(arg >=7) {
			base =(bool) atoi(input[6]);
		}
		if(base) {
			std::cout << "baseline correction activated!" << std::endl;
		} else {
			std::cout << "baseline correction desactivated!" << std::endl;
		}
		int down = 1;
		if(arg >=8) {
			down = atoi(input[7]);
		}
		std::cout << "Downsampling over " << down <<" half-cycles." << std::endl;
		float DespikeCoefficient = 100.0f;
		if(arg >=9) {
			DespikeCoefficient = atof(input[8]);
		}
		std::cout << "Despike coefficient " << DespikeCoefficient << std::endl;
		bool tau = true;
		if(arg >=10) {
			tau = (bool) atoi(input[9]);
		}
		if(tau) {
			std::cout << "Tau projection activated!"<< std::endl;
		} else {
			std::cout << "Tau projection desactivated!"<< std::endl;
		}
		char * textfile = "raw.txt";
		if(arg >= 11) {
			textfile = input[10];
		}
		bool integrate = false;
		if(arg >= 12) {
			integrate = (bool) atoi(input[11]);
		}
		if (integrate) 
			std::cout << "Integrating offtime (B field)" << std::endl;
		else
			std::cout << "No integration (dB/dt)" << std::endl;

		std::cout << "Extracting from " <<FirstFrame << " to " <<LastFrame << std::endl;
		THEMFilter * signal = NULL;
		try {
			std::cout <<"Extracting to binary file... (the data will be processed)"<< std::endl;
			switch (type) {
			case 0 :
				signal = themfile.getEM1(outputfilename,FirstFrame,LastFrame,base,down,DespikeCoefficient,tau, integrate);
				break;
			case 1 :
				signal = themfile.getEM2(outputfilename,FirstFrame,LastFrame,base,down,DespikeCoefficient,tau, integrate);
				break;
			case 2 :
				signal = themfile.getEM3(outputfilename,FirstFrame,LastFrame,base,down,DespikeCoefficient,tau, integrate);
				break;
			case 3 :
				signal = themfile.getEM4(outputfilename,FirstFrame,LastFrame,base,down,DespikeCoefficient,tau, integrate);
				break;
			default :
				signal = themfile.getEM1(outputfilename,FirstFrame,LastFrame,base,down,DespikeCoefficient,tau, integrate);
				break;
			}
			std::cout <<"Extracting to binary file...ok"<< std::endl;
			std::cout <<"Extracting to text file "<<textfile<<"..."<< std::endl;

			int MaxFrames = signal->getNumberOfFrames() > (50 * signal->getDownsamplingSize() )? (50 * signal->getDownsamplingSize() ) : signal->getNumberOfFrames() - 1;
			if(MaxFrames < 0 ) MaxFrames = 0;
			const int off = 0;
			std::cout << "Off = " << off << " downsampling = " <<signal->getDownsamplingSize()<< std::endl;
			if(signal->getDownsamplingSize() > 0)
				signal->textWrite(textfile,(0+off)/ signal->getDownsamplingSize(),(MaxFrames+off)/signal->getDownsamplingSize());
			else 
				signal->textWrite(textfile,(0+off),(MaxFrames+off));
			std::cout <<"Extracting to text file "<<textfile<<"... ok"<< std::endl;
			char* channeltextfile = "channels.txt";
			std::cout <<"Writing channel to text file "<<channeltextfile<<"... "<< std::endl;
			bool invertSigns = false;
			if(down <= 1)
				invertSigns = true;
			if(down / 2 * 2 == down )
				invertSigns = false;
			ChannelSet cs(numberofchannels, channeldef, invertSigns);
			(*signal) >>  cs >> RunningAverage(10) >> "channels.txt";
			std::cout <<"Writing channel to text file  "<<channeltextfile<<"... ok"<< std::endl;
			delete signal;
			signal = NULL;
			
		} catch (THEMFile::UnexpectedEOFException) {
			std::cout << "The file ended before all of the requested frames could be recovered." << std::endl;
			fsin.close();
			delete[] signal;
			return 7;
		} /*catch (...) {
			std::cout << "Something went wrong. This software doesn't yet process such errors." << std::endl;
			fsin.close();
			delete[] signal;
			
			return 5;
		}*/
		
		fsin.close();
		delete[] signal;
		return 0;
	} catch (THEMFile::Not16bitsException) {
		std::cout << "Sorry, this software only support 16 bits signals." << std::endl;
	} catch (THEMFile::UnexpectedEOFException) {
		std::cout << "It was not possible to read the header fully!" << std::endl;
	} /*catch (...) {
		std::cout << "Some unknown error occured." << std::endl;
	}*/
	
	fsin.close();
	return 45;
}