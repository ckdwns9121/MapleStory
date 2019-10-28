
/*

	Mushroom.cpp : Made By â�� 2016 04 02
	winsock ��� ������Ÿ�� ����ȭ
	winpcap testing
*/

#pragma comment(lib,"ws2_32.lib")
//#pragma comment (lib, "wpcap.lib")  winpcap 
#include "stdafx.h"
#include "MFCApplication4.h"
#include "MFCApplication4Dlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <time.h>
#include <io.h>   
#include <fcntl.h>
#include <string>
#include <string.h>
#include <winsock2.h>
#include "pcap.h"
#include "NewDlg.h"

#define DESIRED_WINSOCK_VERSION    0x0101
#define MINIMUM_WINSOCK_VERSION    0x0001

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;
#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1) 
#define PROTO_TCP	6

#define BUFSIZE 512
SOCKET sock; // ����
SYSTEMTIME st; //�ð�
COLORREF color_front;
COLORREF color_back = 0xBB66EE;

int delay = 50;
DWORD ThreadID;
HWND hWnd;
HWND hMP = FindWindow(NULL, "MapleStory");
HWND hKey = FindWindow(NULL, "VirtualKeyboard");
HWND m_hWnd = FindWindow(NULL, "Mushroom");
BOOL WARISTART = FALSE, ColorCheck = FALSE;


char SignalDevice[256], errbuf[PCAP_ERRBUF_SIZE];
pcap_if_t* alldevs;
pcap_if_t* d;
pcap_t* adhandle;

unsigned int	m_SOCK;
int				m_BUFFSZ;
unsigned char* m_BUFF;		// ��Ŷ ����
int				m_PACKETSZ; // ���ۿ� ���� ��Ŷ ũ��	
unsigned char* m_PDATA;	// ������ ����
unsigned char* m_PDATATXT;	// ������ ����(Text)
int				m_DATASZ;	// ������ ũ��	
unsigned short	m_PROTO;	// ��������
unsigned int	m_SRCIP;	// �۽� ip
unsigned int	m_DSTIP;	// ���� ip
int				m_SRCPORT;	// �۽� port
int				m_DSTPORT;	// ���� port
struct iphdr* m_PIPH;		// IP ���
struct tcphdr* m_PTCPH;	// TCP ���
struct udphdr* m_PUDPH;	// UDP ���
struct icmphdr* m_PICMPH;	// ICMP ���
unsigned char* m_TXTBUFF;	// ��¿� ����
int				m_TXTSZ;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
struct iphdr {
	unsigned char ihl : 4,
		version : 4;
	unsigned char tos;
	unsigned short tot_len;
	unsigned short id;
	unsigned short frag_off;
	unsigned char ttl;
	unsigned char protocol;
	unsigned short check;
	unsigned int saddr;
	unsigned int daddr;
};

struct tcphdr {
	unsigned short source;
	unsigned short dest;
	unsigned int seq;
	unsigned int ack_seq;

	unsigned short nsf : 1,
		res1 : 3,
		doff : 4,
		fin : 1,
		syn : 1,
		rst : 1,
		psh : 1,
		ack : 1,
		urg : 1,
		ece : 1,
		cwr : 1;

	unsigned short window;
	unsigned short check;
	unsigned short urg_ptr;
};

struct udphdr {
	unsigned short source;
	unsigned short dest;
	unsigned short len;
	unsigned short check;
};

struct icmphdr {
	unsigned char type;
	unsigned char code;
	unsigned short checksum;
	union {
		struct {
			unsigned short id;
			unsigned short sequence;
		} echo;
		unsigned int gateway;
		struct {
			unsigned short __unused;
			unsigned short mtu;
		} frag;
	} un;
};

int WARICOUNT; //�͸�ī����
int WARIDELAY; //�͸�������
bool dir = 0;
bool cheak = FALSE;
int ix = 0;
int iy = 0;

int pos_x = 0; // ������ ������
int pos_y = 0;

int posuser_x = 0;
int posuser_y = 0;// ����� ����

int startstop = 0;
int thread_stop = 1;
bool expert_state = 0, PixelCheck = 0;//��ȣ �ν� ���� ����
int numberopen = 50;    //â���� Ƚ��
bool isok_state = 0; //�������� ������ ���� ����


int ishillstate; //��� ����
bool speed_state = 1; //���̳� ��Ŭ ����
bool login;  //�α��� ������Ʈ
bool usercom;//���� ��ǻ�� ����
bool team_state = 0; //�� ��Ŭ ������

int protect_state = 0;
int login_state = 0;

static char str_num[50];  //���� ���� ����
static char str_ing[50];
static char str_start[50];
static char str_end[50];
char Buffer[256]; //�αױ��
int H[20] = { 0, }; //�ʱ�ȭ

CString xy; //xy��ǥ
CString S_log; //�α�
CString strIPAddress;
POINT warick1, warick2, StorePoint; //�͸�
COLORREF color, StoreColor;// ������

/*���콺 ��ǥ�� �ȼ��� ��������*/
int getpixcel(HDC hdc, int x, int y) {
	DWORD color;
	void* p = (void*)GetPixel;

	__asm {
		mov esi, esp;
		push dword ptr y;
		push dword ptr x;
		push hdc;
		mov eax, dword ptr[p];
		add eax, 5;

		call cmd;
	cmd:
		pop ecx;
		add ecx, 12;
		push ecx;

		mov edi, edi;
		push ebp;
		mov ebp, esp;

		jmp eax;

		mov color, eax;
	}

	return color;
}
DWORD WINAPI  SockStart(LPVOID Param); //�ϻ���
DWORD WINAPI SockStart2(LPVOID Param);// �����
DWORD WINAPI WinPcapStart(LPVOID Param); //winpcap
DWORD WINAPI PICXEL(LPVOID Param);//���ν� ���
void CALLBACK WARITimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK MTUTimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void storename(); //�ϻ���
void storename2();// �����
void storename3(); //���ϸ鼭���
void OffWindowLine(); // âũ������
void getitem(); // �����ۿø���
bool Isok(); // 4���ν� ����
void ReSetMaket(); //�ϻ�����
void ReSetMaket2(); //�������
void ReSetMaket3(); //�������
void fullmode(); //Ǯ���
void ReSetStore(); //��󸮻�
void LOGWRITE(char* log2); //�α�
void TIMELOGWRITE(char* log2); //Ÿ�ӷα�
void Click(int Count, int x, int y);// �͸�
void SetClipboardText(CString strSource); //�����Լ�
void GetPixelMP(int x, int y, HWND hMP); //������
void ran();//MTU ���� ��-�� �����Ϸ�


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnRun();

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)


