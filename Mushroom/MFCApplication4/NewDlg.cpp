

/*
	Mushroom.cpp : Made By â�� 2016 04 02
	MTU �� ��Ʈ�� ���� ���̾�α�
*/

#include "stdafx.h"
#include "NewDlg.h"
#include "afxdialogex.h"
#include "Resource.h"

// NewDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(NewDlg, CDialogEx)

NewDlg::NewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

NewDlg::~NewDlg()
{
}

void NewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NewDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &NewDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &NewDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// NewDlg �޽��� ó�����Դϴ�.


void NewDlg::OnBnClickedButton2()
{
	MessageBox(_T("������"));
	WinExec("netsh interface ipv4 set subinterface ""�̴���"" mtu=1500 store=persisten", SW_HIDE);
//	WinExec("netsh interface ipv4 set subinterface ""���� ���� ����"" mtu=56 store=persisten", SW_HIDE);
//	WinExec("netsh interface ipv4 set subinterface ""Local Area Connection"" mtu=1476 store=persisten", SW_HIDE);
//	WinExec("netsh interface ipv4 set subinterface ""���� ��Ʈ��ũ ����"" mtu=1476 store=persisten", SW_HIDE);
}


void NewDlg::OnBnClickedButton1()
{
	MessageBox(_T("������"));

	WinExec("netsh interface ipv4 set subinterface ""�̴���"" mtu=30 store=persisten", SW_HIDE);
//	WinExec("netsh interface ipv4 set subinterface ""���� ���� ����"" mtu=30 store=persisten", SW_HIDE);
//	WinExec("netsh interface ipv4 set subinterface ""Local Area Connection"" mtu=30 store=persisten", SW_HIDE);
//	WinExec("netsh interface ipv4 set subinterface ""���� ��Ʈ��ũ ����"" mtu=1476 store=persisten", SW_HIDE);
}