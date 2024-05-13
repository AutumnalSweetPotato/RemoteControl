#pragma once
#include <afxsock.h>
class ClientSocket :
    public CSocket
{
public:
    ClientSocket(class CRemoteControlClientDlg* pMainDlg);
    virtual void OnReceive(int nErrorCode);
private:
    class CRemoteControlClientDlg* m_pMainDlg;
};

