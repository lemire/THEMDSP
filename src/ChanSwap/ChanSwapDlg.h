// ChanSwapDlg.h : header file
//

#if !defined(AFX_CHANSWAPDLG_H__F8BDA708_AEAC_11D4_979D_00001B4BD801__INCLUDED_)
#define AFX_CHANSWAPDLG_H__F8BDA708_AEAC_11D4_979D_00001B4BD801__INCLUDED_

#include "THEMFile.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChanSwapDlg dialog

class CChanSwapDlg : public CDialog
{
// Construction
public:
	void ValidateOptions(void);
	CTHEMFile themFile;
	CChanSwapDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CChanSwapDlg)
	enum { IDD = IDD_CHANSWAP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChanSwapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CChanSwapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelectRaw();
	afx_msg void OnChangeRawFile();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANSWAPDLG_H__F8BDA708_AEAC_11D4_979D_00001B4BD801__INCLUDED_)
