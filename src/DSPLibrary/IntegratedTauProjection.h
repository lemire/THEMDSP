// IntegratedTauProjection.h: interface for the IntegratedTauProjection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTEGRATEDTAUPROJECTION_H__984045A1_2382_4C3B_89A5_99B8A2836B88__INCLUDED_)
#define AFX_INTEGRATEDTAUPROJECTION_H__984045A1_2382_4C3B_89A5_99B8A2836B88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TauProjection.h"

/*
* Experimental class. Works but further study is
* required!
*/
class IntegratedTauProjection : public TauProjection  
{
public:
	// constructor using default tau values...

	IntegratedTauProjection();


	// constructor specifying tau values
	// (class does not own the pointer!)
	IntegratedTauProjection(double * tau, const int numberoftauvalues);

	virtual void project(short * framedata, const int FrameLength);

	virtual ~IntegratedTauProjection();

};

#endif // !defined(AFX_INTEGRATEDTAUPROJECTION_H__984045A1_2382_4C3B_89A5_99B8A2836B88__INCLUDED_)
