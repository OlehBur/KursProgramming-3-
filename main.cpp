#include <afxwin.h>
#include <afxext.h>
#include "Resource.h"
#include "string.h"
#include "resource_1.h"
#include <cmath>
#include <cstring>

#define M_PI       3.14159265358979323846   // pi


const int WND_FRAME_BORDER_HEIGHT = 60;
const int COUNT_SYMBOLS_OF_RESULT = 80;
CString EDIT_CONTROL_VALUE = "";
char functionResult_string[COUNT_SYMBOLS_OF_RESULT];



class CMyDialog:public CDialog {
    public:
			CMyDialog(CWnd* pParent=NULL);
            enum {IDD=IDD_DIALOG1};
	        CEdit  Edit_Controll;
    protected:
	        virtual void DoDataExchange(CDataExchange* pDX);    

    protected:
    	BOOL OnInitDialog();
    	afx_msg void OnOK();
        afx_msg void OnCancel();
    	DECLARE_MESSAGE_MAP()
};

CMyDialog::CMyDialog(CWnd* pParent)
	:CDialog(CMyDialog::IDD, pParent) {};

void CMyDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_EDIT1, Edit_Controll);
};

BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	ON_COMMAND(IDOK, OnOK)
	ON_COMMAND(IDCANCEL, OnCancel)
END_MESSAGE_MAP();

void CMyDialog::OnOK() {
	char editControll_CurrentValue[COUNT_SYMBOLS_OF_RESULT];
	EDIT_CONTROL_VALUE = "";
	Edit_Controll.GetWindowText(editControll_CurrentValue, COUNT_SYMBOLS_OF_RESULT);

	for (int i = 0; i <= strlen(editControll_CurrentValue); i++)
		EDIT_CONTROL_VALUE = EDIT_CONTROL_VALUE + editControll_CurrentValue[i];

	for (int i = 0; i < COUNT_SYMBOLS_OF_RESULT; i++)
		CDialog::OnOK();
}

void CMyDialog::OnCancel() {
	CDialog::OnCancel();
};

BOOL CMyDialog::OnInitDialog() {
	CDialog::OnInitDialog();
	//Edit_Controll.SetWindowText(CString("0"));
	return TRUE;
};



class CAboutDlg : public CDialog {
public:
	CAboutDlg();
	enum { IDD = IDD_HELP_BOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {};

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()



class NumberResult_Wnd :public CFrameWnd {
public:
	NumberResult_Wnd(CWnd* Wnd);
};

NumberResult_Wnd::NumberResult_Wnd(CWnd* WndParent) {
	CRect rect(200, 200, 900, 900);
	Create(NULL, "The numerical functionResultult of the function", WS_OVERLAPPEDWINDOW, rect, WndParent, NULL);
};



class CGraphWnd:public CFrameWnd {
	public:
	CGraphWnd(CWnd *Wnd);
};

CGraphWnd::CGraphWnd(CWnd *WndParent) {
	Create(NULL, "Graphical functionResultult of the function", WS_OVERLAPPEDWINDOW, rectDefault, WndParent, NULL);
};



class COpenWnd :public CFrameWnd {
public:
	COpenWnd(CWnd* wnd);
};

COpenWnd::COpenWnd(CWnd* wndParent) {
	CRect rect(300, 420, 600, 620);
	Create(NULL, "Open File", WS_OVERLAPPEDWINDOW, rect, wndParent, NULL);
};



class MainWnd :public CFrameWnd {
	float functionResult{ NULL };
	int drawAnimCoeff{ 0 };

public:
	CMenu MainMenu;
	CMenu PopupMenu;
	MainWnd();

protected:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	HICON m_hIcon;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDraw();
	afx_msg void OnPaint();
	afx_msg void OnOpen();
	afx_msg void OnSave();
	afx_msg void Open_Input_Dialog();
	afx_msg void Get_Function_Results();
	afx_msg void OnExit();
	afx_msg void OnHelp();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP();

private:
	inline double FunctionForDrawing(const int& x, const int& maxDiapY);
	void DrawCoordinatePlane(CPaintDC* dc, const CRect& rect, const CPoint& centerPos, const CPoint& markDistance);
	void DrawFunctionGraph(CPaintDC* dc, const CPoint& centerPos);

};

BEGIN_MESSAGE_MAP(MainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()

	ON_COMMAND(IDM_FILE_OPEN, OnOpen)
	ON_COMMAND(IDM_SAVE_FILE, OnSave)
	ON_COMMAND(IDM_QUIT, OnExit)
	ON_COMMAND(IDM_RUN, Open_Input_Dialog)
	ON_COMMAND(IDM_CALC, Get_Function_Results)
	ON_COMMAND(IDM_RESULT_DRAW, OnDraw)
	ON_COMMAND(IDM_HELP, OnHelp)

	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP();

static UINT indicators[] = {
	ID_SEPARATOR,         
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

MainWnd::MainWnd() {
	Create(NULL,
		"Burdeyniy Oleh, 1SP-21b",
		WS_OVERLAPPEDWINDOW,
		rectDefault,
		NULL
		MAKEINTRESOURCE(IDR_MAINFRAME));


	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_APP);
	SetIcon(m_hIcon, true);
	this->LoadAccelTable(MAKEINTRESOURCE(IDR_MAINFRAME));

	MainMenu.LoadMenu(IDR_MAINFRAME);//завантаження основного меню
	SetMenu(&MainMenu);//прив'язка основного меню

	PopupMenu.LoadMenu(IDR_MENUPOPUP); //прив'язка випадаючого меню
};

int MainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
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
			sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;
	}

