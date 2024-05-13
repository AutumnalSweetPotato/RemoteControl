#include "pch.h"
#include "CListenSocket.h"
#include "RemoteControlServerDlg.h"
#include "CClientSocket.h"
CListenSocket::CListenSocket(CRemoteControlServerDlg* dlg)
{
	m_pMaindlg = dlg;
}

void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CClientSocket* pSocket = new CClientSocket;
	Accept(*pSocket);
	m_pMaindlg->InsertList(pSocket);
	CSocket::OnAccept(nErrorCode);
}