END_MESSAGE_MAP()

// CMFCApplication4Dlg ��ȭ ����

CMFCApplication4Dlg::CMFCApplication4Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION4_DIALOG, pParent)
	, start1(FALSE)
	, start2(FALSE)
	, how1(_T("�Ϲݻ���"))
	, Name(_T("^^"))
	, price(_T("9999999999"))
	, enterdll(FALSE)
	, log2(_T("v"))
	, hoykeyf5(false)
	, mtu56(_T("56"))
	, MTU1476(_T("1476"))
	, LAN(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, log);
	DDX_CBString(pDX, IDC_COMBO1, how1);
	DDX_Text(pDX, IDC_EDIT1, Name);
	DDX_Text(pDX, IDC_EDIT3, price);
	DDX_Check(pDX, IDC_CHECK1, enterdll);
	DDX_Check(pDX, IDC_CHECK3, goout);
	DDX_LBString(pDX, IDC_LIST1, log2);
	DDX_Text(pDX, IDC_EDIT2, mtu56);
	DDX_Text(pDX, IDC_EDIT4, MTU1476);

	DDX_Control(pDX, IDC_EDIT5, WARI1);
	DDX_Control(pDX, IDC_EDIT6, WARI2);
	DDX_Control(pDX, IDC_LIST6, getIP);
	DDX_Check(pDX, IDC_CHECK4, LAN);
}

BEGIN_MESSAGE_MAP(CMFCApplication4Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication4Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication4Dlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication4Dlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT3, &CMFCApplication4Dlg::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_CHECK1, &CMFCApplication4Dlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK3, &CMFCApplication4Dlg::OnBnClickedCheck3)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMFCApplication4Dlg::OnLbnSelchangeList1)
	ON_COMMAND(IDR_HotKey, &CMFCApplication4Dlg::OnHotkey)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication4Dlg::OnBnClickedButton3)


	ON_COMMAND(ID_HOTKET2, &CMFCApplication4Dlg::OnHotket2)

	ON_COMMAND(ID_WARI, &CMFCApplication4Dlg::OnWari)
	ON_COMMAND(ID_RUNMP, &CMFCApplication4Dlg::OnRunmp)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)


	ON_BN_CLICKED(IDC_CHECK4, &CMFCApplication4Dlg::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CMFCApplication4Dlg �޽��� ó����


/* ���콺 ��ǥ �޾ƿ��� */
void CMFCApplication4Dlg::OnMouseMove(UINT nFlags, CPoint point) {

	m_pos = point;

	Invalidate();
	CDialogEx::OnMouseMove(nFlags, point);

}
/* ������ ��Ű��*/
BOOL CMFCApplication4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	if (hMP == FALSE) {
		MessageBox(_T("�������� ��������ּ���."), _T("Error"),
			MB_ICONERROR | MB_OK);
	}
	CString Temp;
	Temp.Format("%s", GetLocalIP());
	getIP.AddString(Temp);

	ModifyStyle(WS_THICKFRAME, 0, SWP_FRAMECHANGED);

	//LOGWRITE("-------  Made By â��  -------");
	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.
	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu * pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR2));

	RegisterHotKey(m_hWnd, 3, MOD_NOREPEAT, VK_F3);
	RegisterHotKey(m_hWnd, 4, MOD_NOREPEAT, VK_F4);
	RegisterHotKey(m_hWnd, 6, MOD_NOREPEAT, VK_F6);
	RegisterHotKey(m_hWnd, 10, MOD_NOREPEAT, VK_F10);
	RegisterHotKey(m_hWnd, 11, MOD_NOREPEAT, VK_F11);
	//��Ű����

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

LONG CMFCApplication4Dlg::OnHotKey(WPARAM wParam, LPARAM IParam) {
	if (wParam == 4) {
		CString str;
		GetDlgItemText(IDC_EDIT1, str);
		SetClipboardText(str);
		ReSetStore();
	}
	if (wParam == 3) {

		GetCursorPos(&StorePoint);
		GetPixelMP(StorePoint.x, StorePoint.y, hMP);
		StoreColor = color;
		sprintf(Buffer, "�ȼ��� X:%d Y: %d C: 0x%X", StorePoint.x, StorePoint.y, StoreColor);
		TIMELOGWRITE(": �ȼ� ���� �Ϸ�");
		LOGWRITE(Buffer);

	}
	if (wParam == 6) {
		CString str;
		GetDlgItemText(IDC_EDIT1, str);
		SetClipboardText(str);
		ran();
		TIMELOGWRITE(": ���ۼ���");
	}

	if (wParam == 10) {
		CString str;
		GetDlgItemText(IDC_EDIT1, str); //���񺹻�
		SetClipboardText(str);
		UpdateData(TRUE);
		if (hMP == FALSE) {
			MessageBox(_T("�������� ��������ּ���."), _T("Error"),
				MB_ICONERROR | MB_OK);
		}

		else if (how1 == "�Ϲݻ���") {

			TIMELOGWRITE(": �ϻ������");
			OffWindowLine();
			storename();
			CreateThread(NULL, 0, SockStart, NULL, 0, &ThreadID);
			//CreateThread(NULL, 0, WinPcapStart, NULL, 0, &ThreadID);
			//CreateThread(NULL, 0, PICXEL, NULL, 0, &ThreadID);
		}

		else if (how1 == "������") {
			TIMELOGWRITE(": ��������");
			OffWindowLine();
			CreateThread(NULL, 0, SockStart2, NULL, 0, &ThreadID);
			storename2();
		}

		else {
			MessageBox(_T("����� ����."), _T("Error"),
				MB_ICONERROR | MB_OK);
		}

		UpdateData(TRUE);
		if (how1 == "�Ϲݻ���" || how1 == "������") {
			if (goout == TRUE) {
				OffWindowLine();
				WARIDELAY = (atoi(Buffer) * 60000);
				if (WARIDELAY == 0)
				{
					WARIDELAY = 1800000;
				}
				WARICOUNT = 0;

				SetTimer(1234, 1800000, (TIMERPROC)WARITimerProc); //�͸� 30��
				SetTimer(1235, 600000, (TIMERPROC)MTUTimerProc); //mtu �͸� 20��
			}
		}
		if (enterdll == TRUE) {

			WinExec("netsh interface ipv4 set subinterface ""�̴���"" mtu=56 store=persisten", SW_HIDE);
		}
	}
	if (wParam == 11) {
		if (hMP == FALSE) {
			MessageBox(_T("�������� ��������ּ���."), _T("Error"),
				MB_ICONERROR | MB_OK);
		}
		else {
			expert_state = FALSE;
			WARISTART = FALSE;
			KillTimer(1234);
			KillTimer(1235);
			TIMELOGWRITE(": �ν�����");
		}
	}

	return 0;
}