	return 0;
}

void MainWnd::OnOpen() {
	CString currentFileString;
	CFileDialog dlgOpen(TRUE, "txt", "", OFN_HIDEREADONLY, "Text File (*.txt)| *.txt| ALL Files(*.*)|*.*|", this);
	if (dlgOpen.DoModal() == IDOK)
	{
		COpenWnd* OpenWnd = new COpenWnd(this);
		OpenWnd->ShowWindow(SW_SHOWNORMAL);
		OpenWnd->UpdateWindow();
		CClientDC dc(OpenWnd);
		dc.SetTextColor(RGB(45, 52, 52));

		CStdioFile File(dlgOpen.GetPathName(), CFile::modeRead | CFile::typeBinary);
		while (File.ReadString(currentFileString))
		{
			dc.TextOut(10, 0, currentFileString, currentFileString.GetLength());
			if (currentFileString[0] == 'X')
			{
				int minStrCounts = 2;
				while (minStrCounts != currentFileString.GetLength())
				{
					EDIT_CONTROL_VALUE = EDIT_CONTROL_VALUE + currentFileString[minStrCounts];
					minStrCounts++;
				}
			}
		}
		OpenWnd->UpdateWindow();
		File.Close();
	}
};

void MainWnd::OnSave() {
	CString string = "Results of program calculations: ";

	CFileDialog DlgSave(FALSE, "txt", "", OFN_HIDEREADONLY,
		"Text File (*.txt)|*.txt|All Files(*.*)|*.*|", this);
	string += functionResult_string;

	if (DlgSave.DoModal() == IDOK)
	{
		CStdioFile File(DlgSave.GetPathName(),
			CFile::modeCreate | CFile::modeWrite);
		File.WriteString(LPCSTR(string));
	}
};

void MainWnd::OnExit() {
	SendMessage(WM_CLOSE);
};

void MainWnd::Open_Input_Dialog() {
	CMyDialog dialog(this);
	dialog.DoModal();
	Get_Function_Results();
};

void MainWnd::Get_Function_Results() {
	float xValue =  atof(EDIT_CONTROL_VALUE);
	HINSTANCE hDll;

	hDll = ::LoadLibrary("BurLibrary.dll");
	typedef float(__cdecl* BURFUNCTION)(float); //оголошення типу функи з ліби (шоб не ругалось)
	BURFUNCTION BurFunction;
	BurFunction = (BURFUNCTION)GetProcAddress(hDll, "BurFunction");
	functionResult = BurFunction(xValue);

	if (isnan(functionResult)) {
		strncpy(functionResult_string, "No solutions", sizeof(functionResult_string));
		functionResult_string[sizeof(functionResult_string) - 1] = '\0'; //закидуємо символ кінця рядка вручну, бо strncpy цього не гарантує
	}
	else {
		sprintf(functionResult_string, "%f", functionResult);
	}


	::FreeLibrary(hDll);
	
	NumberResult_Wnd* NumberResult_Wnd_Ptr = new NumberResult_Wnd(this);
	NumberResult_Wnd_Ptr->ShowWindow(SW_SHOWNORMAL);
	NumberResult_Wnd_Ptr->UpdateWindow();

	CClientDC myDC(NumberResult_Wnd_Ptr);
	myDC.SetTextColor(RGB(13, 66, 6));
	myDC.TextOut(125, 25, functionResult_string, strlen(functionResult_string));

	//OnDraw();
};

void MainWnd::OnHelp() {
	::MessageBox(NULL, "To start work, open the menu \"Dialog>Run\", then enter the value X in the window field and confirm by pfunctionResultsing the OK key", "Help", MB_OK |
		MB_ICONINFORMATION);
};

void MainWnd::OnRButtonDown(UINT nFlags, CPoint point) {
	CPoint cursorCliclPos;

	GetCursorPos(&cursorCliclPos);
	PopupMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
		cursorCliclPos.x, cursorCliclPos.y,
		this); //спав меню на кордах миші
};

