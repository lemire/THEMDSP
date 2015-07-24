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


#if !defined(_CHANNELSET_)
#define _CHANNELSET_


#include "Signal.h"

// used to encapsulate the channels
//
// For simplicity, this implementation of ChannelSet
// uses a very simple data model. 
//
//
class ChannelSet  
{
	public:

		/**
		* Channel Definition defines
		* channels simply with a series of
		* integer whose sum should be 
		* the FrameLength, each integer is
		* the width of the corresponding channel
		*/
		ChannelSet(const int NumberOfChannels, int * ChannelDefinitions, const bool AlternateSigns);
		virtual ~ChannelSet();
		ChannelSet (ChannelSet& cs);
		// get a given channel
		float * getChannel(const int);
		void textWrite(char * );
		void processSignal(Signal&);
		int getChannelLength();
		int getNumberOfChannels();

		friend ChannelSet& operator >> ( Signal& ,  ChannelSet&);
		friend ChannelSet& operator >> ( ChannelSet& ,  char *);

		// if your channel definition doesn't make sense (negative values for example)
		class IllegalChannelDefinitionException{};
		// if you set an impossible number of channels
		class IllegalNumberOfChannelsException{};
		// if you request a channel that doesn't exist
		class NoSuchChannelException{};
		static float mean(int length, short * data);
	private:

		int mNumberOfUsefulChannels;
		int mNumberOfChannels;
		int mNumberOfFrames;
		int * mChannelDefinition;
		float * mChannelData;
		bool mAlternateSigns;

};

#endif // !defined(AFX_CHANNELSET_H__798FCCE0_96C4_11D3_A589_00105ADCDFA4__INCLUDED_)