void CMFCApplication4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMFCApplication4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}


}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMFCApplication4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��Ű�����Լ�
BOOL CMFCApplication4Dlg::PreTranslateMessage(MSG * pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (m_hAccelTable != NULL)
	{
		if (TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
			return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);

}


/*�Ϲ� ���� ��� ���� ������*/
DWORD WINAPI SockStart(LPVOID Param)
{

	m_hWnd = FindWindow(NULL, "Mushroom");

	char ac[255] = { 0, };
	unsigned long v = 1, v2;
	struct in_addr addr;

	expert_state = 1; // recvfrom �ʱ�ȭ
	PixelCheck = 0;
	//UpMapleWindow();
	SetForegroundWindow(hMP);
	//	Protect1();  ///////////////////------------------����
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;
	sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);

	if (sock == INVALID_SOCKET) {
		MessageBox(NULL, "�����ڱ������� �������ּ���.", "ERROR", MB_OK);
		return 0;
	}
	if (gethostname(ac, sizeof(ac)) != SOCKET_ERROR)
	{
		struct hostent* phe = gethostbyname(ac);
		if (phe != NULL)
		{
			memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
		}

	}
	struct sockaddr_in SockAddr;
	ZeroMemory(&SockAddr, sizeof(SockAddr));
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = NULL;
	SockAddr.sin_addr.s_addr = addr.s_addr;
	if (bind(sock, (sockaddr*)& SockAddr, sizeof(SockAddr)) == SOCKET_ERROR)
	{
		MessageBox(NULL, "���ε�", "����", MB_OK);
	}

	struct sockaddr_in fromAddr;
	int fromAddrLen = sizeof(fromAddr);

	if (WSAIoctl(sock, SIO_RCVALL, &v, sizeof(v), 0, 0, &v2, 0, 0) == SOCKET_ERROR)
	{
		MessageBox(NULL, "�ɼ� ���� ", "����", MB_OK);
	}

	HDC hdc;
	hdc = GetDC(NULL);
	int iphsz, hsz, pksz;
	m_BUFFSZ = 65536 * 2;
	// ��Ŷ ���� ����
	m_BUFF = (unsigned char*)malloc(m_BUFFSZ + 2);
	//m_PDATATXT=(unsigned char*)malloc(m_BUFFSZ+2);
	m_PIPH = (struct iphdr*)m_BUFF;
	ColorCheck == TRUE;
	while (expert_state)
	{

		memset(&fromAddr, 0, fromAddrLen);
		//   m_PACKETSZ = recvfrom(sock,(char*)m_BUFF,m_BUFFSZ,0,(struct sockaddr *)&fromAddr,&fromAddrLen);
		m_PACKETSZ = recvfrom(sock, (char*)m_BUFF, m_BUFFSZ, 0, (struct sockaddr*) & fromAddr, &fromAddrLen);


		if (m_PACKETSZ > 0) {

			m_PROTO = m_PIPH->protocol;

			m_SRCPORT = 0;

			iphsz = (m_PIPH->ihl * 4);
			pksz = m_PACKETSZ;

			//if(m_PROTO==PROTO_TCP && m_SRCPORT == 80 ) {
			if (m_PROTO == PROTO_TCP) {

				m_PTCPH = (struct tcphdr*) & m_BUFF[iphsz];
				m_SRCPORT = ntohs(m_PTCPH->source);
				//			m_DSTPORT=ntohs(m_PTCPH->dest);			
				hsz = m_PTCPH->doff * 4;
				m_PDATA = (unsigned char*)& m_BUFF[iphsz + hsz];
				m_DATASZ = pksz - iphsz - hsz;

				if (m_SRCPORT >= 0x2124 && m_SRCPORT <= 0x231d) {

					if (m_DATASZ == 11) {

						PostMessageSend(hMP, 13, 1835009);
						TIMELOGWRITE(": ��ȣ����");
						if (Isok())
						{
							KillTimer(m_hWnd, 1234);
							KillTimer(m_hWnd, 1235);
							getitem();
							cheak = 0;
							closesocket(sock);
							WSACleanup();
							expert_state = 0;
							PixelCheck = 1;
							Sleep(500);
							TIMELOGWRITE(": �ν�����");
							WinExec("netsh interface ipv4 set subinterface ""�̴���"" mtu=1476 store=persisten", SW_HIDE);
							ColorCheck == FALSE;
							return 1;

						}
						else {

							ReSetMaket();
							TIMELOGWRITE(": ������������");
							TIMELOGWRITE(": �����Լ�ȣ��");
						}
					}
				}

			}

		}
	}


}

