
// RemoteControlServerDlg.h: 头文件
//

#pragma once


// CRemoteControlServerDlg 对话框
class CRemoteControlServerDlg : public CDialogEx
{
// 构造
public:
	CRemoteControlServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMOTECONTROLSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnClose();

private:
	CTime m_serverStartTime;		///服务器开启时的当前时间
	class CListenSocket* m_listenSocket;
public:
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void InsertList(class CClientSocket* clientSocket);
private:
	CListCtrl m_list;

	
public:
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	
	
};
