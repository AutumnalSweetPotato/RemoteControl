// CShowDlg.cpp: 实现文件
//

#include "pch.h"
#include "RemoteControlServer.h"
#include "CShowDlg.h"
#include "afxdialogex.h"
#include"CClientSocket.h"

// CShowDlg 对话框

IMPLEMENT_DYNAMIC(CShowDlg, CDialogEx)

CShowDlg::CShowDlg(class CClientSocket* pSocket, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHOW, pParent)
{
	m_pSocket = pSocket;
}

CShowDlg::~CShowDlg()
{
}



void CShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CShowDlg 消息处理程序


void CShowDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	CDialogEx::OnCancel();
	//delete this;
	
}


BOOL CShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	// TODO:  在此添加额外的初始化
	CString ip;
	UINT port;
	m_pSocket->GetPeerName(ip, port);
	CString strPort;
	strPort.Format(L"%d", port);
	CString title;
	title.Format(L"\\\\%s : %s", ip, strPort);
	SetWindowText(title);
	m_pSocket->SetShowDlg(this);
	
	MoveWindow(0, 0, 1920, 1080);

	//发送监控命令
	MSGHEAD head{ COMMAND::CLIENT_START_MONITOR ,0 };
	m_pSocket->Send(&head, sizeof(MSGHEAD));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CShowDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect, RGB(0, 0, 0));
	return TRUE;

	//return CDialogEx::OnEraseBkgnd(pDC);
}

void CShowDlg::ShowImage(IMAGEDATA image)
{
	m_imageData = image;
	Invalidate(FALSE);
}


void CShowDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	if (m_imageData.data == NULL)
	{
		return;
	}
	int nSize = m_imageData.logbmp.bmWidthBytes * m_imageData.logbmp.bmHeight;
	BITMAPINFOHEADER bih;
	bih.biBitCount = m_imageData.logbmp.bmBitsPixel;
	bih.biClrImportant = 0;
	bih.biClrUsed = 0;
	bih.biCompression = 0;
	bih.biHeight = m_imageData.logbmp.bmHeight;
	bih.biWidth = m_imageData.logbmp.bmWidth;
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = nSize;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;

	//创建位图
	CBitmap bmp;
	if (FALSE == bmp.CreateBitmapIndirect(&m_imageData.logbmp))
	{
		return;
	}
	if (bmp.m_hObject == nullptr)
		return;

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	SetDIBits(memDC, bmp, 0, m_imageData.logbmp.bmHeight, m_imageData.data, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
	memDC.SelectObject(&bmp);


	CRect rect;
	GetClientRect(&rect);

	dc.SetStretchBltMode(HALFTONE);
	dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, m_imageData.logbmp.bmWidth, m_imageData.logbmp.bmHeight,SRCCOPY);

	delete[] m_imageData.data;
	m_imageData.data = NULL;
}


void CShowDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	MSGHEAD headMsg = { COMMAND::CLIENT_STOP_MONITOR ,0 };
	m_pSocket->Send(&headMsg,sizeof(MSGHEAD));

	CDialogEx::OnClose();
	
}