/*������ ��� ���� ������ */
DWORD WINAPI SockStart2(LPVOID Param)
{

	m_hWnd = FindWindow(NULL, "Mushroom");
	char ac[255] = { 0, };
	unsigned long v = 1, v2;

	struct in_addr addr;

	expert_state = 1; // recvfrom �ʱ�ȭ

					  //UpMapleWindow();

	SetForegroundWindow(hMP);

	//	Protect1();  ///////////////////------------------����
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (sock == INVALID_SOCKET) {

		MessageBox(NULL, "�����ڱ������� �������ּ���.", "ERROR", MB_OK);

		return 0;
	}

	if (gethostname(ac, sizeof(ac)) != SOCKET_ERROR)
	{
		struct hostent* phe = gethostbyname(ac);
		if (phe != NULL)
		{
			memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
		}

	}

	// connect()
	struct sockaddr_in SockAddr;
	ZeroMemory(&SockAddr, sizeof(SockAddr));
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = NULL;
	SockAddr.sin_addr.s_addr = addr.s_addr;

	//	if(GetPort()) SockAddr.sin_port = GetPort();
	//	else SockAddr.sin_port = NULL;
	//	SockAddr.sin_addr.s_addr = inet_addr("114.205.138.138");

	if (bind(sock, (sockaddr*)& SockAddr, sizeof(SockAddr)) == SOCKET_ERROR)
	{
		MessageBox(NULL, "���ε�", "����", MB_OK);
	}


	struct sockaddr_in fromAddr;
	int fromAddrLen = sizeof(fromAddr);


	if (WSAIoctl(sock, SIO_RCVALL, &v, sizeof(v), 0, 0, &v2, 0, 0) == SOCKET_ERROR)
	{
		MessageBox(NULL, "�ɼ� ���� ", "����", MB_OK);
		//ErrMsg("WSAIoctl() ����");
		//goto ERR;
	}

	HDC hdc;
	hdc = GetDC(NULL);

	int iphsz, hsz, pksz;

	m_BUFFSZ = 65536 * 2;

	// ��Ŷ ���� ����
	m_BUFF = (unsigned char*)malloc(m_BUFFSZ + 2);
	//m_PDATATXT=(unsigned char*)malloc(m_BUFFSZ+2);
	m_PIPH = (struct iphdr*)m_BUFF;


	while (expert_state)
	{
		memset(&fromAddr, 0, fromAddrLen);
		//    m_PACKETSZ = recvfrom(sock,(char*)m_BUFF,m_BUFFSZ,0,(struct sockaddr *)&fromAddr,&fromAddrLen);
		m_PACKETSZ = recvfrom(sock, (char*)m_BUFF, m_BUFFSZ, 0, (struct sockaddr*) & fromAddr, &fromAddrLen);

		if (m_PACKETSZ > 0) {
			m_PROTO = m_PIPH->protocol;

			m_SRCPORT = 0;

			iphsz = (m_PIPH->ihl * 4);
			pksz = m_PACKETSZ;

			//if(m_PROTO==PROTO_TCP && m_SRCPORT == 80 ) {
			if (m_PROTO == PROTO_TCP) {


				m_PTCPH = (struct tcphdr*) & m_BUFF[iphsz];
				m_SRCPORT = ntohs(m_PTCPH->source);
				//			m_DSTPORT=ntohs(m_PTCPH->dest);			
				hsz = m_PTCPH->doff * 4;
				m_PDATA = (unsigned char*)& m_BUFF[iphsz + hsz];
				m_DATASZ = pksz - iphsz - hsz;

				if (m_SRCPORT >= 0x2124 && m_SRCPORT <= 0x231d) {
					if (m_DATASZ == 11) {
						PostMessageSend(hMP, 13, 1835009);
						TIMELOGWRITE(": ��ȣ����");
						if (Isok())
						{

							KillTimer(m_hWnd, 1234);
							KillTimer(m_hWnd, 1235);
							getitem();
							closesocket(sock);
							WSACleanup();
							expert_state = 0;
							Sleep(500);
							TIMELOGWRITE(": �ν�����");
							WinExec("netsh interface ipv4 set subinterface ""�̴���"" mtu=1476 store=persisten", SW_HIDE);
							return 1;

						}
						else {

							ReSetMaket2();
							TIMELOGWRITE(": ������������");
							TIMELOGWRITE(": �����Լ�ȣ��");
						}
					}

				}

			}

		}
	}
}

/* ��ȣ ���� ���� ��ȣ + ���� �ν�*/
DWORD WINAPI PICXEL(LPVOID Param) {

	expert_state = 1;
	SetForegroundWindow(hMP);
	if (color != StoreColor) {
		PostMessageSend(hMP, 13, 1835009);
		TIMELOGWRITE(": ������");
		if (Isok())
		{

			//	KillTimer(m_hWnd, 1234);
			//	KillTimer(m_hWnd, 1235);
			getitem();
			closesocket(sock);
			WSACleanup();
			expert_state = 0;
			Sleep(500);
			TIMELOGWRITE(": �ν�����");
			WinExec("netsh interface ipv4 set subinterface ""�̴���"" mtu=1476 store=persisten", SW_HIDE);
			return 1;

		}
		////���� �Լ� ȣ��
		else {

			ReSetMaket2();
			TIMELOGWRITE(": ������������");
			TIMELOGWRITE(": �����Լ�ȣ��");
		}

	}


}
/* WINPCAP ��� ������*/
/*
DWORD WINAPI WinPcapStart(LPVOID Param)
{


	char *filter = "tcp && src portrange 8585-8589";
	//SignalCount = 0;
	//sprintf(Buffer, "%d", SignalCount);
	//SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
	int res;
	int inum = atoi(SignalDevice);
	int i = 0;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	//���ͷ� ����
	struct bpf_program fcode;
	bpf_u_int32 NetMask;
	//��Ʈ��ũ �ٹ��̽� ����� �����´�.
	// alldevs�� ����Ʈ ���·� ����Ǹ�, ������ errbuf�� ���� ���� ����
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		sprintf(Buffer, "Error in pcap_findalldevs: %s", errbuf);
		//		MessageBox(MainDlg, Buffer, TEXT("ERROR"), 16);
		return -1;
	}
	//��Ʈ��ũ �ٹ��̽����� ����Ѵ�.
	for (d = alldevs; d; d = d->next)
	{
		++i;
	}
	//���� ó��
	if (i == 0)
	{
		//MessageBox(MainDlg, TEXT("No interfaces found! Make sure WinPcap is installed."), TEXT("ERROR"), 16);
		return -1;
	}

	//ĸó�� ��Ʈ��ũ ����̽� ����
	// �Է°��� ��ȿ�� �Ǵ�
	if (inum < 1 || inum > i)
	{
		//	MessageBox(MainDlg, TEXT("Interface number out of range."), TEXT("ERROR"), 16);
			// ��ġ ��� ����
		pcap_freealldevs(alldevs);
		return -1;
	}

	 ����ڰ� ������ ��ġ��� ����
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);
	Open the device
	if ((adhandle = pcap_open(d->name,          // name of the device
		65536,            // portion of the packet to capture.
						  // 65536 guarantees that the whole packet will be captured on all the link layers
		PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode��  TopSeven��
		1,             // read timeout
		NULL,             // authentication on the remote machine
		errbuf            // error buffer
		)) == NULL)
	{
		sprintf(Buffer, "Unable to open the adapter. %s is not supported by WinPcap", d->name);
		//	MessageBox(MainDlg, TEXT(Buffer), TEXT("ERROR"), 16);
			//Free the device list
		pcap_freealldevs(alldevs);
		return -1;
	}
//	if (SendMessage(GetDlgItem(m_hWnd, IDC_CHECK4), BM_GETCHECK, 0, 0) != BST_UNCHECKED)

	// �ݸ���ũ ����, �̺κ��� ���� �� �𸣰���
	NetMask = 0xffffff;

	// ����ڰ� ������ ���ͷ� ������
	if (pcap_compile(adhandle, &fcode, filter, 1, NetMask) < 0)
	{
	//	MessageBox(MainDlg, TEXT("Error compiling filter: wrong syntax."), TEXT("ERROR"), 16);
		pcap_close(adhandle);
		return -3;
	}

	// ����ڰ� ������ ���ͷ� ����
	if (pcap_setfilter(adhandle, &fcode) < 0)
	{
		//MessageBox(MainDlg, TEXT("Error setting the filter"), TEXT("ERROR"), 16);
		pcap_close(adhandle);
		return -4;
	}
	else
	{
		LOGWRITE("��ȣMAX����.");
	}



	At this point, we don't need any more the device list. Free it
	pcap_freealldevs(alldevs);
	HDC hdc;
	hdc = GetDC(NULL);

	while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0)
	{
		if (header->caplen == 64)
		{
			PostMessageSend(hMP, 13, 1835009);
			TIMELOGWRITE("  :  ��ȣ����");
			if (Isok())
			{
				getitem();
				cheak = 0;
				closesocket(sock);
				WSACleanup();
				expert_state = 0;
				KillTimer(m_hWnd, 1234);
				Sleep(500);
				TIMELOGWRITE("  :  �ν�����");

				return 1;

			}
		}
	}
}
*/


