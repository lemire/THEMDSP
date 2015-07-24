#if !defined(_ob4_)
#define _ob4_

    /*
	* Remarks:
	*   you shouldn't change this... it is the maximum number
    *   of type of data we can store.
	*/
    #define MAX_TYPES 20

    enum
    {
        EM1_TYPE = 0,
        EM2_TYPE,
        EM3_TYPE,
        EM4_TYPE,
        MAG_TYPE,
        A2D1_TYPE,
        A2D2_TYPE,
        CNT_TYPE,
        GPS_TYPE,
        LINE_TYPE,
        MARK_TYPE,
        TIME_TYPE
    };

    /*
	* this is the descriptor for each individual data element
	*
	*  Remarks:
	*   Each EM channel will have one of these
	*/
    typedef struct tagOBXelement
    {
        int type;       // type of data element
        int size;       // number of bytes per point
        int nPoints;    // number of points per block
        int offset;
    } OBX_ELEMENT_T;


    /*
	* this is the data header
	*
	*  Remarks:
	*    It should normally contain the main descriptor for the THEM DAT file.
	*/
    typedef struct tagOBXdescr
    {
        int frameSize;      // number of points per half-cycle (EM)
        int nChan;          // number of EM channels actually recorded
        int cfg;            // bits 0-7 reserved for version, 8-15 for rep-rate 16-23 for
                            // number of em frames per block, 24-31 for ???
        int nTypes;         // number of different types of recorded data
        int size;           // size of each block in bytes
        OBX_ELEMENT_T element[MAX_TYPES]; // individual data element descriptors
    } OBX_DESCR_T;
#endif