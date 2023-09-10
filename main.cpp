#include <afxwin.h>
#include <afxext.h>
#include "Resource.h"
#include "string.h"
#include "resource_1.h"


CString T="";
char s[80];

 class CMyDialog:public CDialog
  {
    public:
			CMyDialog(CWnd* pParent=NULL);
            enum {IDD=IDD_DIALOG1};
	        CEdit  edit1;
    protected:
	        virtual void DoDataExchange(CDataExchange* pDX);    

    protected:
    	BOOL OnInitDialog();
    	afx_msg void OnOK();
        afx_msg void OnCancel();
    	DECLARE_MESSAGE_MAP()
  public:
	  afx_msg void OnEnChangeEdit1();
 };

CMyDialog::CMyDialog(CWnd* pParent)
		:CDialog(CMyDialog::IDD, pParent)
  {}
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_EDIT1, edit1);
}

BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	ON_COMMAND(IDOK,OnOK)
	ON_COMMAND(IDCANCEL,OnCancel)
	ON_EN_CHANGE(ID_EDIT1, &CMyDialog::OnEnChangeEdit1)
END_MESSAGE_MAP();

  void CMyDialog::OnOK()
   {
	char str[80];
	T="";
	edit1.GetWindowText(str,80);
	for(int i=0;i<=strlen(str);i++)
		T=T+str[i];
	for(int i=0;i<80;i++)
	CDialog::OnOK();
   }
  void CMyDialog::OnCancel()
   {
      CDialog::OnCancel();
   }

  BOOL CMyDialog::OnInitDialog()
   {
	CDialog::OnInitDialog();
	edit1.SetWindowText(CString("0"));
	return TRUE;
	}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum { IDD = IDD_HELP_BOX};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

class CTextWnd:public CFrameWnd
	{
	public:
	CTextWnd(CWnd *Wnd);
	};
CTextWnd::CTextWnd(CWnd *WndParent)
{
	CRect rect(200,200,500,500);
	Create (NULL,"Текстовий результат",WS_OVERLAPPEDWINDOW,rect,WndParent,NULL);
};
class CGraphWnd:public CFrameWnd
{
	public:
	CGraphWnd(CWnd *Wnd);
};

CGraphWnd::CGraphWnd(CWnd *WndParent)
{
	Create (NULL,"Графічний результат",WS_OVERLAPPEDWINDOW,rectDefault,WndParent,NULL);
};
class COpenWnd:public CFrameWnd
{
public:
	COpenWnd(CWnd *wnd);
};
COpenWnd::COpenWnd(CWnd *wndParent)
{
	CRect rect(300,420,600,620);
	Create (NULL,"Відкрити файл",WS_OVERLAPPEDWINDOW,rect,wndParent,NULL);
}

class CMyFrameWin:public CFrameWnd
{
   public:
	 CMyFrameWin();
	 CMenu PopupMenu;
    protected:
	 CStatusBar  m_wndStatusBar;
	 CToolBar    m_wndToolBar;
	 HICON m_hIcon;
	 afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	 afx_msg void OnOpen();
	 afx_msg void OnSave();
	 afx_msg void OnInput();
	 afx_msg void OnRun();
	 afx_msg void OnResult();
     afx_msg void OnExit();
     afx_msg void OnAbout();
 	 afx_msg void OnHelp();
	 afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
     DECLARE_MESSAGE_MAP();         
  };

BEGIN_MESSAGE_MAP(CMyFrameWin, CFrameWnd)
	 ON_WM_CREATE()
	 ON_COMMAND(IDM_FILE_OPEN,OnOpen)
	 ON_COMMAND(IDM_SAVE_FILE,OnSave)
     ON_COMMAND(IDM_QUIT,OnExit)
	 ON_COMMAND(IDM_RUN,OnInput)
     ON_COMMAND(IDM_CALC,OnRun)
	 ON_COMMAND(IDM_RESULT_DRAW,OnResult)
	 ON_COMMAND(IDM_HELP, OnHelp)
	 	 ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP();

static UINT indicators[] =
{
	ID_SEPARATOR,         
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
CMyFrameWin::CMyFrameWin()
{
	Create(NULL,
		"Burdeyniy Oleh, 1SP-21b",
		WS_OVERLAPPEDWINDOW,
		rectDefault,
		NULL
		MAKEINTRESOURCE(IDR_MAINFRAME)); 
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_APP);
	SetIcon(m_hIcon, true);
	this->LoadAccelTable(MAKEINTRESOURCE(IDR_MAINFRAME));
	PopupMenu.LoadMenu(IDR_MAINFRAME);
	SetMenu(&PopupMenu);
};  

int CMyFrameWin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD 
	| WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS 
		 | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	    !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      
	}

	return 0;
}

void CMyFrameWin::OnOpen()
  {
	  CString Text;
		CFileDialog dlgOpen(TRUE, "txt","", OFN_HIDEREADONLY, "Текстовий файл (*.txt)| *.txt| ALL Files(*.*)|*.*|", this);
		if(dlgOpen.DoModal()==IDOK)
		{
			COpenWnd *OpenWnd=new COpenWnd(this);
			OpenWnd -> ShowWindow(SW_SHOWNORMAL);
			OpenWnd -> UpdateWindow();
			CClientDC dc(OpenWnd);
			dc.SetTextColor(RGB(45,52,52));

			CStdioFile File(dlgOpen.GetPathName(), CFile::modeRead| CFile::typeBinary);
			while(File.ReadString(Text))
			{	
				dc.TextOut(10,0,Text,Text.GetLength());
				if (Text[0]=='X')
				{		
				int N=2;
				while(N!=Text.GetLength())
				{
					T=T+Text[N];
					N++;
				}
				}
			}
			OpenWnd -> UpdateWindow();
			File.Close();
		}
  }