/* ��ȣ�ν� ���۹�ư*/
void CMFCApplication4Dlg::OnBnClickedButton1()
{


	//ClipBoard("hi");
	CString str;
	GetDlgItemText(IDC_EDIT1, str); //���񺹻�
	SetClipboardText(str);
	UpdateData(TRUE);
	if (hMP == FALSE) {
		MessageBox(_T("�������� ��������ּ���."), _T("Error"),
			MB_ICONERROR | MB_OK);
	}

	else if (how1 == "�Ϲݻ���") {

		TIMELOGWRITE(": �ϻ������");
		OffWindowLine();
		storename();
		CreateThread(NULL, 0, SockStart, NULL, 0, &ThreadID);
		//CreateThread(NULL, 0, WinPcapStart, NULL, 0, &ThreadID);
		//CreateThread(NULL, 0, PICXEL, NULL, 0, &ThreadID);
	}

	else if (how1 == "������") {
		TIMELOGWRITE(": ��������");
		OffWindowLine();
		CreateThread(NULL, 0, SockStart2, NULL, 0, &ThreadID);
		storename2();
	}

	else {
		MessageBox(_T("����� ����."), _T("Error"),
			MB_ICONERROR | MB_OK);
	}

	UpdateData(TRUE);
	if (how1 == "�Ϲݻ���" || how1 == "������") {
		if (goout == TRUE) {
			OffWindowLine();
			WARIDELAY = (atoi(Buffer) * 60000);
			if (WARIDELAY == 0)
			{
				WARIDELAY = 1800000;
			}
			WARICOUNT = 0;

			SetTimer(1234, 1800000, (TIMERPROC)WARITimerProc); //�͸� 30��
			SetTimer(1235, 600000, (TIMERPROC)MTUTimerProc); //mtu �͸� 20��
		}
	}
	if (enterdll == TRUE) {

		WinExec("netsh interface ipv4 set subinterface ""�̴���"" mtu=56 store=persisten", SW_HIDE);
	}

}

/*���� ��ư*/
void CMFCApplication4Dlg::OnBnClickedButton2()
{
	::SendMessage(GetSafeHwnd(), WM_CLOSE, NULL, NULL);
}

/*���� ��ư*/
void CMFCApplication4Dlg::OnBnClickedButton3()
{
	if (hMP == FALSE) {
		MessageBox(_T("�������� ��������ּ���."), _T("Error"),
			MB_ICONERROR | MB_OK);
	}
	else {
		expert_state = FALSE;
		WARISTART = FALSE;
		KillTimer(1234);
		KillTimer(1235);
		TIMELOGWRITE(": �ν�����");
	}
}

/*���� && ���� */
void CMFCApplication4Dlg::OnEnChangeEdit1() {}
void CMFCApplication4Dlg::OnEnChangeEdit3() {}

/*���� ���� �ϱ� (MTU�� �����)*/
void CMFCApplication4Dlg::OnBnClickedCheck1()
{
	UpdateData(TRUE);
	if (enterdll == TRUE) {
		CString str;
		GetDlgItemText(IDC_EDIT2, str); //���񺹻�
		SetClipboardText(str);
		S_log.Format("MTU = %s ��������", str);
		log.AddString(S_log);
		//	WinExec("netsh interface ipv4 set subinterface ""Local Area Connection"" mtu=56 store=persisten", SW_HIDE);
		WinExec("netsh interface ipv4 set subinterface ""�̴���"" mtu=56 store=persisten", SW_HIDE);
		//	WinExec("netsh interface ipv4 set subinterface ""���� ���� ����"" mtu=56 store=persisten", SW_HIDE);
		//	WinExec("netsh interface ipv4 set subinterface ""���� ��Ʈ��ũ ����"" mtu=56 store=persisten", SW_HIDE);
	}
	else if (enterdll == FALSE) {
		CString str;
		GetDlgItemText(IDC_EDIT4, str); //���񺹻�
		SetClipboardText(str);
		S_log.Format("MTU = %s ������������", str);
		log.AddString(S_log);

		//	WinExec("netsh interface ipv4 set subinterface ""Local Area Connection"" mtu=1476 store=persisten", SW_HIDE);
		WinExec("netsh interface ipv4 set subinterface ""�̴���"" mtu=1476 store=persisten", SW_HIDE);
		//	WinExec("netsh interface ipv4 set subinterface ""���� ���� ����"" mtu=1476 store=persisten", SW_HIDE);
		//	WinExec("netsh interface ipv4 set subinterface ""���� ��Ʈ��ũ ����"" mtu=1476 store=persisten", SW_HIDE);
	}

}

