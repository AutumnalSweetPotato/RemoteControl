
// RemoteControlClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RemoteControlClient.h"
#include "RemoteControlClientDlg.h"
#include "afxdialogex.h"
#include "ClientSocket.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRemoteControlClientDlg 对话框



CRemoteControlClientDlg::CRemoteControlClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REMOTECONTROLCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteControlClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRemoteControlClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRemoteControlClientDlg 消息处理程序

BOOL CRemoteControlClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	MoveWindow(-10, -10, -5, -5);
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if (!InitSystem())
	{
		MessageBox(L"初始化失败.");
		EndDialog(IDOK);
	}
	m_clientSocket = new ClientSocket(this);
	CString str;
	if (m_clientSocket->Create() == FALSE)
	{
		str.Format(L"客户端创建失败.错误代号%d", GetLastError());
		MessageBox(str);
		EndDialog(IDOK);
		
	}
	
	if (m_clientSocket->Connect(m_strIpAddress,SERVER_PORT) == FALSE)
	{
		str.Format(L"连接服务器失败.错误代号%d", GetLastError());
		MessageBox(str);
		EndDialog(IDOK);
		
	}


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRemoteControlClientDlg::OnPaint()
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
HCURSOR CRemoteControlClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRemoteControlClientDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void CRemoteControlClientDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}

BOOL CRemoteControlClientDlg::InitSystem()
{
	//读取配置文件
	wchar_t szAddress[16];
	::GetPrivateProfileString(L"Base", L"ServerIPAddress", L"0.0.0.0", szAddress, sizeof(szAddress), L"./InitSystem.ini");
	m_strIpAddress = szAddress;
	if (m_strIpAddress.IsEmpty() || m_strIpAddress == L"0.0.0.0")
	{
		//创建一个配置文件模板
		CFile file;
		if (FALSE == file.Open(L"InitSystem.ini", CFile::modeCreate | CFile::modeReadWrite))
		{
			return FALSE;
		}
		char szText[] = "[Base]\r\nServerIPAddress=169.254.76.130";
		file.Write(szText, strlen(szText));
		file.Close();
		
		return FALSE;
	}
	return TRUE;
}
/// <summary>
/// 捕获屏幕
/// </summary>
IMAGEDATA CRemoteControlClientDlg::CatchScreen()
{

	//获取屏幕宽和高
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	//CDC* dc = GetDesktopWindow()->GetDC();
	CDC dc;
	dc.CreateDC(L"DISPLAY",NULL,NULL,NULL);
	
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, width, height);
	//创建内存DC
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&bmp);
	memDC.BitBlt(0, 0, width, height, &dc, 0, 0, SRCCOPY);

	//获取图片的位图信息
	BITMAP logBmp;
	bmp.GetBitmap(&logBmp);

	//获取大小
	int nSize = logBmp.bmWidthBytes * logBmp.bmHeight;
	char* buffer = new char[nSize];

	BITMAPINFOHEADER bih;
	bih.biBitCount = logBmp.bmBitsPixel;	
	bih.biClrImportant = 0;
	bih.biClrUsed = 0;
	bih.biCompression = 0;
	bih.biHeight = logBmp.bmHeight;
	bih.biWidth = logBmp.bmWidth;
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = nSize;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	
	GetDIBits(dc, bmp, 0, logBmp.bmHeight, buffer, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
	//释放
	memDC.DeleteDC();
	bmp.DeleteObject();
	dc.DeleteDC();


	//赋值返回
	IMAGEDATA imageData;
	imageData.data = buffer;
	imageData.logbmp = logBmp;
	return imageData;
}
/// <summary>
/// 开始捕获屏幕
/// </summary>
void CRemoteControlClientDlg::StartMonitor()
{
	SetTimer(MONITOR_TIMER_ID, 50, NULL);
}

void CRemoteControlClientDlg::StopMonitor()
{
	KillTimer(MONITOR_TIMER_ID);
}


void CRemoteControlClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case MONITOR_TIMER_ID:
	{
		//获取屏幕
		SendImage();
		break;
	}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
/// <summary>
/// 发送图像
/// </summary>
void CRemoteControlClientDlg::SendImage()
{
	IMAGEDATA image = CatchScreen();
	MSGHEAD headMsg{ CLIENT_IMAGE, 0};
	m_clientSocket->Send(&headMsg, sizeof(MSGHEAD));		//发命令
	m_clientSocket->Send(&image.logbmp, sizeof(BITMAP));	//发头

	//发送数据
	int nSize = image.logbmp.bmWidthBytes * image.logbmp.bmHeight;

	char* p = image.data;

	int nSendByteTotal = 0;
	int nCurrentSendByte = 0;

	do
	{
		nCurrentSendByte = m_clientSocket->Send(p, nSize - nSendByteTotal);
		nSendByteTotal += nCurrentSendByte;
		p += nCurrentSendByte;
	} while (nSendByteTotal < nSize);

	delete[] image.data;
	image.data = NULL;
}
