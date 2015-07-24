// THEMFile.h: interface for the CTHEMFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THEMFILE_H__F8BDA711_AEAC_11D4_979D_00001B4BD801__INCLUDED_)
#define AFX_THEMFILE_H__F8BDA711_AEAC_11D4_979D_00001B4BD801__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

	enum
	{ 
		MTHD_CREATENEW,
		MTHD_INPLACE,
		MTHD_HEADERONLY
	};

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

    // this is the descriptor for each individual data element
    typedef struct tagOBXelement
    {
        int type;       // type of data element
        int size;       // number of bytes per point
        int nPoints;    // number of points per block
        int offset;
    } OBX_ELEMENT_T;


    // this is the data header
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

class CTHEMFile  
{
public:
	bool SwapChannels(char *path, char *newpath, int ch1, int ch2, int method);
	bool CheckPath(char *path);
	CTHEMFile();
	virtual ~CTHEMFile();

	OBX_DESCR_T desc;
};

#endif // !defined(AFX_THEMFILE_H__F8BDA711_AEAC_11D4_979D_00001B4BD801__INCLUDED_)