//�͸�â ������
void CMFCApplication4Dlg::OnBnClickedCheck3()
{
	UpdateData(TRUE);
	if (goout == TRUE) {
		OffWindowLine();
		LOGWRITE("�͸���� ������Ű == F1,F2");
	}
}
/* ���� �ϱ�*/
void CMFCApplication4Dlg::OnBnClickedCheck4()
{
	UpdateData(TRUE);
	if (LAN == TRUE) {
		NewDlg dlg;
		dlg.DoModal();
	}
}
/* F5 �����ϱ� */
void CMFCApplication4Dlg::OnHotkey()
{
	UpdateData(TRUE);
	//ClipBoard("chagnjun");
	CString str;
	GetDlgItemText(IDC_EDIT1, str); //copy
	SetClipboardText(str);

	if (hMP == FALSE) {
		MessageBox(_T("�������� ��������ּ���."), _T("Error"),
			MB_ICONERROR | MB_OK);
	}
	else {
		ReSetStore();
	}
}


/*		F1�͸�		*/
void CMFCApplication4Dlg::OnHotket2()
{
	GetCursorPos(&warick1);
	warick1.x;
	warick1.y;
	xy.Format("x=%d,y=%d", warick1.x, warick1.y);
	WARI1.SetWindowText(xy);
	TIMELOGWRITE(": �͸���� �����Ϸ�");
}

/*		F2�͸�		*/
void CMFCApplication4Dlg::OnWari()
{

	GetCursorPos(&warick2);
	warick2.x;
	warick2.y;
	xy.Format("x=%d,y=%d", warick2.x, warick2.y);
	WARI2.SetWindowText(xy);
	TIMELOGWRITE(": �͸���� �����Ϸ�");
}

/* ������ ����*/
void CMFCApplication4Dlg::OnRunmp()
{
	//	ShellExecute(NULL, "open", "C:\Nexon\Maple\GameLauncher.exe", NULL, NULL, SW_SHOW);
}

/*�͸� �ϴ� �Լ�*/
void CALLBACK WARITimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{


	Sleep(2000);
	WARISTART = TRUE;
	TIMELOGWRITE(": �͸����");
	TIMELOGWRITE(": �͸�����");
	Sleep(500);
	keybd_event(VK_ESCAPE, 0, 0, 0);
	keybd_event(VK_ESCAPE, 0, 2, 0);
	Sleep(500);

	switch (WARICOUNT)
	{
	case 0:
		TIMELOGWRITE(": 1����������");
		Click(2, warick1.x, warick1.y);
		Sleep(500);
		//	if (StoreOpenCheck(hMP))
		{
			Sleep(300);
			TIMELOGWRITE(": 1���������� ������");
			keybd_event(VK_ESCAPE, 0, 0, 0);
			keybd_event(VK_ESCAPE, 0, 2, 0);
			WARICOUNT++;
		}
	case 1:
		TIMELOGWRITE(": 2����������");
		Click(2, warick2.x, warick2.y);
		Sleep(500);
		//if (StoreOpenCheck(hMP))
		{
			Sleep(300);
			TIMELOGWRITE(": 2���������� ������");
			keybd_event(VK_ESCAPE, 0, 0, 0);
			keybd_event(VK_ESCAPE, 0, 2, 0);
			WARICOUNT++;
		}
		break;
	}
	if (WARICOUNT >= 1)
		WARICOUNT = 0;
	//if (StoreOpenCheck(hMP))
	//{
	//	Sleep(1000);
	//	PostMessageSend(hMP, 27, 65537);
	//	Sleep(300);
	//}
	TIMELOGWRITE(": �͸� ����");
	//	MapleIven(hMP);
	storename();
	storename();
	TIMELOGWRITE(": ���� ����");

	WARISTART = FALSE;

}

void CALLBACK MTUTimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	WinExec("netsh interface ipv4 set subinterface ""�̴���"" mtu=1476 store=persisten", SW_HIDE);
	TIMELOGWRITE(": MTU �� 1476");
	Sleep(5000);
	WinExec("netsh interface ipv4 set subinterface ""�̴���"" mtu=56 store=persisten", SW_HIDE);
	TIMELOGWRITE(": MTU �� 56");
}


//�����Լ�
BOOL CopyToClipboard(CListCtrl * pListCtrl, LPCTSTR lpszSeparator = _T("\t"), BOOL bCopyHeaderText = FALSE)
{
	ASSERT(pListCtrl && ::IsWindow(pListCtrl->GetSafeHwnd()));

	CString sResult;
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
	if (!pos)
		return TRUE;

	CWaitCursor wait;
	int nItem, nCount = 0;
	int nColumn = 1;

	if ((pListCtrl->GetStyle() & LVS_TYPEMASK) == LVS_REPORT &&
		pListCtrl->GetExtendedStyle() & LVS_EX_FULLROWSELECT)
	{
		CHeaderCtrl* pHeader = pListCtrl->GetHeaderCtrl();
		nColumn = pHeader ? pHeader->GetItemCount() : 1;

		if (bCopyHeaderText && pHeader)
		{
			for (int i = 0; i < nColumn; ++i)
			{
				TCHAR szBuffer[256];
				HDITEM hdi;
				hdi.mask = HDI_TEXT;
				hdi.pszText = szBuffer;
				hdi.cchTextMax = 256;

				pHeader->GetItem(i, &hdi);
				sResult += szBuffer;
				if (i != nColumn - 1)
					sResult += lpszSeparator;
			}
			++nCount;
		}
	}

	while (pos)
	{
		nItem = pListCtrl->GetNextSelectedItem(pos);
		if (0 != nCount)
			sResult += _T("\r\n");

		for (int i = 0; i < nColumn; ++i)
		{
			sResult += pListCtrl->GetItemText(nItem, i);
			if (i != nColumn - 1)
				sResult += lpszSeparator;
		}
		++nCount;
	}

	if (pListCtrl->OpenClipboard())
	{
		EmptyClipboard();

		int nLen = (sResult.GetLength() + 1) * sizeof(WCHAR);
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLen);
		LPBYTE pGlobalData = (LPBYTE)GlobalLock(hGlobal);

		USES_CONVERSION_EX;
		CopyMemory(pGlobalData, T2CW_EX(sResult, _ATL_SAFE_ALLOCA_DEF_THRESHOLD), nLen);
		SetClipboardData(CF_UNICODETEXT, hGlobal);

		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);

		CloseClipboard();
		return TRUE;
	}
	return FALSE;
}

