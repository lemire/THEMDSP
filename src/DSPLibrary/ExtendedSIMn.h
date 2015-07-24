#if !defined(_EXTENDEDSIMN_H_)
#define _EXTENDEDSIMN_H_

#include "PrecompHeaders.h"
#include "SIMn.h"

/*
* This is a corrected version of the SIMn algorithm.
* Essentially, it relies on the SIMn algorithm to first
* approximate the baseline and then "corrects" it. It was
* found through numerical experiments that SIMn may be 
* unaccurate near the on-time. Therefore, Lagrange interpolation
* is now used to correct SIMn. (Note this isn't unlike what
* Robin was doing at first, but instead of working on the raw 
* signal, we work on the SIMn corrected signal. Also, we
* use the fact the lateFrameWidth parameter to choose how to
* do the Lagrange interpolation.
*
* This class doesn't work too well right now on real signals
* but it does work really well on simulated signals.
*/
class ExtendedSIMn : public SIMn  
{

public:
	ExtendedSIMn(const int order, const int lateFrameWidth, const int LagrangeOrder = 3);

	virtual ~ExtendedSIMn(); 

	inline int getLagrangeOrder() {
		return mLagrangeOrder;
	}
	

	/*
	* This amounts to resetting.
	* should be called whenever you start processing
	* a new signal. Please not that this object tracks
	* the last few "frames" processed. This essentially
	* erase the memory of the processing object. You should
	* use this carefully and you should have more than one
	* processing object per program if you intend on processing
	* numerous signals simultaneously.
	*/
	virtual void processFirstFrame(short * firstframe, const int framelength);
	
	virtual void processNextFrame(short * currentframe);

protected:
	virtual double getLagrangeY(const int x) ;

private:
	int mExtDelay;
	int mLagrangePointer;	
	int mLagrangeOrder;
	double * mLagrangePoints;

};

#endif // !defined(_EXTENDEDSIMN_H_)
