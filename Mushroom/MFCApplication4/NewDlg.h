#pragma once


// NewDlg ��ȭ �����Դϴ�.

class NewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewDlg)

public:
	NewDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~NewDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