/*���� ���� ���� ���� ip�ּҰ� �޾ƿ���*/
CString CMFCApplication4Dlg::GetLocalIP(void)
{
	WSADATA wsaData;
	char name[255];
	CString ip; // ip ����.
	PHOSTENT hostinfo;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)
	{
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				ip = inet_ntoa(*(struct in_addr*) * hostinfo->h_addr_list);
			}
		}
		WSACleanup();
	}
	return ip;
}


/* EDIT BOX �� �ִ� TEXT�� �����ؼ� �������� ���� ����*/
int ClipBoard(char* source)
{
	int ok = OpenClipboard(NULL);
	if (!ok) return 0;
	HGLOBAL clipbuffer;
	char* buffer;
	EmptyClipboard();
	clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(source) + 1);
	buffer = (char*)GlobalLock(clipbuffer);
	strcpy(buffer, source);
	GlobalUnlock(clipbuffer);
	SetClipboardData(CF_TEXT, clipbuffer);
	CloseClipboard();
	return 1;
}

//�����Լ�2
void SetClipboardText(CString strSource)
{
	//put your text in source
	if (::OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
		char* buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, strSource.GetLength() + 1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, LPCSTR(strSource));
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
	}
}

/* ��Ȱ�� ��忡���� �����ÿ� ������Ŷ ����*/
void PostMessageSend(HWND hMP, WPARAM wParam, LPARAM lParam)
{
	PostMessage(hMP, WM_KEYDOWN, wParam, lParam);
}


/* ����Ŭ�� */
void Click(int Count, int x, int y)
{
	SetCursorPos(x, y);
	for (int index = 0; index <= Count; index++)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
}


//�ε�
void LOGWRITE(char* log2)
{
	m_hWnd = FindWindow(NULL, "Mushroom");
	SendMessage(GetDlgItem(m_hWnd, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)log2);
	SendMessage(GetDlgItem(m_hWnd, IDC_LIST1), LB_SETCURSEL, SendMessage(GetDlgItem(m_hWnd, IDC_LIST1), LB_GETCOUNT, 0, 0) - 1, 0);
}

//Ÿ�ӷα�
void TIMELOGWRITE(char* log2)
{
	m_hWnd = FindWindow(NULL, "Mushroom");
	GetLocalTime(&st);
	sprintf(Buffer, "[%d:%d:%d] %s", st.wHour, st.wMinute, st.wSecond, log2);
	LOGWRITE(Buffer);
	SendMessage(GetDlgItem(m_hWnd, IDC_LIST1), LB_SETCURSEL, SendMessage(GetDlgItem(m_hWnd, IDC_LIST1), LB_GETCOUNT, 0, 0) - 1, 0);
}

void copy() {
	m_hWnd = FindWindow(NULL, "Mushroom");

	CString    csTemp;
	char* pszTemp = new char[csTemp.GetLength() + 1];

	strcpy(pszTemp, csTemp);

	GetDlgItemText(m_hWnd, IDC_EDIT3, pszTemp, 1009);
	SetClipboardText(pszTemp);
	delete[] pszTemp;
}


void ran() {

	//WinExec("netsh interface ipv4 set subinterface ""�̴���"" mtu=20 store=persisten", SW_HIDE);
	WinExec("C:\\��off", SW_HIDE);
	Sleep(1000);
	keybd_event(VK_ESCAPE, 0, 0, 0);
	keybd_event(VK_ESCAPE, 0, 2, 0);
	Sleep(50);
	storename();
	Sleep(50);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);
	Sleep(50);
	WinExec("C:\\��on", SW_HIDE);
	getitem();
}

/*�ϻ� ���� �������� */
void storename() {
	//ĳ��â�̵�
	SetCursorPos(783, 64);
	//ĳ���� Ŭ��
	Sleep(200);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // ���콺 ���� ��ư�� �����ϴ�.
	Sleep(200);
	//�ϻ� �̵�
	SetCursorPos(663, 97);
	//�ϻ����Ŭ��
	Sleep(200);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // ���콺 ���� ��ư�� �����ϴ�.
	Sleep(200);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(200);

	//���� v
	keybd_event(0xA2, 0, 0, 0);
	keybd_event(0x56, 0, 0, 0);
	Sleep(20);
	keybd_event(0x56, 0, 2, 0);
	keybd_event(0xA2, 0, 2, 0);
	Sleep(20);


}

/*��� ���� ��������  */
void storename2() {

	//ĳ��â�̵�
	SetCursorPos(787, 64);
	//ĳ���� Ŭ��
	Sleep(200);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // ���콺 ���� ��ư�� �����ϴ�.
	Sleep(200);
	//��� �̵�
	SetCursorPos(706, 95);
	//������Ŭ��
	Sleep(200);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // ���콺 ���� ��ư�� �����ϴ�.
	Sleep(200);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(200);

	//���� v
	keybd_event(0xA2, 0, 0, 0);
	keybd_event(0x56, 0, 0, 0);
	Sleep(20);
	keybd_event(0x56, 0, 2, 0);
	keybd_event(0xA2, 0, 2, 0);
	Sleep(20);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);
}

void storename3() {


	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // ���콺 ���� ��ư�� �����ϴ�.
	Sleep(100);
	//�ϻ� �̵�
	SetCursorPos(663, 97);
	//�ϻ����Ŭ��
	Sleep(100);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // ���콺 ���� ��ư�� �����ϴ�.
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(100);

	//���� v
	keybd_event(0xA2, 0, 0, 0);
	keybd_event(0x56, 0, 0, 0);
	Sleep(20);
	keybd_event(0x56, 0, 2, 0);
	keybd_event(0xA2, 0, 2, 0);
	Sleep(20);


}

