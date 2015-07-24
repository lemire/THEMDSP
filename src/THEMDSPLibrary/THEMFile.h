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

#if !defined(_THEMFILE_)
#define _THEMFILE_

#include <istream>
#include <fstream>

#include "ob4.h"
#include "THEMFilter.h"

/*
* Currently, this class assumes 16 bits encoding. There 
* doesn't seem to appear to be any clean way to check this...
* assuming...
* 
* TODO: Should implement assignment operator and 
* copy constructor.
*/
class THEMFile  
{
public:

	/*
	* Build an object to read DAT files. Notice that
	* the implementation is very much version dependent
	* and since there is no versioning currently implemented
	* (as of January 2001) this object may or may not work.
	*
	* Arguments
	*	is : input stream to a THEM dat file or the equivalent 
	*	(doesn't have to be a file!) However, if it doesn't
	*	support positioning (seekg) bugs may appear.
	*/
	THEMFile(std::istream& is);
	virtual ~THEMFile();

	/*
	* Give the size of the frame (usually 512).
	*
	* Results
	*    size in number of samples
	*/
	int getFrameSize() const {
		return mDesc.frameSize;
	}

	/*
	* Give the number of EM channels available.
	*
	* Result
	*    number of EM channels
	*/
	int getNumberOfEMChannels() const {
		return mDesc.nChan;
	}
     
	/*
	* Give the version number for the THEM format
	*
	* Results
	*    version number
	*/
	int getVersion() const {
		return (mDesc.cfg & 255);
	}

	/*
	* Give the rep rate whatever this means
	*
	* Results
	*     Rep rate
	*/
	int getRepRate() const {
		return ((mDesc.cfg >> 8) & 255);
	}

	/*
	* Give the number of EM frames in each block.
	*
	* Results
	*   the number of EM frames
	*/
	int getEMFramesPerBlock() const {
		return ((mDesc.cfg >> 16) & 255);
	}
     
	/*
	* Give the number of different types available in the
	* the file
	*
	* Results
	*      Number of types
	*/
	int getNumberOfDifferentTypes() const {
		return mDesc.nTypes;
	}

	/*
	* Give the size of each block
	* 
	* Results
	*    size in bytes
	*/
	int getSizeOfBlocks() const {
		return mDesc.size;
	}


	/*
	* Get the EM1 data and put it in a binary file
	* 
	* Arguments
	*	filename : name of the binary file to be used
	*   FirstFrame : first frame to extract
	*   LastFrame : last frame to extract
	*	baselinecorrection : should we apply the baseline correction?
	*	downsampling : number of frames to stack
	*	DespikeCoefficient : a value indicating the level of despiking (100 is recommended)
	*
	*
	* Results
	*	A Signal object built on top the filename and
	*	containing the proper data (actually
	*	of class BinaryFileSignal)
	*/
	THEMFilter * getEM1(char * filename, const int FirstFrame, const int LastFrame, const bool baselinecorrection = true, const int downsampling = 1, const float DespikeCoefficient = 100.0f, const bool tau = true,  const bool integrate = false) {return(getSignal(EM1_TYPE, filename, FirstFrame, LastFrame, baselinecorrection, downsampling, DespikeCoefficient, tau,  integrate));}

	/*
	* Get the EM2 data and put it in a binary file
	* 
	* Arguments
	*	filename : name of the binary file to be used
	*   FirstFrame : first frame to extract
	*   LastFrame : last frame to extract
	*	baselinecorrection : should we apply the baseline correction?
	*	downsampling : number of frames to stack
	*	DespikeCoefficient : a value indicating the level of despiking (100 is recommended)
	*
	* Results
	*	A Signal object built on top the filename and
	*	containing the proper data (actually
	*	of class BinaryFileSignal)
	*/
	THEMFilter * getEM2(char * filename, const int FirstFrame, const int LastFrame, const bool baselinecorrection = true, const int downsampling = 1, const float DespikeCoefficient = 100.0f, const bool tau = true,  const bool integrate = false) {return(getSignal(EM2_TYPE, filename, FirstFrame, LastFrame, baselinecorrection, downsampling, DespikeCoefficient, tau, integrate));}

	/*
	* Get the EM3 data and put it in a binary file
	* 
	* Arguments
	*	filename : name of the binary file to be used
	*   FirstFrame : first frame to extract
	*   LastFrame : last frame to extract
	*	baselinecorrection : should we apply the baseline correction?
	*	downsampling : number of frames to stack
	*	DespikeCoefficient : a value indicating the level of despiking (100 is recommended)
	*
	* Results
	*	A Signal object built on top the filename and
	*	containing the proper data (actually
	*	of class BinaryFileSignal)
	*/
	THEMFilter * getEM3(char * filename, const int FirstFrame, const int LastFrame, const bool baselinecorrection = true, const int downsampling = 1, const float DespikeCoefficient = 100.0f, const bool tau = true,  const bool integrate = false) {return(getSignal(EM3_TYPE, filename, FirstFrame, LastFrame, baselinecorrection, downsampling, DespikeCoefficient, tau, integrate ));}

	/*
	* Get the EM4 data and put it in a binary file
	* 
	* Arguments
	*	filename : name of the binary file to be used
	*   FirstFrame : first frame to extract
	*   LastFrame : last frame to extract
	*	baselinecorrection : should we apply the baseline correction?
	*	downsampling : number of frames to stack
	*	DespikeCoefficient : a value indicating the level of despiking (100 is recommended)
	*
	* Results
	*	A Signal object built on top the filename and
	*	containing the proper data (actually
	*	of class BinaryFileSignal)
	*/
	THEMFilter * getEM4(char * filename, const int FirstFrame, const int LastFrame, const bool baselinecorrection = true, const int downsampling = 1, const float DespikeCoefficient = 100.0f, const bool tau = true,  const bool integrate = false) {return(getSignal(EM4_TYPE, filename, FirstFrame, LastFrame, baselinecorrection, downsampling, DespikeCoefficient, tau, integrate));}


	// if the file format doesn't use 16 bits
	class Not16bitsException{};
	// if we could not read the file fully
	class UnexpectedEOFException{};
	// if the requested frames are not available
	class InvalidFramesException{};

private:
	/*
	*
	* Arguments
	*	type : type of the signal to recover (EM1,...)
	*	filename : output file
	*	FirstFrame : first frame to extract
	*	LastFrame : last frame to extract
	*	baselinecorrection : should we apply the baseline correction?
	*	downsampling : number of frames to stack
	*	DespikeCoefficient : a value indicating the level of despiking (100 is recommended)
	*
	* Results
	*	NULL whenever the call failed
	*/
	THEMFilter * getSignal(const int type, char* filename, const int FirstFrame, const int LastFrame, const bool baselinecorrection, const int downsampling, const float DespikeCoefficient, const bool tau, const bool integrate);

	OBX_DESCR_T mDesc;
	std::istream * mIS;

};

#endif // !defined(_THEMFILE_)
