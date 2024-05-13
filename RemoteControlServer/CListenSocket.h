#pragma once
#include <afxsock.h>
class CListenSocket :
    public CSocket
{
private:
    class CRemoteControlServerDlg* m_pMaindlg;
public:
    CListenSocket(class CRemoteControlServerDlg* dlg);
    virtual void OnAccept(int nErrorCode);
};

