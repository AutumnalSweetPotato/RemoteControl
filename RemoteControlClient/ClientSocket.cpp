#include "pch.h"
#include "ClientSocket.h"
#include "RemoteControlClientDlg.h"



ClientSocket::ClientSocket(CRemoteControlClientDlg* pMainDlg)
{
	m_pMainDlg = pMainDlg;
}

void ClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	MSGHEAD head{ 0 };
	Receive(&head, sizeof(MSGHEAD));

	switch (head.msg)
	{
	case COMMAND::CLIENT_START_MONITOR :  //开始监控
		{
		m_pMainDlg->StartMonitor();


			break;
		}
	case COMMAND::CLIENT_STOP_MONITOR:
	{
		m_pMainDlg->StopMonitor();
		break;
	}

	default:
		break;
	}
	CSocket::OnReceive(nErrorCode);
}
