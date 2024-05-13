#pragma once


// CShowDlg 对话框

class CShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowDlg)

public:
	CShowDlg(class CClientSocket* pSocket, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CShowDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOW };
#endif
private:
	class CClientSocket* m_pSocket;
	IMAGEDATA m_imageData;

	
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void ShowImage(IMAGEDATA image);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	
};
