
// MFCApplication4Dlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

//CIP AddressCtrl E_IPv;

// CMFCApplication4Dlg ��ȭ ����
class CMFCApplication4Dlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFCApplication4Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	virtual BOOL PreTranslateMessage(MSG* pMsg); //����Ű esc ��ŷ
	static CWinThread* m_pThreadSend; //����������
	CPoint m_pos; //���콺��ǥ





// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION4_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.
	afx_msg LONG OnHotKey(WPARAM wParam, LPARAM IParam);

// �����Դϴ�.
protected:
	HICON m_hIcon;
	HACCEL m_hAccelTable; //��Ű����
	//HACCEL m_hAccelTable2;
	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	void OnMouseMove(UINT nFlags, CPoint point);;
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	BOOL start1;
	BOOL start2;
	CListBox log; //�α�


	CString how1; //������
	
	CString Name; //����
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit3();

	CString price;
	afx_msg void OnBnClickedCheck1();
	BOOL enterdll;

	afx_msg void OnBnClickedCheck3();
	BOOL goout; //d�͸�
	CString log2;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnHotkey();
	bool hoykeyf5;
	afx_msg void OnBnClickedButton3();
	CString mtu56;
	CString MTU1476;


	afx_msg void OnHotket2();
	CEdit WARI1;
	CEdit WARI2;

	afx_msg void OnWari();
	afx_msg void OnRunmp();

	CString GetLocalIP(void);

	
	CListBox getIP;
	CComboBox storemode;

	afx_msg void OnBnClickedCheck4();
	BOOL LAN;
};