/*������ â Ȱ��ȭ */
void OffWindowLine()
{
	//S_CheckID();//���ȸ��

	hMP = FindWindow(NULL, "MapleStory");
	Sleep(1);
	SetWindowPos(hMP, 0, 0, 0, 800, 600, SWP_NOSIZE);
	Sleep(1);
	SetWindowPos(hMP, 0, 0, 0, 800, 600, SWP_NOSIZE);
}


/* ���� ���� Ȯ�� ������ ������ �ø���*/
void getitem() {

	Sleep(delay);
	//���âŬ��
	SetCursorPos(666, 66);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // ���콺 ���� ��ư�� �����ϴ�.
	Sleep(50);
	//����� Ŭ��
	SetCursorPos(666, 105);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(50);
	//�ۿø��°����� �̵�
	SetCursorPos(187, 294);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	//99999��
	Sleep(100);


	copy();
	keybd_event(0xA2, 0, 0, 0);
	keybd_event(0x56, 0, 0, 0);
	Sleep(20);
	keybd_event(0x56, 0, 2, 0);
	keybd_event(0xA2, 0, 2, 0);

	Sleep(20);
	SetCursorPos(655, 68);

	Sleep(20);
	///����
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);

	Sleep(50);
	//����
	SetCursorPos(353, 145);
	Sleep(delay);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(500);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	TIMELOGWRITE(": ������������");
}

/*�Ϲݻ��� �ν� ���� �ϱ� */
void ReSetMaket()
{
	//���� ����â �ݱ�

	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);

	//ĳ��
	Sleep(30);
	SetCursorPos(784, 60);
	Sleep(30);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(30);

	//ĳ���� ����
	SetCursorPos(655, 102);
	Sleep(10);

	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(10);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(10);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(10);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	Sleep(30);
	//���� ���⿩��
	keybd_event(0xA2, 0, 0, 0);
	keybd_event(0x56, 0, 0, 0);
	Sleep(20);
	keybd_event(0x56, 0, 2, 0);
	keybd_event(0xA2, 0, 2, 0);
	Sleep(20);

	Sleep(200);
	TIMELOGWRITE("  :  �ϻ����⼺��");
}


/* ������ �νĽ� ���� */
void ReSetMaket2() {


	//���� ����â �ݱ�
	Sleep(300);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);

	//ĳ��
	Sleep(200);
	SetCursorPos(785, 61);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(50);

	//ĳ���� ����
	SetCursorPos(692, 92);
	Sleep(100);

	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(10);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(10);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(10);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	Sleep(500);
	keybd_event(0xA2, 0, 0, 0);
	keybd_event(0x56, 0, 0, 0);
	Sleep(20);
	keybd_event(0x56, 0, 2, 0);
	keybd_event(0xA2, 0, 2, 0);
	Sleep(20);

	Sleep(500);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);
	TIMELOGWRITE(": ������⼺��");

}

/* ����*/
void ReSetStore() {

	OffWindowLine();
	SetForegroundWindow(hMP);
	//�����ݱ�
	SetCursorPos(344, 164);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	//�������ðٽ��ϱ�?
	Sleep(100);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);
	//�����۰� �޼� ���ã�ѽ��ϴ�
	Sleep(300);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);
	//�����۸޼� ã�°� ����


	//ĳ��â�̵�
	SetCursorPos(787, 64);
	//ĳ���� Ŭ��
	Sleep(200);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // ���콺 ���� ��ư�� �����ϴ�.
	Sleep(200);
	//��� �̵�
	SetCursorPos(706, 95);
	//������Ŭ��
	Sleep(200);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // ���콺 ���� ��ư�� �����ϴ�.
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(200);

	//���� edit�� ����
	keybd_event(0xA2, 0, 0, 0);
	keybd_event(0x56, 0, 0, 0);
	Sleep(20);
	keybd_event(0x56, 0, 2, 0);
	keybd_event(0xA2, 0, 2, 0);
	Sleep(20);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);

	Sleep(100);
	//���âŬ��
	SetCursorPos(666, 66);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // ���콺 ���� ��ư�� �����ϴ�.
	Sleep(50);
	//����� Ŭ��
	SetCursorPos(666, 105);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(50);
	//�ۿø��°����� �̵�
	SetCursorPos(187, 294);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	//99999��
	Sleep(50);

	copy();
	keybd_event(0xA2, 0, 0, 0);
	keybd_event(0x56, 0, 0, 0);
	Sleep(20);
	keybd_event(0x56, 0, 2, 0);
	keybd_event(0xA2, 0, 2, 0);
	SetCursorPos(655, 68);

	Sleep(20);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);

	Sleep(50);
	//����
	SetCursorPos(353, 145);
	Sleep(300);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(200);
	TIMELOGWRITE("  :  ����Ϸ�");
}



void GetPixelMP(int x, int y, HWND hMP)
{
	HDC hdc = GetDC(hMP);
	color = GetPixel(hdc, x, y);
}

/*��Ŷ�� ã�� ���� ������ ������ ��Ҵ��� üũ*/
bool Isok()
{
	Sleep(500);
	TIMELOGWRITE(": ������������Ȯ����");

	HDC hdc;
	COLORREF c_back1, c_back2, c_back3, c_back4;
	hdc = GetDC(NULL);
	CWindowDC dc(NULL);

	SetForegroundWindow(hWnd);
	Sleep(200);
	c_back1 = GetPixel(hdc, 500, 220);
	Sleep(1);
	c_back2 = GetPixel(hdc, 500, 250);
	Sleep(1);
	c_back3 = GetPixel(hdc, 500, 270);
	Sleep(1);
	c_back4 = GetPixel(hdc, 500, 300);

	SetForegroundWindow(hMP);
	if ((c_back1 == 16777215) && (c_back2 == 16777215) && (c_back3 == 16777215)) {//&& (c_back4 == 16777215)) {
		return 1;
	}// && (c_back3 == 16777215) && (c_back4 == 16777215)) return 1;

	return 0;


}

/*��Ȱ�� ���*/
void fullmode() {

	hMP = FindWindow(NULL, "MapleStory");
	//SetWindowPos(hMP, 0, 0, 0, 800, 600, SW_MAXIMIZE);
	Sleep(500);
	CloseWindow(hMP);
	SetWindowPos(hMP, 0, 0, 0, 800, 600, SW_SHOWNORMAL);

}

void ReSetMaket3() {

	Sleep(50);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);

}






