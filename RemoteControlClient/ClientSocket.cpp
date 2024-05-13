#include "pch.h"
#include "ClientSocket.h"
#include "RemoteControlClientDlg.h"



ClientSocket::ClientSocket(CRemoteControlClientDlg* pMainDlg)
{
	m_pMainDlg = pMainDlg;
}

void ClientSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	MSGHEAD head{ 0 };
	Receive(&head, sizeof(MSGHEAD));

	switch (head.msg)
	{
	case COMMAND::CLIENT_START_MONITOR :  //��ʼ���
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
