
// RemoteControlServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RemoteControlServer.h"
#include "RemoteControlServerDlg.h"
#include "afxdialogex.h"
#include "CListenSocket.h"
#include "CClientSocket.h"
#include "CShowDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRemoteControlServerDlg 对话框



CRemoteControlServerDlg::CRemoteControlServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REMOTECONTROLSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_listenSocket = nullptr;
}

void CRemoteControlServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
}

BEGIN_MESSAGE_MAP(CRemoteControlServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CRemoteControlServerDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CRemoteControlServerDlg::OnBnClickedBtnClose)
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CRemoteControlServerDlg::OnDblclkList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, &CRemoteControlServerDlg::OnRclickList)
	ON_WM_SIZE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRemoteControlServerDlg 消息处理程序

BOOL CRemoteControlServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_list.InsertColumn(0, L"IP地址", LVCFMT_LEFT, 150);
	m_list.InsertColumn(1, L"端口", LVCFMT_LEFT, 80);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRemoteControlServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRemoteControlServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRemoteControlServerDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	
}


void CRemoteControlServerDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listenSocket = new CListenSocket(this);
	CString str;
	if (m_listenSocket->Create(SERVER_PORT, SOCK_STREAM) == FALSE)
	{
		str.Format(L"服务器创建失败.错误代号%d", GetLastError());
		MessageBox(str);
		return;
	}
	if (m_listenSocket->Listen() == FALSE)
	{
		str.Format(L"服务器监听失败.错误代号%d", GetLastError());
		MessageBox(str);
		return;
	}
	m_serverStartTime = CTime::GetCurrentTime();
	SetTimer(STATUS_TIMER_INFO, 1000,nullptr);
	
	GetDlgItem(IDC_BTN_CLOSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
}


void CRemoteControlServerDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_listenSocket != nullptr)
	{
		m_listenSocket->Close();
		delete m_listenSocket;
		m_listenSocket = nullptr;
	}
	KillTimer(STATUS_TIMER_INFO);
	SetDlgItemText(IDC_STATE,L"服务器未运行：0天00时00分00秒");
	GetDlgItem(IDC_BTN_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
}


void CRemoteControlServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
	case STATUS_TIMER_INFO:
	{
		CTime currentTime = CTime::GetCurrentTime();
		CTimeSpan timeSpan = currentTime - m_serverStartTime;

		SetDlgItemText(IDC_STATE, timeSpan.Format(L"服务器已运行：%D天%H时%M分%S秒"));
		break;
	}

	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CRemoteControlServerDlg::InsertList(CClientSocket* clientSocket)
{
	int nCount = m_list.GetItemCount();
	CString ip;
	UINT port;
	clientSocket->GetPeerName(ip, port);
	CString strPort;
	strPort.Format(L"%d", port);

	m_list.InsertItem(nCount, ip);
	m_list.SetItemText(nCount, 1, strPort);

	m_list.SetItemData(nCount, (DWORD)clientSocket);
}




void CRemoteControlServerDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int nSel = m_list.GetNextSelectedItem(pos);
	if (nSel < 0)
		return;

	//获取本行的数据段
	CClientSocket* pSocket = (CClientSocket*)m_list.GetItemData(nSel);
	CShowDlg* dlg = new CShowDlg(pSocket);
	dlg->Create(IDD_SHOW, this);
	dlg->ShowWindow(SW_SHOW);
	

	
	
	*pResult = 0;

}


void CRemoteControlServerDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int nSel = m_list.GetNextSelectedItem(pos);
	if (nSel < 0)
		return;

	POINT pt;
	GetCursorPos(&pt);
	//弹出菜单
	CMenu menu;
	menu.LoadMenuW(IDR_MENU);
	CMenu* pSubMenu = menu.GetSubMenu(0);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
	
	
	*pResult = 0;
}






