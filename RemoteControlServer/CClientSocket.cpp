#include "pch.h"
#include "CClientSocket.h"
#include "CShowDlg.h"

void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	MSGHEAD headMsg{ 0 };
	Receive(&headMsg, sizeof(MSGHEAD));

	switch (headMsg.msg)
	{
	case CLIENT_IMAGE:
	{
		//����ͼ��
		IMAGEDATA image = RecvImage();
		//��ʾͼ��
		m_pMainDlg->ShowImage(image);
		break;
	}
	default:
		break;
	}


	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::SetShowDlg(CShowDlg* pMainDlg)
{
	m_pMainDlg = pMainDlg;
}

IMAGEDATA CClientSocket::RecvImage()
{
	IMAGEDATA image = { 0 };
	Receive(&image.logbmp, sizeof(BITMAP));
	int nSize = image.logbmp.bmWidthBytes * image.logbmp.bmHeight;
	image.data = new char[nSize];


	int nSendByteTotal = 0;
	int nCurrentSendByte = 0;
	char* p = image.data;
	do
	{
		nCurrentSendByte = Receive(p, nSize - nSendByteTotal);
		nSendByteTotal += nCurrentSendByte;
		p += nCurrentSendByte;
	} while (nSendByteTotal < nSize);

	return image;
}
