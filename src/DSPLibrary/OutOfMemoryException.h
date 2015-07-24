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

#if !defined(_OUTOFMEMORYEXCEPTION___INCLUDED_)
#define _OUTOFMEMORYEXCEPTION___INCLUDED_

class OutOfMemoryException  
{
public:
	OutOfMemoryException() throw();
	virtual ~OutOfMemoryException() throw();

};

#endif 