void MainWnd::OnDraw() {
	CRect wndRect;
	GetClientRect(&wndRect);
	InvalidateRect(&wndRect, true);
}

void MainWnd::OnPaint() {
	CPaintDC dc(this); // об'єкт контексту пристрою для малювання
	CRect wndRect;// розмір клієнтської об-сті вікна

	GetClientRect(&wndRect);
	
	CPoint centerPos = { // центр полотна (корд. площ.)
		wndRect.Width() / 2,
		wndRect.Height() / 2 };
	CPoint markDistance = {// відстань між поділками на площині
		wndRect.Width() / 50,
		wndRect.Height() / 20 };
	

	DrawCoordinatePlane(&dc, wndRect, centerPos, markDistance);

	if (functionResult != NULL) //EDIT_CONTROL_VALUE.GetLength())
		DrawFunctionGraph(&dc, centerPos);

};

inline double MainWnd::FunctionForDrawing(const int& x, const int &maxDiapY) {
	const float frequency = 2.0f * 3.14 / (maxDiapY * 2); // Один період на всю ширину полотна

	return maxDiapY * sin(frequency * x + 0.0f);
};

void MainWnd::DrawCoordinatePlane(CPaintDC* dc, const CRect& rect, const CPoint& centerPos, const CPoint& markDistance) {
	int markSize = centerPos.y / 35;

	CPen cordPlan_Pen(PS_SOLID, min(centerPos.y, centerPos.x) / 150, RGB(0, 0, 0)); //товщина відповідно до розмірів вікна
	CPen* prevPen = dc->SelectObject(&cordPlan_Pen);

	// осі площини
	dc->MoveTo(0, centerPos.y);
	dc->LineTo(centerPos.x * 2, centerPos.y);
	dc->MoveTo(centerPos.x, 0);
	dc->LineTo(centerPos.x, centerPos.y * 2);

	//поділки осей
	for (int i = 0; i < centerPos.x; i += markDistance.x) {// поділки по осі Х
		dc->MoveTo(centerPos.x + i, centerPos.y - markSize);
		dc->LineTo(centerPos.x + i, centerPos.y + markSize);
		dc->MoveTo(centerPos.x - i, centerPos.y - markSize);
		dc->LineTo(centerPos.x - i, centerPos.y + markSize);
	}

	for (int i = 0; i < centerPos.y; i += markDistance.y) {//поділки по осі У
		dc->MoveTo(centerPos.x - markSize, centerPos.y + i);
		dc->LineTo(centerPos.x + markSize, centerPos.y + i);
		dc->MoveTo(centerPos.x - markSize, centerPos.y - i);
		dc->LineTo(centerPos.x + markSize, centerPos.y - i);
	}

	// повернення попереднього пензла
	dc->SelectObject(prevPen);
};

void MainWnd::DrawFunctionGraph(CPaintDC* dc, const CPoint& centerPos) {
	CPen* graph_Pen;
	switch ((int)functionResult) {
	case 12:
		graph_Pen = new CPen(PS_SOLID, min(centerPos.y, centerPos.x) / 100, RGB(10, 250, 10));
		break;
	case 3:
		graph_Pen = new CPen(PS_SOLID, min(centerPos.y, centerPos.x) / 100, RGB(150, 150, 20));
		break;
	case 420:
		graph_Pen = new CPen(PS_SOLID, min(centerPos.y, centerPos.x) / 100, RGB(0, 40, 250));
		break;
	default:
		graph_Pen = new CPen(PS_DASH, 1, RGB(210, 0, 10));
	}

	CPen* prevPen = dc->SelectObject(graph_Pen);
	double amplitude = (centerPos.y - WND_FRAME_BORDER_HEIGHT)
		/ ((isnan(functionResult)) ? 
		1 : min(20, functionResult));
	double frequency = min(15, (isnan(functionResult))?
		1: functionResult);

	for (int x = -centerPos.x; x <= centerPos.x; x++)
	{
		double radian = (double)x / centerPos.x * (M_PI * 2) * frequency;
		int y = (int)(amplitude * sin(radian));

		(x == -centerPos.x) ?
			dc->MoveTo(x + centerPos.x, centerPos.y - y) :
			dc->LineTo(x + centerPos.x, centerPos.y - y);
	}

	dc->SelectObject(prevPen);
};



class CMyApp:public CWinApp {
  public:
	virtual BOOL InitInstance();
};

BOOL CMyApp::InitInstance() {
	MainWnd *pMainWnd=new MainWnd;
	m_pMainWnd=pMainWnd;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	    return TRUE;
};

CMyApp app;
