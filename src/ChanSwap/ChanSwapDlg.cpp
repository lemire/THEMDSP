// ChanSwapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChanSwap.h"
#include "ChanSwapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChanSwapDlg dialog

CChanSwapDlg::CChanSwapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChanSwapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChanSwapDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChanSwapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChanSwapDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChanSwapDlg, CDialog)
	//{{AFX_MSG_MAP(CChanSwapDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECT_RAW, OnSelectRaw)
	ON_EN_CHANGE(IDC_RAW_FILE, OnChangeRawFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChanSwapDlg message handlers

BOOL CChanSwapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	SendDlgItemMessage(IDC_HEADERONLY, BM_SETCHECK, BST_CHECKED, 0);
	ValidateOptions();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChanSwapDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChanSwapDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChanSwapDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CChanSwapDlg::OnSelectRaw() 
{
	OPENFILENAME ofn;
	char path[MAX_PATH+4];
	path[0] = path[MAX_PATH] = 0;
	
	FillMemory(&ofn, sizeof(ofn), 0);
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = "T.H.E.M. Raw Data (*.dat)\0*.dat\0All Files (*.*)\0*.*\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = path;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select T.H.E.M. Raw Data File";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER;

	if(::GetOpenFileName(&ofn))
	{
		SetDlgItemText(IDC_RAW_FILE, path);
	}
}

void CChanSwapDlg::ValidateOptions()
{
	GetDlgItem(IDC_IN1)->EnableWindow(themFile.desc.nChan > 1);
	GetDlgItem(IDC_IN2)->EnableWindow(themFile.desc.nChan > 1);
	GetDlgItem(IDC_IN3)->EnableWindow(themFile.desc.nChan > 2);
	GetDlgItem(IDC_IN4)->EnableWindow(themFile.desc.nChan > 3);
	GetDlgItem(IDC_OUT1)->EnableWindow(themFile.desc.nChan > 1);
	GetDlgItem(IDC_OUT2)->EnableWindow(themFile.desc.nChan > 1);
	GetDlgItem(IDC_OUT3)->EnableWindow(themFile.desc.nChan > 2);
	GetDlgItem(IDC_OUT4)->EnableWindow(themFile.desc.nChan > 3);
	SendDlgItemMessage(IDC_IN1, BM_SETCHECK, BST_CHECKED, 0);
	SendDlgItemMessage(IDC_OUT2, BM_SETCHECK, BST_CHECKED, 0);
}

void CChanSwapDlg::OnChangeRawFile() 
{
	char buff[MAX_PATH+4];
	buff[MAX_PATH] = 0;

	GetDlgItemText(IDC_RAW_FILE, buff, MAX_PATH);
	themFile.CheckPath(buff);
	ValidateOptions();
}

void CChanSwapDlg::OnOK() 
{
	int i, method = 0;
	int in = 0, out = 0;
	char buff[MAX_PATH+4];
	char newpath[MAX_PATH+4];
	buff[MAX_PATH] = 0;
	newpath[MAX_PATH] = 0;

	GetDlgItemText(IDC_RAW_FILE, buff, MAX_PATH);
	if(!themFile.CheckPath(buff))
	{
		MessageBox(buff, "Invalid File", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	for(i = 0; i < 4; i++)
	{
		in |= SendDlgItemMessage(IDC_IN1 + i, BM_GETCHECK, 0, 0) == BST_CHECKED ? i : 0;
		out |= SendDlgItemMessage(IDC_OUT1 + i, BM_GETCHECK, 0, 0) == BST_CHECKED ? i : 0;
	}

	if(in == out)
	{
		MessageBox("Switching the channel with itself would be pointless", "Hey Genius", MB_OK | MB_ICONINFORMATION);
		return;
	}

	for(i = 0; i < 3; i++)
	{
		method |= SendDlgItemMessage(IDC_CREATE_NEW + i, BM_GETCHECK, 0, 0) == BST_CHECKED ? i : 0;
	}

	if(method == MTHD_CREATENEW)
	{
		OPENFILENAME sfn;

		newpath[0] = 0;

		FillMemory(&sfn, sizeof(sfn), 0);
		sfn.lStructSize = sizeof(sfn);
		sfn.hwndOwner = GetSafeHwnd();
		sfn.lpstrFilter = "THEM Files (*.dat)\0*.dat\0*All Files (*.*)\0*.*\0\0";
		sfn.lpstrFile = newpath;
		sfn.nMaxFile = MAX_PATH;
		sfn.lpstrTitle = "Save New File As";
		sfn.lpstrDefExt = "dat";
		sfn.Flags = OFN_OVERWRITEPROMPT | OFN_EXPLORER;

		if(!GetSaveFileName(&sfn))
			return;
	}

	if(themFile.SwapChannels(buff, method == MTHD_CREATENEW ? newpath : NULL, in, out, method))
		MessageBox("It appears to have been a success", "Report");

//	CDialog::OnOK();
}
