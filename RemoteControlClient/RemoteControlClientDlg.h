﻿
// RemoteControlClientDlg.h: 头文件
//

#pragma once


// CRemoteControlClientDlg 对话框
class CRemoteControlClientDlg : public CDialogEx
{
// 构造
public:
	CRemoteControlClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMOTECONTROLCLIENT_DIALOG };
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
	virtual void OnCancel();
private:
	class ClientSocket* m_clientSocket;
	CString m_strIpAddress;
	BOOL InitSystem();

	IMAGEDATA CatchScreen();				///捕获屏幕
public:
	void StartMonitor();			///开始监控函数
	void StopMonitor();				///停止监控函数
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SendImage();
	
};
