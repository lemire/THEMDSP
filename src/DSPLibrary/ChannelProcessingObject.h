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

#if !defined(AFX_CHANNELPROCESSINGOBJECT_H__798FCCE1_96C4_11D3_A589_00105ADCDFA4__INCLUDED_)
#define AFX_CHANNELPROCESSINGOBJECT_H__798FCCE1_96C4_11D3_A589_00105ADCDFA4__INCLUDED_


#include "ChannelSet.h"

// a base class for all objects meant to process channels
class ChannelProcessingObject  
{
	public:
		ChannelProcessingObject();
		virtual ~ChannelProcessingObject();
		virtual void processChannelSet(ChannelSet&);
		friend ChannelSet& operator >> ( ChannelSet&, ChannelProcessingObject&);

};

#endif // !defined(AFX_CHANNELPROCESSINGOBJECT_H__798FCCE1_96C4_11D3_A589_00105ADCDFA4__INCLUDED_)
