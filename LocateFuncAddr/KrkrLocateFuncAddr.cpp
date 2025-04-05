#include <afxwin.h>
#include "../src/detours.h"
#include "tvp.h"
#pragma comment(lib, "../detours.lib")

static HRESULT (__stdcall *RealV2Link)(iTVPFunctionExporter *exporter) = NULL;

HRESULT __stdcall DetourV2Link(iTVPFunctionExporter *exporter) {
	TVPInitImportStub(exporter);
	return RealV2Link(exporter);
}

static FARPROC (__stdcall *RealGetProcAddress)(HMODULE hModule, LPCSTR lpProcName) = GetProcAddress;

FARPROC __stdcall DetourGetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
	FARPROC ptr = RealGetProcAddress(hModule, lpProcName);
	if (strcmp(lpProcName, "V2Link") == 0) {
		RealV2Link = reinterpret_cast<decltype(RealV2Link)>(ptr);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)RealV2Link, DetourV2Link);
		DetourDetach(&(PVOID&)RealGetProcAddress, DetourGetProcAddress);
		DetourTransactionCommit();
	}
	return ptr;
}

class CMyApp : public CWinApp {
	public:
		virtual BOOL InitInstance() override;
		virtual int ExitInstance() override;
};

class CMyWnd : public CFrameWnd {
	CFont m_font;
	CStatic m_static1, m_static2;
	CEdit m_edit;
	CButton m_button;
	public:
		CMyWnd();
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnBtnClicked();
		DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyWnd, CFrameWnd)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(1001, &CMyWnd::OnBtnClicked)
END_MESSAGE_MAP()

BOOL CMyApp::InitInstance() {
	CWinApp::InitInstance();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)RealGetProcAddress, DetourGetProcAddress);
	DetourTransactionCommit();
	m_pMainWnd = new CMyWnd();
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CMyApp::ExitInstance() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)RealV2Link, DetourV2Link);
	DetourTransactionCommit();
	return CWinApp::ExitInstance();
}

CMyWnd::CMyWnd() {
	Create(NULL, "KrkrLocateFuncAddr", WS_OVERLAPPEDWINDOW, CRect(0, 0, 800, 200));
	m_font.CreatePointFont(100, _T("Microsoft Yahei"));
	m_static1.Create(_T("Function Definition:"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(10, 10, 280, 50), this);
	m_static2.Create(_T("Address:"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(10, 80, 760, 120), this);
	m_edit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, CRect(280, 10, 600, 50), this, 1000);
	m_button.Create(_T("Search"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(610, 10, 760, 50), this, 1001);
	m_static1.SetFont(&m_font);
	m_static2.SetFont(&m_font);
	m_edit.SetFont(&m_font);
	m_button.SetFont(&m_font);
}

HBRUSH CMyWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	return CFrameWnd::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CMyWnd::OnBtnClicked() {
	char def[1024];
	m_edit.GetWindowText(def, 1024);
	void *ptr = TVPGetImportFuncPtr(def);
	if (ptr != nullptr) {
		CString text;
		text.Format(_T("Address: %p"), ptr);
		m_static2.SetWindowText(text);
		(m_static2.GetParent())->RedrawWindow();
	}
}

CMyApp app;
