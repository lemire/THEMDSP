// THEMFile.cpp: implementation of the THEMFile class.
//
//////////////////////////////////////////////////////////////////////

#include "THEMFile.h"


#include "BinaryFileSignal.h"

#if !defined(DWORD)
#define DWORD unsigned int
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

THEMFile::THEMFile(std::istream& is)
{
	mIS = & is;
	mIS->seekg(0);
	mIS->read((char *) &mDesc, sizeof(mDesc));
	if(mIS->fail() || mIS->eof() || mIS->gcount() != sizeof(mDesc)) {
		throw UnexpectedEOFException();
	}
	std::cout << " mDesc.size = " << mDesc.size << std::endl;
}

THEMFile::~THEMFile()
{
	mIS = NULL; // just to be sure no idiot delete this!
}

//////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////


THEMFilter * THEMFile::getSignal(const int type,char * filename, const int FirstFrame, const int LastFrame, const bool baselinecorrection, const int downsampling, const float DespikeCoefficient, const bool tau,  const bool integrate ) {
		if( (FirstFrame < 0) || (FirstFrame > LastFrame) )
			throw InvalidFramesException();
		/*
		* Firstly, we look for the element of type "type"
		*/
		int BytesPerBlock = -1;
		int Offset = -1;
		int Index = -1;
		int SizeOfDWORD = sizeof(DWORD);
		for(int i = 0; i < mDesc.nTypes; i++)
		{
			if(mDesc.element[i].type == type && mDesc.element[i].size != 0)
			{
				/*
				* Whatever size and points are... it appears
				* that this particular type will occupy size * nPoints
				* bytes in each block.
				*/
				std::cout << "mDesc.element[i].size = " << mDesc.element[i].size << std::endl;
				std::cout << "mDesc.element[i].nPoints = " << mDesc.element[i].nPoints << std::endl;
				std::cout << "mDesc.element[i].offset = " << mDesc.element[i].offset << std::endl;
				if( mDesc.element[i].size == 2)
					SizeOfDWORD = 1;
				BytesPerBlock = mDesc.element[i].size * mDesc.element[i].nPoints * SizeOfDWORD;
				Offset = mDesc.element[i].offset * SizeOfDWORD;
				Index = i;
				break;
			}
		}
		/*
		* exit if we didn't find a corresponding type in
		* the data file
		*/
		if( Index == -1 )
			return NULL;// error, type not found!
		/*
		* The FrameSize is given
		* by the descriptor (usually 512??)
		*/
		const int FrameSize = mDesc.frameSize;
		/*
		* the number of frames per block is important... and apparently
		* cannot be read directly, so we compute it through BytesPerBlock
		*/
		const int FramesPerBlock = BytesPerBlock / (sizeof(short) * mDesc.frameSize);
		std::cout << "FramesPerBlock = " <<FramesPerBlock << std::endl;

		THEMFilter * signal = NULL;
		char * buffer = NULL;
		int CurrentFrame = 0;
	/*	try {*/
			signal = new THEMFilter(FrameSize,filename,false, baselinecorrection, downsampling,DespikeCoefficient,tau, integrate);
			const int BlocksToSkip = FirstFrame / FramesPerBlock;			
			mIS->seekg(sizeof(mDesc) +  BlocksToSkip * mDesc.size * SizeOfDWORD);
			if( mIS->fail()) {
				throw UnexpectedEOFException();
			}
			// allocate the buffer here
			buffer = new char[mDesc.size * SizeOfDWORD];
			// first time around we don't go into a loop
			mIS->read((char *) buffer, mDesc.size * SizeOfDWORD);
			if((mIS->gcount() != mDesc.size * SizeOfDWORD) || (mIS->fail()))  {
				throw UnexpectedEOFException();
			}
			CurrentFrame = BlocksToSkip * FramesPerBlock;
			const int skip = FirstFrame - BlocksToSkip * FramesPerBlock;
			assert(skip < FramesPerBlock);
			CurrentFrame += skip;

			for (int k = skip; k < FramesPerBlock; k++) {
				short * frame = ((short *) (buffer + Offset))  + k * FrameSize ;
				signal->addNewFrame(frame);
				++CurrentFrame;
			}
			CurrentFrame += FramesPerBlock - skip;
			while( CurrentFrame + FramesPerBlock < LastFrame) {
				mIS->read((char *) buffer, mDesc.size * SizeOfDWORD);
				if((mIS->gcount() != mDesc.size * SizeOfDWORD) || (mIS->fail()))  {
					throw UnexpectedEOFException();
				}
				for (int k = 0; k < FramesPerBlock; k++) {
					short * frame = ((short *) (buffer + Offset)) + k * FrameSize;
					signal->addNewFrame(frame);
					++CurrentFrame;
				}
			}
			if( CurrentFrame < LastFrame) {
				mIS->read((char *) buffer, mDesc.size * SizeOfDWORD);
				if((mIS->gcount() != mDesc.size * SizeOfDWORD) || (mIS->fail()))  {
					throw UnexpectedEOFException();
				}
				assert(LastFrame - CurrentFrame <= FramesPerBlock);
				for (int k = 0; k < LastFrame - CurrentFrame + 1; k++) {
					signal->addNewFrame(((short *) (buffer + Offset)) + k * FrameSize);
					++CurrentFrame;
				}
			}
	/*	} catch (...) {
			delete signal;
			delete[] buffer;
			return(NULL);
		}*/
		delete[] buffer;
		return signal;
}