void CMyFrameWin::OnSave()
  {
	CString string="Результати обчислень програми: ";
	  
	CFileDialog DlgSave(FALSE,"txt","",OFN_HIDEREADONLY,
	"Text file (*.txt)|*.txt|All Files(*.*)|*.*|",this);
	string+=s;
	if(DlgSave.DoModal()==IDOK)
	{ 
	CStdioFile File(DlgSave.GetPathName(),
	CFile::modeCreate | CFile::modeWrite);
	File.WriteString(LPCSTR(string));     
	}
  }

 void CMyFrameWin::OnExit()
  {
    SendMessage(WM_CLOSE);
  }

void CMyFrameWin::OnInput()
  {
	CMyDialog dialog(this);
     dialog.DoModal();
  }

void CMyFrameWin::OnRun()
  {
    float res;
	float x = 1.0f;// atof(T);
	HINSTANCE hDll;

	hDll=::LoadLibrary("BurLibrary.dll");
	typedef float(__cdecl* BURFUNCTION)(float); //оголошення типу функи з ліби (шоб не ругалось)
	BURFUNCTION BurFunction;
	BurFunction = (BURFUNCTION)GetProcAddress(hDll, "BurFunction");
	res= BurFunction(x);
	sprintf(s,"%f",res);
	::FreeLibrary(hDll);
	CTextWnd * TextWnd=new CTextWnd(this);
	TextWnd->ShowWindow(SW_SHOWNORMAL);
	TextWnd->UpdateWindow();
	CClientDC myDC(TextWnd);
	myDC.SetTextColor(RGB(13,66,6));
	myDC.TextOut(125,125,s,strlen(s));
  }

void CMyFrameWin::OnResult()
  {
	CGraphWnd * GraphWnd=new CGraphWnd(this);
	GraphWnd->ShowWindow(SW_SHOWNORMAL);
	GraphWnd->UpdateWindow();
	CClientDC DCS(GraphWnd);
	
    float z=atof(T);
	CPen pen; 
	CPoint arr[6];
    arr[0].x=100;
    arr[0].y=220;
    arr[1].x=180;
    arr[1].y=100;
    arr[2].x=260;
    arr[2].y=100;
	arr[3].x=180;
    arr[3].y=220;
    arr[4].x=260;
    arr[4].y=340;
	arr[5].x=180;
    arr[5].y=340;

	CBrush brush,brush2,brush3;  
	
	pen.CreatePen( PS_SOLID, 5, RGB( 0, 0, 0));
	brush.CreateHatchBrush(HS_DIAGCROSS, RGB( 0, 0, 255));  
    brush2.CreateSolidBrush(RGB( 255, 0, 0));
	brush3.CreateSolidBrush(RGB( 0, 255, 0));

	float m=atof(s);
    DCS.SelectObject(&brush);
	DCS.Rectangle(200-m,500-m,700+m,700);  

	DCS.SelectObject(&pen);
    DCS.SelectObject( &brush3);
    if(z>4)
    {
	DCS.Polygon(arr,6);  
     };
    if(z<4)
    {
    arr[0].x=260;
	arr[3].x=340;
	DCS.Polygon(arr,6);  
    };
	for (int i=20;i>0;i--)
	{
	CPoint arr2[10];         
    arr2[0].x=420+i;
    arr2[0].y=260+i;
    arr2[1].x=540+i;
    arr2[1].y=20+i;
    arr2[2].x=620+i;
    arr2[2].y=20+i;
	arr2[3].x=540+i;
    arr2[3].y=180+i;
    arr2[4].x=700+i;
    arr2[4].y=140+i;
	arr2[5].x=760+i;
    arr2[5].y=60+i;
    arr2[6].x=860+i;
    arr2[6].y=60+i;
    arr2[7].x=640+i;
    arr2[7].y=380+i;
	arr2[8].x=560+i;
    arr2[8].y=380+i;
    arr2[9].x=660+i;
    arr2[9].y=200+i;
    DCS.SelectObject(&brush2);
    DCS.Polygon(arr2,10); 
	}
	CString str1="Результати обрахувань програми: ";
	DCS.SetTextColor(RGB(155,100,55));
	DCS.TextOut(350,550,str1);
  	DCS.SetTextColor(RGB(100,155,255));
	DCS.TextOut(450,600,s);

	pen.DeleteObject();
    brush.DeleteObject(); 
	brush2.DeleteObject();
	brush3.DeleteObject();
  }
void CMyFrameWin::OnAbout()
  {
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
  }
void CMyFrameWin::OnHelp()
  {
	::MessageBox(NULL,"text","Help",MB_OK|
	           MB_ICONINFORMATION);
  }
void CMyFrameWin::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint t;
    GetCursorPos(&t);
	PopupMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,t.x, t.y, this);
}
class CMyApp:public CWinApp
{
  public:
	virtual BOOL InitInstance();
};
BOOL CMyApp::InitInstance()
{
	CMyFrameWin *pMainWnd=new CMyFrameWin;
	m_pMainWnd=pMainWnd;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	    return TRUE;
};

  CMyApp app;


  void CMyDialog::OnEnChangeEdit1()
  {
	  // TODO:  If this is a RICHEDIT control, the control will not
	  // send this notification unless you override the CDialog::OnInitDialog()
	  // function and call CRichEditCtrl().SetEventMask()
	  // with the ENM_CHANGE flag ORed into the mask.

	  // TODO:  Add your control notification handler code here
  }
