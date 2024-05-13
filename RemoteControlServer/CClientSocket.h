#pragma once
#include <afxsock.h>
class CClientSocket :
    public CSocket
{
public:
    virtual void OnReceive(int nErrorCode);
    void SetShowDlg(class CShowDlg* pMainDlg);
private:
    IMAGEDATA RecvImage();
    class CShowDlg* m_pMainDlg;
};

