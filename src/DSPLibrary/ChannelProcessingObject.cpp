///////////////////////////////////////////////////////////////////
//
// (c) 1999, 2000 Daniel Lemire, Ph.D. 
// for THEM Geophysics. Please call if you have any problem
// with this code.
//
// Phone (514) 524-3517. Fax (514) 524-0141.
//
// This code shouldn't not be used outside THEM Geophysics
// without my written consent. You cannot
// remove this notice from the code.
//
//	commercial home page :
//		http://www.techelements.com/
//		http://www.ondelette.com/ (soon to come)
//	home page :
//		http://cafe.rapidus.net/danilemi/
//
//	
//	Daniel.Lemire@Videotron.ca
//	lemire@techelements.com
//	lemire@ondelette.com (soon to come)
//
// 
////////////////////////////////////////////////////////////////////

#include "PrecompHeaders.h"
#include "ChannelProcessingObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ChannelProcessingObject::ChannelProcessingObject()
{

}

ChannelProcessingObject::~ChannelProcessingObject()
{

}

// processing the channel set
void ChannelProcessingObject::processChannelSet(ChannelSet&) {
	// do nothing
}

/*
* process the channel using the given channel processing object
*
*	inputs: 
*		cs - the ChannelSet 
*		cpo - the CPO used to process cs
*/
ChannelSet& operator >> (  ChannelSet& cs, ChannelProcessingObject& cpo ) {
	cpo.processChannelSet(cs);
	return(cs);
}