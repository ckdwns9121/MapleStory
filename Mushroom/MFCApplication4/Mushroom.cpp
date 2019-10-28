/*
	Mushroom.cpp : Made By â�� 2016 04 02
*/

#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "UrlMon.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#include "C:\WpdPack\Include\packet32.h"
#include "pcap.h"
#include "stdafx.h"
#include "resource.h"
#include "MTU.h"
#include "stdlib.h"
#include <process.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <io.h>  
#include <fcntl.h>
#include <string.h>
#include <winsock2.h>
#include <Shlwapi.h>
#include <tchar.h>
#include <mmsystem.h>
#include <ntddndis.h>
#include <psapi.h>
#include <Shlobj.h>//������ ���� Ȯ��

#define PROTO_TCP	6
#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1) 
typedef int(WINAPI *WINSTATREMINATEPROC)(HANDLE hServer, DWORD dwProcessId, UINT uExitCode);
typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;
BOOL WSWRITE = FALSE, WPWRITE = FALSE, FirstLogIN = FALSE, WARIONOFF = FALSE, SAVECHECK = FALSE, WARISTART = FALSE, ColorCheck = FALSE, MapleOnOff = TRUE;
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK LOGINDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SETTINGDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
void CALLBACK MapleStoryScreenXY(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK WARITimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK MTUTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void LOGWRITE(char* LOGTEXT);
int SignalOnOff = 0, SignalCount = 0, WARIINDEX = 1, WARIDELAY, Teleport, PixelCheck, MX, MY, MX1 = 0, MY1 = 0, WARICOUNT, GetMX, GetMY, GetMX1, GetMY1;
HINSTANCE g_hInst;
HWND g_hDlg, MainDlg, START, hMP;
char Version[256], Buffer[256], SignalMODE[256], Save[256], StoreName[256], SignalSTORE[256], SignalIP[256], SignalDevice[256], SignalOPENSTORE[256], errbuf[PCAP_ERRBUF_SIZE], AdapterName[8192] = { 0, }, *temp, *temp1, AdapterList[10][1024] = { 0, }, MapleRoot[128];
RECT rect, MapleXY, BufferXY;
pcap_if_t *alldevs;
pcap_if_t *d;
DWORD ThreadID, Serial;
COLORREF color, StoreColor;
POINT Mxy, WARI1, WARI2, WARI3, WARI4, StorePoint;
pcap_t *adhandle;
SYSTEMTIME st;

struct iphdr
{
	unsigned char	ihl : 4,
		version : 4;
	unsigned char	tos;
	unsigned short	tot_len;
	unsigned short	id;
	unsigned short	frag_off;
	unsigned char	ttl;
	unsigned char	protocol;
	unsigned short	check;
	unsigned int	saddr;
	unsigned int	daddr;
};

struct tcphdr
{
	unsigned short	source;
	unsigned short	dest;
	unsigned int	seq;
	unsigned int	ack_seq;

	unsigned short	nsf : 1,
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

	unsigned short	window;
	unsigned short	check;
	unsigned short	urg_ptr;
};

struct udphdr
{
	unsigned short	source;
	unsigned short	dest;
	unsigned short	len;
	unsigned short	check;
};

struct icmphdr
{
	unsigned char	type;
	unsigned char	code;
	unsigned short	checksum;
	union
	{
		struct
		{
			unsigned short	id;
			unsigned short	sequence;
		} echo;
		unsigned int	gateway;
		struct
		{
			unsigned short	__unused;
			unsigned short	mtu;
		} frag;
	} un;
};

unsigned int	m_SOCK;
int				m_BUFFSZ;
unsigned char*	m_BUFF;		// ��Ŷ ����
int				m_PACKETSZ; // ���ۿ� ���� ��Ŷ ũ��	
unsigned char*	m_PDATA;	// ������ ����
unsigned char*	m_PDATATXT;	// ������ ����(Text)
int				m_DATASZ;	// ������ ũ��	
int				m_PROTO;	// ��������
unsigned int	m_SRCIP;	// �۽� ip
unsigned int	m_DSTIP;	// ���� ip
int				m_SRCPORT;	// �۽� port
int				m_DSTPORT;	// ���� port
struct iphdr*	m_PIPH;		// IP ���
struct tcphdr*	m_PTCPH;	// TCP ���
struct udphdr*	m_PUDPH;	// UDP ���
struct icmphdr*	m_PICMPH;	// ICMP ���

unsigned char*	m_TXTBUFF;	// ��¿� ����
int				m_TXTSZ;

inline bool IsDbgPresentPrefixCheck()
{
	__try
	{
		__asm __emit 0xF3 // 0xF3 0x64 disassembles as PREFIX REP:
		__asm __emit 0x64
		__asm __emit 0xF1 // One byte INT 1
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

inline void PushPopSS()
{

	__asm
	{
		push ss
		pop ss
			mov eax, 9 // This line executes but is stepped over
			xor edx, edx // This is where the debugger will step to
	}
}

inline bool Int2DCheck()
{
	__try
	{
		__asm
		{
			int 0x2d
			xor eax, eax
				add eax, 2
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

inline bool DebugObjectCheck()
{
	// Much easier in ASM but C/C++ looks so much better
	typedef NTSTATUS(WINAPI *pNtQueryInformationProcess)
		(HANDLE, UINT, PVOID, ULONG, PULONG);

	HANDLE hDebugObject = NULL;
	NTSTATUS Status;

	// Get NtQueryInformationProcess
	pNtQueryInformationProcess NtQIP = (pNtQueryInformationProcess)
		GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")),
			"NtQueryInformationProcess");

	Status = NtQIP(GetCurrentProcess(),
		0x1e, // ProcessDebugObjectHandle
		&hDebugObject, 4, NULL);

	if (Status != 0x00000000)
		return false;

	if (hDebugObject)
		return true;
	else
		return false;
}

inline bool CheckProcessDebugFlags()
{
	// Much easier in ASM but C/C++ looks so much better
	typedef NTSTATUS(WINAPI *pNtQueryInformationProcess)
		(HANDLE, UINT, PVOID, ULONG, PULONG);

	DWORD NoDebugInherit = 0;
	NTSTATUS Status;

	// Get NtQueryInformationProcess
	pNtQueryInformationProcess NtQIP = (pNtQueryInformationProcess)
		GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")),
			"NtQueryInformationProcess");

	Status = NtQIP(GetCurrentProcess(),
		0x1f, // ProcessDebugFlags
		&NoDebugInherit, 4, NULL);

	if (Status != 0x00000000)
		return false;

	if (NoDebugInherit == FALSE)
		return true;
	else
		return false;
}

bool MemoryBreakpointDebuggerCheck()
{
	unsigned char *pMem = NULL;
	SYSTEM_INFO sysinfo = { 0 };
	DWORD OldProtect = 0;
	void *pAllocation = NULL; // Get the page size for the system 

	GetSystemInfo(&sysinfo); // Allocate memory 

	pAllocation = VirtualAlloc(NULL, sysinfo.dwPageSize,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE);

	if (pAllocation == NULL)
		return false;

	// Write a ret to the buffer (opcode 0xc3)
	pMem = (unsigned char*)pAllocation;
	*pMem = 0xc3;

	// Make the page a guard page         
	if (VirtualProtect(pAllocation, sysinfo.dwPageSize,
		PAGE_EXECUTE_READWRITE | PAGE_GUARD,
		&OldProtect) == 0)
	{
		return false;
	}

	__try
	{
		__asm
		{
			mov eax, pAllocation
			// This is the address we'll return to if we're under a debugger
				push MemBpBeingDebugged
				jmp eax // Exception or execution, which shall it be :D?
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		// The exception occured and no debugger was detected
		VirtualFree(pAllocation, NULL, MEM_RELEASE);
		return false;
	}

	__asm {MemBpBeingDebugged:}
	VirtualFree(pAllocation, NULL, MEM_RELEASE);
	return true;
}

BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;

	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			//handle error
		}
	}
	return bIsWow64;
}

bool AnotherInstance()
{
	HANDLE ourMutex;// ���ؽ� ���� �õ�
	ourMutex = CreateMutex(NULL, true, "TopSevenSignal");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{//�̹� ���� �̸����� ���ý��� �����Ǿ��� ���
		HWND BufferHWND;
		BufferHWND = FindWindow(NULL, "TopSeven Signal");
		PostMessage(BufferHWND, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
		SetForegroundWindow(BufferHWND);
		return true;
	}
	return false;
}

BOOL MapleStoryReg()
{
	DWORD dwType = REG_SZ;
	DWORD dwSize = 128;
	HKEY hKey;
	LONG lResult;
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Wizet\\Maple", 0, KEY_READ, &hKey);//�����ý��丮 �ּҰ� ����
	if (lResult == ERROR_SUCCESS)
	{
		RegQueryValueEx(hKey, "RootPath", NULL, &dwType, (LPBYTE)MapleRoot, &dwSize);//�ּҰ��� MapleRoot�� ����
		sprintf(Buffer, "������ ��ġ : %s", MapleRoot);
		LOGWRITE(Buffer);//������ �ּҰ��� �α�â�� �Ѹ�
		for (int index = 0; index <= 255; index++)
		{
			if (MapleRoot[index] == '\\')
			{//�ּҸ� �����ּҰ��� �ٲٱ� ���� \ �� \\�� �ٲ�
				for (int i = 255; i > index; i--)
					MapleRoot[i] = MapleRoot[i - 1];
				MapleRoot[index] = '\\';
				index++;
			}
			if (Buffer[index] == NULL)// �迭�� �д��� ���� ���� �迭�� ������� While�� ����
				break;
			index++;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void HOTKEY(HWND hDlg)
{//��Ű ����

	RegisterHotKey(hDlg, 1, MOD_NOREPEAT, VK_F1);
	//RegisterHotKey(hDlg, 2, MOD_NOREPEAT, VK_F2);
	RegisterHotKey(hDlg, 3, MOD_NOREPEAT, VK_F3);
	RegisterHotKey(hDlg, 4, MOD_NOREPEAT, VK_F4);
	//RegisterHotKey(hDlg, 5, MOD_NOREPEAT, VK_F5);
	//RegisterHotKey(hDlg, 6, MOD_NOREPEAT, VK_F6);
	RegisterHotKey(hDlg, 9, MOD_NOREPEAT, VK_F9);
	RegisterHotKey(hDlg, 10, MOD_NOREPEAT, VK_F10);
	RegisterHotKey(hDlg, 11, MOD_NOREPEAT, VK_F11);
}

void CLOSEHOTKEY(HWND hDlg)
{//��Ű����
	UnregisterHotKey(hDlg, 1);
	//UnregisterHotKey(hDlg, 2);
	UnregisterHotKey(hDlg, 3);
	UnregisterHotKey(hDlg, 4);
	//UnregisterHotKey(hDlg, 5);
	//UnregisterHotKey(hDlg, 6);
	UnregisterHotKey(hDlg, 9);
	UnregisterHotKey(hDlg, 10);
	UnregisterHotKey(hDlg, 11);
}



void VersionCheck()
{
	GetModuleFileName(NULL, Buffer, 256);
	PathRemoveFileSpec(Buffer);
	sprintf(Buffer, "%s\\Version.ini", Buffer);
	URLDownloadToFile(NULL, "http://kts6056.dothome.co.kr/TopSeven/TopSevenSignal/version/version.ini", Buffer, NULL, NULL);
	GetPrivateProfileString("TopSeven", "Version", NULL, Version, sizeof(Version), Buffer);
	remove(Buffer);
	if (!strcmp(Version, "1.2.1"))
	{
		ZeroMemory(Buffer, sizeof(Buffer));
		ZeroMemory(Buffer, sizeof(Version));
	}
	else
	{
		ZeroMemory(Buffer, sizeof(Buffer));
		ZeroMemory(Buffer, sizeof(Version));
		MessageBox(MainDlg, TEXT("���ο� ���� �߰�."), TEXT("TopSeven"), 16);
		ExitProcess(0);
	}
}

void ServerONOFF()
{
	URLDownloadToFile(NULL, "http://kts6056.dothome.co.kr/TopSeven/TopSevenSignal/ONOFF/Check.txt", "1.txt", NULL, NULL);
	FILE *f;
	f = fopen("1.txt", "r");
	sprintf(Buffer, "%d", fgetc(f));
	ZeroMemory(f, sizeof(f));
	fclose(f);
	remove("1.txt");
	if (strcmp(Buffer, "49"))
	{
		MessageBox(MainDlg, TEXT("����[OFF]."), TEXT("TopSeven"), 16);
		ExitProcess(0);
	}
}

void LOGWRITE(char* LOGTEXT)
{
	SendMessage(GetDlgItem(MainDlg, LOG), LB_ADDSTRING, NULL, (LPARAM)LOGTEXT);
	SendMessage(GetDlgItem(MainDlg, LOG), LB_SETCURSEL, SendMessage(GetDlgItem(MainDlg, LOG), LB_GETCOUNT, 0, 0) - 1, 0);
}

void TIMELOGWRITE(char* LOGTEXT)
{
	GetLocalTime(&st);
	sprintf(Buffer, "%d��%d��%d��%s", st.wHour, st.wMinute, st.wSecond, LOGTEXT);
	LOGWRITE(Buffer);
	SendMessage(GetDlgItem(MainDlg, LOG), LB_SETCURSEL, SendMessage(GetDlgItem(MainDlg, LOG), LB_GETCOUNT, 0, 0) - 1, 0);
}

void HddSerialLogin()
{
	char szDriveName[32], szFileSystem[16], DownUrl[128];
	GetVolumeInformation("c:\\", szDriveName, 32, &Serial, NULL, NULL, szFileSystem, 16);
	sprintf(DownUrl, "%s%X.txt", "http://kts6056.dothome.co.kr/TopSevenSignal/LOGIN/", Serial);
	URLDownloadToFile(NULL, DownUrl, "1.txt", NULL, NULL);
	FILE *f;
	f = fopen("1.txt", "r");
	sprintf(Buffer, "%#X", fgetc(f));
	if (strcmp(Buffer, "0X3C"))
	{
		ZeroMemory(f, sizeof(f));
		fclose(f);
		remove("1.txt");
		sprintf(Buffer, "�ڵ� : %X", Serial);
		MessageBox(MainDlg, Buffer, TEXT("TopSeven"), 16);
		return;
		//DialogBox(g_hInst, MAKEINTRESOURCE(MAIN), HWND_DESKTOP, MainDlgProc);
	}
	else
	{
		ZeroMemory(f, sizeof(f));
		fclose(f);
		remove("1.txt");
		ZeroMemory(Buffer, sizeof(Buffer));
		DialogBox(g_hInst, MAKEINTRESOURCE(MAIN), HWND_DESKTOP, MainDlgProc);
	}
}

int ClipBoard(char *source)
{
	int ok = OpenClipboard(NULL);
	if (!ok) return 0;
	HGLOBAL clipbuffer;
	char * buffer;
	EmptyClipboard();
	clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(source) + 1);
	buffer = (char*)GlobalLock(clipbuffer);
	strcpy(buffer, source);
	GlobalUnlock(clipbuffer);
	SetClipboardData(CF_TEXT, clipbuffer);
	CloseClipboard();
	return 1;
}

void Click(int Count, int x, int y)
{
	SetCursorPos(x, y);
	for (int index = 0; index <= Count; index++)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
}

void PostMessageSend(HWND hMP, WPARAM wParam, LPARAM lParam)
{
	PostMessage(hMP, WM_KEYDOWN, wParam, lParam);
}

BOOL GetMapleScreenSize()
{
	hMP = FindWindow("MapleStoryClass", NULL);
	if (hMP == NULL)
	{
		if (MapleOnOff == TRUE)
		{
			LOGWRITE("�������� �Ⱥ�����.");
			MX1 = 0, MX = 0, MY1 = 0, MY = 0;
			MapleOnOff = FALSE;
		}
		return FALSE;
	}
	else
	{
		MapleOnOff = TRUE;
		GetClientRect(hMP, &rect);
		MX = rect.right - rect.left;
		MY = rect.bottom - rect.top;
		if (MX != MX1 || MY != MY1)
		{
			MX1 = MX;
			MY1 = MY;
			if (MX1 != 0 && MY1 != 0)
			{
				SetWindowPos(MainDlg, HWND_TOP, GetMX1 + MX1, GetMY1, 0, 0, SWP_NOSIZE);
				sprintf(Buffer, "�ػ� : %d x %d", MX1, MY1);
				LOGWRITE(Buffer);
			}
			else
			{
				GetWindowRect(MainDlg, &rect);
				SetWindowPos(MainDlg, HWND_TOP, GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left), 0, 0, 0, SWP_NOSIZE);
				LOGWRITE("������â ��Ȱ��ȭ��.");
			}
		}
		GetWindowRect(hMP, &rect);
		GetMX = rect.left;
		GetMY = rect.top;
		if (GetMX != GetMX1 || GetMY != GetMY1)
		{
			GetMX1 = GetMX;
			GetMY1 = GetMY;
			if (MX1 != 0 && MY1 != 0)
			{
				SetWindowPos(MainDlg, HWND_TOP, GetMX1 + MX1, GetMY1, 0, 0, SWP_NOSIZE);
			}
			else
			{
				GetWindowRect(MainDlg, &rect);
				SetWindowPos(MainDlg, HWND_TOP, GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left), 0, 0, 0, SWP_NOSIZE);
			}
		}
		return TRUE;
	}
}

void MapleSotryActive(HWND hMP)
{
	SetWindowPos(hMP, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);
	SetForegroundWindow(hMP);
	Sleep(1);
}

void GetPixelMP(int x, int y, HWND hMP)
{
	HDC hdc = GetDC(hMP);
	color = GetPixel(hdc, x, y);
}

void TeleportUP()
{
	TIMELOGWRITE("�� ��� ������");
	keybd_event(VK_UP, 0, 0, 0);
	Sleep(100);
	keybd_event(VK_F2, MapVirtualKey(VK_F2, MAPVK_VK_TO_VSC), 0, 0);
	Sleep(100);
	keybd_event(VK_F2, MapVirtualKey(VK_F2, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
	Sleep(100);
	keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
}

void TeleportRIGHT()
{
	TIMELOGWRITE("�� ���� ������");
	keybd_event(VK_RIGHT, 0, 0, 0);
	Sleep(100);
	keybd_event(VK_F2, MapVirtualKey(VK_F2, MAPVK_VK_TO_VSC), 0, 0);
	Sleep(100);
	keybd_event(VK_F2, MapVirtualKey(VK_F2, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
	Sleep(100);
	keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
}

void TeleportDOWN()
{
	TIMELOGWRITE("�� ��� ������");
	keybd_event(VK_DOWN, 0, 0, 0);
	Sleep(100);
	keybd_event(VK_F2, MapVirtualKey(VK_F2, MAPVK_VK_TO_VSC), 0, 0);
	Sleep(100);
	keybd_event(VK_F2, MapVirtualKey(VK_F2, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
	Sleep(100);
	keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
}

void TeleportLEFT()
{
	TIMELOGWRITE("�� ���� ������");
	keybd_event(VK_LEFT, 0, 0, 0);
	Sleep(100);
	keybd_event(VK_F2, MapVirtualKey(VK_F2, MAPVK_VK_TO_VSC), 0, 0);
	Sleep(100);
	keybd_event(VK_F2, MapVirtualKey(VK_F2, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
	Sleep(100);
	keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
}

void MapleIven(HWND hMP)
{
	//GetPixelMP(742, 319, hMP);
	GetPixelMP(MX1 - 58, 319, hMP);
	if (color != 0xDDAA44 && color != 0xCC9933)
	{
		Click(0, 33, 100);
		Sleep(150);
		PostMessageSend(hMP, 73, 1507329);
		Sleep(150);
	}
}

void StorWait(HWND hMP)
{
	if (MY1 == 600)
	{
		for (int Index = 0; Index <= 100; Index++)
		{
			//GetPixelMP(357, 355, hMP);
			GetPixelMP((MX1 / 2) - 50, 355, hMP);
			if (color == 0xEEEEEE)
			{
				break;
			}
			else
			{
				Sleep(10);
			}
		}
	}
	else
	{
		for (int Index = 0; Index <= 100; Index++)
		{
			//GetPixelMP(357, 355, hMP);
			GetPixelMP((MX1 / 2) - 50, 439, hMP);
			if (color == 0xEEEEEE)
			{
				break;
			}
			else
			{
				Sleep(10);
			}
		}
	}
}

void ClickStore0(HWND hMP)
{
	Click(1, MX1 - 26, 60);
	Sleep(150);
	Click(3, MX1 - 150, 85);
	StorWait(hMP);
}

void ClickStore1(HWND hMP)
{
	Click(1, MX1 - 26, 60);
	Sleep(150);
	Click(3, MX1 - 110, 85);
	StorWait(hMP);
}

void TitleSend()
{
	hMP = FindWindow("MapleStoryClass", NULL);
	if (!strcmp(StoreName, ""))
	{
		ClipBoard("��  TopSeven��  ");
	}
	else
	{
		//ClipBoard("��  TopSeven��  ");
		ClipBoard(StoreName);
	}
	keybd_event(0xA2, 0, 0, 0);
	keybd_event(0x56, 0, 0, 0);
	Sleep(20);
	keybd_event(0x56, 0, 2, 0);
	keybd_event(0xA2, 0, 2, 0);
	Sleep(20);
}

void ClickStore()
{
	if (SignalCount != 0)
	{
		PlaySound((LPCSTR)MAKEINTRESOURCE(ReStoreWave), NULL, SND_ASYNC | SND_RESOURCE);
	}
	if (!strcmp(SignalOPENSTORE, "�Ϲݻ���"))
	{
		ClickStore0(hMP);
		TitleSend();
	}
	if (!strcmp(SignalOPENSTORE, "������"))
	{
		ClickStore1(hMP);
		TitleSend();
		if (Teleport == -1)
		{
			PostMessageSend(hMP, 13, 1835009);
		}
	}
	if (Teleport != -1)
	{
		if (SignalCount != 0)
		{
			if (Teleport == 0)
			{
				TeleportDOWN();
			}
			if (Teleport == 1)
			{
				TeleportLEFT();
			}
			if (Teleport == 2)
			{
				TeleportUP();
			}
			if (Teleport == 3)
			{
				TeleportRIGHT();
			}
		}
	}
	if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
	{
		PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_MINIMIZE, 0);
	}
}

BOOL StoreOpenCheck(HWND hMP)
{
	for (int Index = 0; Index <= 100; Index++)
	{
		GetPixelMP((MX1 / 2) + 50, 450, hMP);
		if (color == 0xFFFFFF)
		{
			GetPixelMP((MX1 / 2) + 150, 450, hMP);
			if (color == 0xFFFFFF)
			{
				if (WARISTART == TRUE)
				{
					PlaySound((LPCSTR)MAKEINTRESOURCE(WARIWave), NULL, SND_ASYNC | SND_RESOURCE);
				}
				else
				{
					PlaySound((LPCSTR)MAKEINTRESOURCE(OpenWave), NULL, SND_ASYNC | SND_RESOURCE);
				}
				return TRUE;
			}
		}
		else
		{
			Sleep(5);
		}
	}
	return FALSE;
	PostMessageSend(hMP, 13, 1835009);
}

void OpenStore0(HWND hMP)
{
	Click(1, MX1 - 140, 60);
	Sleep(50);
	for (int count = 1; count <= 3; count++)
	{
		Click(0, MX1 - 80, 85);
		Sleep(25);
		Click(0, MX1 / 2, 350);
		Sleep(25);
		for (int index = 0; index <= 9; index++)
		{
			PostMessageSend(hMP, 57, 655361);
		}
		Sleep(25);
		PostMessageSend(hMP, 13, 1835009);
		Click(0, MX1 - 45, 85);
		Sleep(25);
		Click(0, MX1 / 2, 350);
		Sleep(25);
		for (int index = 0; index <= 9; index++)
		{
			PostMessageSend(hMP, 57, 655361);
		}
		Sleep(25);
		PostMessageSend(hMP, 13, 1835009);
	}
	for (int index = 1; index <= 8; index++)
	{
		Sleep(50);
		if (MY1 == 600)
		{
			Click(5, (MX / 2) - 80, 140);
		}
		else
		{
			Click(5, (MX / 2) - 80, 224);
		}
		PostMessageSend(hMP, 13, 1835009);
	}
}

void ReStore0(HWND hMP, HWND hDlg)
{
	PostMessageSend(hMP, 13, 1835009);
	if (MY1 == 600)
	{
		Click(3, (MX1 / 2) - 15, 565);
	}
	else
	{
		Click(3, (MX1 / 2) - 15, 649);
	}
	Sleep(100);
	Click(3, MX1 - 30, 60);
	Sleep(100);
	SetCursorPos(MX1 - 137, 90);
	Sleep(50);
	keybd_event(VK_ESCAPE, 0, 0, 0);
	keybd_event(VK_ESCAPE, 0, 2, 0);
	Click(3, MX1 - 137, 90);
	keybd_event(0x37, 0, 0, 0);
	keybd_event(0x37, 0, 2, 0);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 2, 0);
	Sleep(150);
	if (StoreOpenCheck(hMP))
	{
		Sleep(50);
		OpenStore0(hMP);
		LOGWRITE("���� ����!");
	}
	else
	{
		LOGWRITE("���� ����!");
	}
}

void ReStore1(HWND hMP, HWND hDlg)
{
	int index1 = 0;
	Click(3, MX1 - 26, 60);
	Sleep(100);
	if (MX1 == 600)
	{
		Click(1, (MX1 / 2) - 84, 160);
	}
	else
	{
		Click(1, (MX1 / 2) - 84, 244);
	}
	Sleep(100);
	do
	{
		if (index1 >= 10)
		{
			break;
		}
		for (int index = 0; index <= 5; index++)
		{
			PostMessageSend(hMP, 13, 1835009);
			Sleep(15);
		}
		GetPixelMP((MX1 / 2) + 50, 450, hMP);
		index1++;
	} while (color == 0xFFFFFF);
	Sleep(100);
	index1 = 0;
	do
	{
		if (index1 >= 3)
		{
			LOGWRITE("��� ����!");
			break;
		}
		ClickStore1(hMP);
		TitleSend();
		for (int index = 0; index <= 5; index++)
		{
			PostMessageSend(hMP, 13, 1835009);
		}
		if (StoreOpenCheck(hMP))
		{
			Sleep(50);
			OpenStore0(hMP);
			LOGWRITE("��� ����!");
		}
		index1++;
	} while (StoreOpenCheck(hMP) != TRUE);
}

DWORD WINAPI NPFStart(LPVOID Param)
{
	hMP = FindWindow("MapleStoryClass", NULL);
	if (hMP == NULL)
	{
		MessageBox(MainDlg, TEXT("�������� �Ⱥ�����."), TEXT("TopSeven"), 16);
		return 0;
	}
	SignalCount = 0;
	sprintf(Buffer, "%d", SignalCount);
	SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
	struct bpf_stat stat;
	register LPADAPTER lpAdapter = 0;
	register LPPACKET lpPacket;
	char buffer[256000];
	//����
	int        i;
	//ascii strings
	char		AdapterName[8192]; // string that contains a list of the network adapters
	char		*temp, *temp1;
	int			AdapterNum = 0;
	ULONG		AdapterLength;
	//
	// Obtain the name of the adapters installed on this machine
	//
	printf("Packet.dll test application. Library version:%s\n", PacketGetVersion());
	printf("Adapters installed:\n");
	i = 0;
	AdapterLength = sizeof(AdapterName);
	if (PacketGetAdapterNames(AdapterName, &AdapterLength) == FALSE)
	{
		printf("Unable to retrieve the list of the adapters!\n");
		return -1;
	}
	temp = AdapterName;
	temp1 = AdapterName;
	while ((*temp != '\0') || (*(temp - 1) != '\0'))
	{
		if (*temp == '\0')
		{
			memcpy(AdapterList[i], temp1, temp - temp1);
			temp1 = temp + 1;
			i++;
		}
		temp++;
	}
	AdapterNum = i;
	int NPFDevice = atoi(SignalDevice);
	lpAdapter = PacketOpenAdapter(AdapterList[NPFDevice - 1]);
	if (!lpAdapter || (lpAdapter->hFile == INVALID_HANDLE_VALUE))
	{
		MessageBox(MainDlg, "����� ���� ���� : 1", "!", MB_ICONSTOP | MB_TASKMODAL);
		return -1;
	}
	if (PacketSetHwFilter(lpAdapter, NDIS_PACKET_TYPE_PROMISCUOUS) == FALSE)
	{
		MessageBox(MainDlg, "promiscuous ��� ���� : 2", "!", MB_ICONSTOP | MB_TASKMODAL);
		return -1;
	}
	if (PacketSetBuff(lpAdapter, 512000) == FALSE)
	{
		MessageBox(MainDlg, "Ŀ�� ���� ���� ���� : 3", "!", MB_ICONSTOP | MB_TASKMODAL);
		return -1;
	}
	if (PacketSetReadTimeout(lpAdapter, 1000) == FALSE)
	{
		MessageBox(MainDlg, "�ǵ� �ð� ���� ���� : 4", "!", MB_ICONSTOP | MB_TASKMODAL);
		return -1;
	}
	if ((lpPacket = PacketAllocatePacket()) == NULL)
	{
		MessageBox(MainDlg, "��Ŷ ����ü �Ҵ� ���� : 5", "!", MB_ICONSTOP | MB_TASKMODAL);
		return (-1);
	}
	PacketInitPacket(lpPacket, (char*)buffer, 256000);
	register struct bpf_hdr * Packet;
	MapleSotryActive(hMP);
	MapleIven(hMP);
	if (Teleport == -1)
	{
		ClickStore();
	}
	else
	{
		if (!strcmp(SignalOPENSTORE, "�Ϲݻ���"))
		{
			ClickStore0(hMP);
			TitleSend();
		}
		if (!strcmp(SignalOPENSTORE, "������"))
		{
			ClickStore1(hMP);
			TitleSend();
		}
	}
	PixelCheck = 0;
	int count = 0;
	do
	{
		PacketReceivePacket(lpAdapter, lpPacket, TRUE);
		Packet = (struct bpf_hdr *)lpPacket->Buffer;
		if (Packet->bh_datalen == 65)
		{
			if (SendMessage(GetDlgItem(MainDlg, SignalColor_Check), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
			{
				TIMELOGWRITE("�� ���ν� ����.");
				PixelCheck = 0;
				ColorCheck = TRUE;
				for (int Index = 0; Index <= 1500; Index++)
				{
					GetPixelMP(StorePoint.x, StorePoint.y, hMP);
					if (color != StoreColor)
					{
						TIMELOGWRITE("�� �� Ȯ�ε�.");
						PixelCheck = 1;
						ColorCheck = FALSE;
						break;
					}
					if (Index >= 1500)
					{
						TIMELOGWRITE("�� ��ȣ�� ������ ���� ���� ����.");
						ColorCheck = FALSE;
					}
					Sleep(1);
				}
			}
			else
			{
				PixelCheck = 1;
			}
			if (PixelCheck == 1)
			{
				if (Teleport != -1)
				{
					if (Teleport == 0)
					{
						TeleportUP();
					}
					if (Teleport == 1)
					{
						TeleportRIGHT();
					}
					if (Teleport == 2)
					{
						TeleportDOWN();
					}
					if (Teleport == 3)
					{
						TeleportLEFT();
					}
					Sleep(400);
					PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
					PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
					SignalCount++;
					sprintf(Buffer, "%d", SignalCount);
					SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
					TIMELOGWRITE("�� ������");
					if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
					{
						PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
					}
					TIMELOGWRITE("�� ��������Ȯ����");
					if (!StoreOpenCheck(hMP))
					{
						PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
						ClickStore();
						TIMELOGWRITE("�� ���� �غ���");
						if ((SignalCount % 3) == 0)
						{
							TIMELOGWRITE("�� ���Ǹ���.");
							keybd_event(VK_F1, MapVirtualKey(VK_F1, MAPVK_VK_TO_VSC), 0, 0);
							Sleep(100);
							keybd_event(VK_F1, MapVirtualKey(VK_F1, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
						}
						Sleep(400);
						TIMELOGWRITE("�� ���� ����");
					}
					else
					{
						TIMELOGWRITE("�� ��������Ȯ�� NPF����");
						LOGWRITE(Buffer);
						OpenStore0(hMP);
						SignalOnOff = 0;
						if (SendMessage(GetDlgItem(MainDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
						{
							LOGWRITE("�͸� ����.");
							KillTimer(MainDlg, 1);
							EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), TRUE);
						}
						if (SendMessage(GetDlgItem(MainDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
						{
							MTUONOFF(1500);
							LOGWRITE("MTU ����.");
							KillTimer(MainDlg, 2);
							EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), TRUE);
						}
						SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
						PacketGetStats(lpAdapter, &stat);
						PacketFreePacket(lpPacket);
						PacketCloseAdapter(lpAdapter);
						break;
					}
				}
				if (!strcmp(SignalSTORE, "������"))
				{
					count = 0;
					TIMELOGWRITE("�� �����Ŷ Ȯ����.");
					do
					{
						PacketReceivePacket(lpAdapter, lpPacket, TRUE);
						Packet = (struct bpf_hdr *)lpPacket->Buffer;
						count++;
						if (Packet->bh_datalen == 65)
						{
							count = 0;
						}
						if (count >= 10)
						{
							LOGWRITE("�����Ŷ �ƴ�");
							break;
						}
						if (count <= 10)
						{
							if (Packet->bh_datalen == 64)
							{
								PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
								PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
								TIMELOGWRITE("�� ������������");
								SignalCount++;
								sprintf(Buffer, "%d", SignalCount);
								SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
								TIMELOGWRITE("�� ������");
								if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
								{
									PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
								}
								Sleep(200);
								TIMELOGWRITE("�� ��������Ȯ����");
								if (!StoreOpenCheck(hMP))
								{
									TIMELOGWRITE("�� ���� �غ���");
									ClickStore();
									TIMELOGWRITE("�� ���� ����");
								}
								else
								{
									TIMELOGWRITE("�� ��������Ȯ�� NPF����");
									LOGWRITE(Buffer);
									OpenStore0(hMP);
									SignalOnOff = 0;
									if (SendMessage(GetDlgItem(MainDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
									{
										LOGWRITE("�͸� ����.");
										KillTimer(MainDlg, 1);
										EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), TRUE);
									}
									if (SendMessage(GetDlgItem(MainDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
									{
										MTUONOFF(1500);
										LOGWRITE("MTU ����.");
										KillTimer(MainDlg, 2);
										EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), TRUE);
									}
									SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
									PacketGetStats(lpAdapter, &stat);
									PacketFreePacket(lpPacket);
									PacketCloseAdapter(lpAdapter);
									break;
								}
							}
						}
					} while (true);
				}
				if (!strcmp(SignalSTORE, "�Ϲݻ��� + ������"))
				{
					PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
					PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
					SignalCount++;
					sprintf(Buffer, "%d", SignalCount);
					SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
					TIMELOGWRITE("�� ������");
					if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
					{
						PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
					}
					Sleep(200);
					TIMELOGWRITE("�� ��������Ȯ����");
					if (!StoreOpenCheck(hMP))
					{
						TIMELOGWRITE("�� ���� �غ���");
						ClickStore();
						TIMELOGWRITE("�� ���� ����");
					}
					else
					{
						TIMELOGWRITE("�� ��������Ȯ�� NPF����");
						LOGWRITE(Buffer);
						OpenStore0(hMP);
						SignalOnOff = 0;
						if (SendMessage(GetDlgItem(MainDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
						{
							LOGWRITE("�͸� ����.");
							KillTimer(MainDlg, 1);
							EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), TRUE);
						}
						if (SendMessage(GetDlgItem(MainDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
						{
							MTUONOFF(1500);
							LOGWRITE("MTU ����.");
							KillTimer(MainDlg, 2);
							EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), TRUE);
						}
						SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
						PacketGetStats(lpAdapter, &stat);
						PacketFreePacket(lpPacket);
						PacketCloseAdapter(lpAdapter);
						break;
					}
				}
				if (!strcmp(SignalSTORE, "Ŭ����"))
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					SignalCount++;
					sprintf(Buffer, "%d", SignalCount);
					SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
					TIMELOGWRITE("�� ������");
				}
			}
		}
	} while (true);
	return 0;
}

DWORD WINAPI WinPcapStart(LPVOID Param)
{
	hMP = FindWindow("MapleStoryClass", NULL);
	if (hMP == NULL)
	{
		MessageBox(MainDlg, TEXT("�������� �Ⱥ�����."), TEXT("TopSeven"), 16);
		return 0;
	}
	char *filter = "tcp && src portrange 8585-8589";
	SignalCount = 0;
	sprintf(Buffer, "%d", SignalCount);
	SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
	int res;
	int inum = atoi(SignalDevice);
	int i = 0;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	//���ͷ� ���� 
	struct bpf_program fcode;
	bpf_u_int32 NetMask;
	/* ��Ʈ��ũ �ٹ��̽� ����� �����´�. */
	/* alldevs�� ����Ʈ ���·� ����Ǹ�, ������ errbuf�� ���� ���� ���� */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		sprintf(Buffer, "Error in pcap_findalldevs: %s", errbuf);
		MessageBox(MainDlg, Buffer, TEXT("ERROR"), 16);
		return -1;
	}
	/* ��Ʈ��ũ �ٹ��̽����� ����Ѵ�. */
	for (d = alldevs; d; d = d->next)
	{
		++i;
	}
	/* ���� ó�� */
	if (i == 0)
	{
		MessageBox(MainDlg, TEXT("No interfaces found! Make sure WinPcap is installed."), TEXT("ERROR"), 16);
		return -1;
	}

	/* ĸó�� ��Ʈ��ũ ����̽� ���� */
	/* �Է°��� ��ȿ�� �Ǵ� */
	if (inum < 1 || inum > i)
	{
		MessageBox(MainDlg, TEXT("Interface number out of range."), TEXT("ERROR"), 16);
		/* ��ġ ��� ���� */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* ����ڰ� ������ ��ġ��� ���� */
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);
	/* Open the device */
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
		MessageBox(MainDlg, TEXT(Buffer), TEXT("ERROR"), 16);
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	if (SendMessage(GetDlgItem(MainDlg, SignalMAX_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
	{
		/* �ݸ���ũ ����, �̺κ��� ���� �� �𸣰��� */
		NetMask = 0xffffff;

		// ����ڰ� ������ ���ͷ� ������
		if (pcap_compile(adhandle, &fcode, filter, 1, NetMask) < 0)
		{
			MessageBox(MainDlg, TEXT("Error compiling filter: wrong syntax."), TEXT("ERROR"), 16);
			pcap_close(adhandle);
			return -3;
		}

		// ����ڰ� ������ ���ͷ� ����
		if (pcap_setfilter(adhandle, &fcode) < 0)
		{
			MessageBox(MainDlg, TEXT("Error setting the filter"), TEXT("ERROR"), 16);
			pcap_close(adhandle);
			return -4;
		}
		else
		{
			LOGWRITE("��ȣMAX����.");
		}
	}
	/* At this point, we don't need any more the device list. Free it */
	pcap_freealldevs(alldevs);
	MapleSotryActive(hMP);
	MapleIven(hMP);
	ClickStore();
	PixelCheck = 0;
	while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0)
	{
		if (header->caplen == 65)
		{
			if (SendMessage(GetDlgItem(MainDlg, SignalColor_Check), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
			{
				TIMELOGWRITE("�� ���ν� ����.");
				PixelCheck = 0;
				ColorCheck = TRUE;
				for (int Index = 0; Index <= 1500; Index++)
				{
					GetPixelMP(StorePoint.x, StorePoint.y, hMP);
					if (color != StoreColor)
					{
						TIMELOGWRITE("�� �� Ȯ�ε�.");
						PixelCheck = 1;
						ColorCheck = FALSE;
						break;
					}
					if (Index >= 1500)
					{
						TIMELOGWRITE("�� ��ȣ�� ������ ���� ���� ����.");
						ColorCheck = FALSE;
					}
					Sleep(1);
				}
			}
			else
			{
				PixelCheck = 1;
			}
			if (PixelCheck == 1)
			{
				if (Teleport != -1)
				{
					if (Teleport == 0)
					{
						TeleportUP();
					}
					if (Teleport == 1)
					{
						TeleportRIGHT();
					}
					if (Teleport == 2)
					{
						TeleportDOWN();
					}
					if (Teleport == 3)
					{
						TeleportLEFT();
					}
					Sleep(400);
					PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
					PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
					SignalCount++;
					sprintf(Buffer, "%d", SignalCount);
					SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
					TIMELOGWRITE("�� ������");
					if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
					{
						PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
					}
					TIMELOGWRITE("�� ��������Ȯ����");
					if (!StoreOpenCheck(hMP))
					{
						PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
						ClickStore();
						TIMELOGWRITE("�� ���� �غ���");
						if ((SignalCount % 3) == 0)
						{
							TIMELOGWRITE("�� ���Ǹ���.");
							keybd_event(VK_F1, MapVirtualKey(VK_F1, MAPVK_VK_TO_VSC), 0, 0);
							Sleep(100);
							keybd_event(VK_F1, MapVirtualKey(VK_F1, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
						}
						Sleep(400);
						TIMELOGWRITE("�� ���� ����");
					}
					else
					{
						TIMELOGWRITE("�� ��������Ȯ�� WinPcap����");
						LOGWRITE(Buffer);
						OpenStore0(hMP);
						SignalOnOff = 0;
						if (SendMessage(GetDlgItem(MainDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
						{
							LOGWRITE("�͸� ����.");
							KillTimer(MainDlg, 1);
							EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), TRUE);
						}
						if (SendMessage(GetDlgItem(MainDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
						{
							MTUONOFF(1500);
							LOGWRITE("MTU ����.");
							KillTimer(MainDlg, 2);
							EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), TRUE);
						}
						SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
						pcap_breakloop(adhandle);
					}
				}
				if (!strcmp(SignalSTORE, "������"))
				{
					int count = 0;
					TIMELOGWRITE("�� �����Ŷ Ȯ����.");
					while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0)
					{
						if (header->caplen == 65)
						{
							count = 0;
						}
						if (count >= 10)
						{
							LOGWRITE("�����Ŷ �ƴ�.");
							break;
						}
						if (count <= 10)
						{
							if (header->caplen == 64)
							{
								PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
								PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
								TIMELOGWRITE("�� ������������");
								SignalCount++;
								sprintf(Buffer, "%d", SignalCount);
								SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
								TIMELOGWRITE("�� ������");
								if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
								{
									PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
								}
								Sleep(200);
								TIMELOGWRITE("�� ��������Ȯ����");
								if (!StoreOpenCheck(hMP))
								{
									TIMELOGWRITE("�� ���� �غ���");
									ClickStore();
									TIMELOGWRITE("�� ���� ����");
								}
								else
								{
									TIMELOGWRITE("�� ��������Ȯ�� WinPcap����");
									LOGWRITE(Buffer);
									OpenStore0(hMP);
									SignalOnOff = 0;
									if (SendMessage(GetDlgItem(MainDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
									{
										LOGWRITE("�͸� ����.");
										KillTimer(MainDlg, 1);
										EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), TRUE);
									}
									if (SendMessage(GetDlgItem(MainDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
									{
										MTUONOFF(1500);
										LOGWRITE("MTU ����.");
										KillTimer(MainDlg, 2);
										EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), TRUE);
									}
									SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
									pcap_breakloop(adhandle);
								}
							}
							else
							{
								count++;
							}
						}
					}
				}
				if (!strcmp(SignalSTORE, "�Ϲݻ��� + ������"))
				{
					PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
					PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
					SignalCount++;
					sprintf(Buffer, "%d", SignalCount);
					SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
					TIMELOGWRITE("�� ������");
					if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
					{
						PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
					}
					Sleep(200);
					TIMELOGWRITE("�� ��������Ȯ����");
					if (!StoreOpenCheck(hMP))
					{
						TIMELOGWRITE("�� ���� �غ���");
						ClickStore();
						TIMELOGWRITE("�� ���� ����");
					}
					else
					{
						TIMELOGWRITE("�� ��������Ȯ�� WinPcap����");
						LOGWRITE(Buffer);
						OpenStore0(hMP);
						SignalOnOff = 0;
						if (SendMessage(GetDlgItem(MainDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
						{
							LOGWRITE("�͸� ����.");
							KillTimer(MainDlg, 1);
							EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), TRUE);
						}
						if (SendMessage(GetDlgItem(MainDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
						{
							MTUONOFF(1500);
							LOGWRITE("MTU ����.");
							KillTimer(MainDlg, 2);
							EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), TRUE);
						}
						SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
						pcap_breakloop(adhandle);
					}
				}
				if (!strcmp(SignalSTORE, "Ŭ����"))
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					SignalCount++;
					sprintf(Buffer, "%d", SignalCount);
					SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
					TIMELOGWRITE("�� ������");
				}
			}
		}
		if (header->caplen == 64)
		{
			if (!strcmp(SignalSTORE, "�����"))
			{
				if (SendMessage(GetDlgItem(MainDlg, SignalColor_Check), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
				{
					TIMELOGWRITE("�� ���ν� ����.");
					PixelCheck = 0;
					ColorCheck = TRUE;
					for (int Index = 0; Index <= 1500; Index++)
					{
						GetPixelMP(StorePoint.x, StorePoint.y, hMP);
						if (color != StoreColor)
						{
							TIMELOGWRITE("�� �� Ȯ�ε�.");
							PixelCheck = 1;
							ColorCheck = FALSE;
							break;
						}
						if (Index >= 1500)
						{
							TIMELOGWRITE("�� ��ȣ�� ������ ���� ���� ����.");
							ColorCheck = FALSE;
						}
						Sleep(1);
					}
				}
				else
				{
					PixelCheck = 1;
				}
				if (PixelCheck == 1)
				{
					PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
					PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
					SignalCount++;
					sprintf(Buffer, "%d", SignalCount);
					SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
					TIMELOGWRITE("�� �������");
					if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
					{
						PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
					}
					Sleep(200);
					TIMELOGWRITE("�� ��������Ȯ����");
					if (!StoreOpenCheck(hMP))
					{
						TIMELOGWRITE("�� ���� �غ���");
						ClickStore();
						TIMELOGWRITE("�� ���� ����");
					}
					else
					{
						TIMELOGWRITE("�� ��������Ȯ�� WinPcap����");
						LOGWRITE(Buffer);
						OpenStore0(hMP);
						SignalOnOff = 0;
						if (SendMessage(GetDlgItem(MainDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
						{
							LOGWRITE("�͸� ����.");
							KillTimer(MainDlg, 1);
							EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), TRUE);
						}
						if (SendMessage(GetDlgItem(MainDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
						{
							MTUONOFF(1500);
							LOGWRITE("MTU ����.");
							KillTimer(MainDlg, 2);
							EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), TRUE);
						}
						SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
						pcap_breakloop(adhandle);
					}
				}
			}
		}
	}
	if (res == -1)
	{
		sprintf(Buffer, "Error reading the packets: %s", pcap_geterr(adhandle));
		MessageBox(MainDlg, TEXT(Buffer), TEXT("ERROR"), 16);
		return -1;
	}
	return 0;
}

DWORD WINAPI WinSockStart(LPVOID Param)
{
	hMP = FindWindow("MapleStoryClass", NULL);
	if (hMP == NULL)
	{
		MessageBox(MainDlg, TEXT("�������� �Ⱥ�����."), TEXT("TopSeven"), 16);
		return 0;
	}
	SignalCount = 0;
	sprintf(Buffer, "%d", SignalCount);
	SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
	unsigned long v = 1, v2;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;
	SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (sock == INVALID_SOCKET)
	{
		MessageBox(MainDlg, "expert 1.0 ERROR CODE 1", "ERROR", MB_OK);
		return 0;
	}
	struct sockaddr_in SockAddr;
	ZeroMemory(&SockAddr, sizeof(SockAddr));
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = NULL;
	SockAddr.sin_addr.s_addr = inet_addr(SignalIP);
	if (bind(sock, (sockaddr *)&SockAddr, sizeof(SockAddr)) == SOCKET_ERROR)
	{
		MessageBox(MainDlg, "bind ERROR", "ERROR", 16);
		return 0;
	}
	struct sockaddr_in fromAddr;
	int fromAddrLen = sizeof(fromAddr);
	if (WSAIoctl(sock, SIO_RCVALL, &v, sizeof(v), 0, 0, &v2, 0, 0) == SOCKET_ERROR)
	{
		MessageBox(MainDlg, "ERROR", "ERROR", 16);
		return 0;
	}
	int iphsz, hsz, pksz;
	m_BUFFSZ = 65536 * 2;
	m_BUFF = (unsigned char*)malloc(m_BUFFSZ + 2);
	m_PIPH = (struct iphdr*)m_BUFF;
	MapleSotryActive(hMP);
	MapleIven(hMP);
	ClickStore();
	PixelCheck = 0;
	while (SignalOnOff)
	{
		memset(&fromAddr, 0, fromAddrLen); // fromAddr �ʱ�ȭ
		m_PACKETSZ = recvfrom(sock, (char*)m_BUFF, m_BUFFSZ, 0, (struct sockaddr *)&fromAddr, &fromAddrLen); // UDP recvfrom ���Լ��ǹ�ȯ���� ���� ��Ŷ������� �����ϸ� -1 �� ��ȯ�Ǵ��ɷ� ��路��
		if (m_PACKETSZ > 0)
		{//��Ŷ����� 0����ũ��
			m_PROTO = m_PIPH->protocol; // �̰� TCP����� ��°��� �̰��ϸ� �������� ���°ɰ���
			m_SRCPORT = 0;
			iphsz = (m_PIPH->ihl * 4); // �̰� ip�������
			pksz = m_PACKETSZ; // ��Ŷ������
			if (m_PROTO == PROTO_TCP)
			{
				m_PTCPH = (struct tcphdr*)&m_BUFF[iphsz]; //�Ƹ� tcp����� �̰ɷ� ���̱���
				m_SRCPORT = ntohs(m_PTCPH->source); //��Ʈ ntohs �� ���� ����ü�� source���µ�
				hsz = m_PTCPH->doff * 4; //tcp�������
				m_DATASZ = pksz - iphsz - hsz; // �⺻��Ŷ - ip��� - tcp��� = only ������ ����
				if (m_SRCPORT >= 8585 && m_SRCPORT <= 8589)
				{ // ��Ʈ�� 8585~8589 �Ͻ� 
					if (m_DATASZ == 11)
					{
						if (SendMessage(GetDlgItem(MainDlg, SignalColor_Check), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
						{
							TIMELOGWRITE("�� ���ν� ����.");
							PixelCheck = 0;
							ColorCheck = TRUE;
							for (int Index = 0; Index <= 1500; Index++)
							{
								GetPixelMP(StorePoint.x, StorePoint.y, hMP);
								if (color != StoreColor)
								{
									TIMELOGWRITE("�� �� Ȯ�ε�.");
									PixelCheck = 1;
									ColorCheck = FALSE;
									break;
								}
								if (Index >= 1500)
								{
									TIMELOGWRITE("�� ��ȣ�� ������ ���� ���� ����.");
									ColorCheck = FALSE;
								}
								Sleep(1);
							}
						}
						else
						{
							PixelCheck = 1;
						}
						if (PixelCheck == 1)
						{
							if (Teleport != -1)
							{
								if (Teleport == 0)
								{
									TeleportUP();
								}
								if (Teleport == 1)
								{
									TeleportRIGHT();
								}
								if (Teleport == 2)
								{
									TeleportDOWN();
								}
								if (Teleport == 3)
								{
									TeleportLEFT();
								}
								Sleep(400);
								PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
								PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
								SignalCount++;
								sprintf(Buffer, "%d", SignalCount);
								SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
								TIMELOGWRITE("�� ������");
								if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
								{
									PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
								}
								TIMELOGWRITE("�� ��������Ȯ����");
								if (!StoreOpenCheck(hMP))
								{
									PostMessage(hMP, WM_KEYDOWN, VK_RETURN, 1835009);
									ClickStore();
									TIMELOGWRITE("�� ���� �غ���");
									if ((SignalCount % 3) == 0)
									{
										TIMELOGWRITE("�� ���Ǹ���.");
										keybd_event(VK_F1, MapVirtualKey(VK_F1, MAPVK_VK_TO_VSC), 0, 0);
										Sleep(100);
										keybd_event(VK_F1, MapVirtualKey(VK_F1, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
									}
									Sleep(400);
									TIMELOGWRITE("�� ���� ����");
								}
								else
								{
									TIMELOGWRITE("�� ��������Ȯ�� WinSock����");
									LOGWRITE(Buffer);
									OpenStore0(hMP);
									SignalOnOff = 0;
									if (SendMessage(GetDlgItem(MainDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
									{
										LOGWRITE("�͸� ����.");
										KillTimer(MainDlg, 1);
										EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), TRUE);
									}
									if (SendMessage(GetDlgItem(MainDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
									{
										MTUONOFF(1500);
										LOGWRITE("MTU ����.");
										KillTimer(MainDlg, 2);
										EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), TRUE);
									}
									SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
								}
							}
							if (!strcmp(SignalSTORE, "������"))
							{
								int count = 0;
								while (count >= 10)
								{
									memset(&fromAddr, 0, fromAddrLen); // fromAddr �ʱ�ȭ
									m_PACKETSZ = recvfrom(sock, (char*)m_BUFF, m_BUFFSZ, 0, (struct sockaddr *)&fromAddr, &fromAddrLen); // UDP recvfrom ���Լ��ǹ�ȯ���� ���� ��Ŷ������� �����ϸ� -1 �� ��ȯ�Ǵ��ɷ� ��路��
									if (m_PACKETSZ > 0)
									{//��Ŷ����� 0����ũ��
										m_PROTO = m_PIPH->protocol; // �̰� TCP����� ��°��� �̰��ϸ� �������� ���°ɰ���
										m_SRCPORT = 0;
										iphsz = (m_PIPH->ihl * 4); // �̰� ip�������
										pksz = m_PACKETSZ; // ��Ŷ������
										if (m_PROTO == PROTO_TCP)
										{
											m_PTCPH = (struct tcphdr*)&m_BUFF[iphsz]; //�Ƹ� tcp����� �̰ɷ� ���̱���
											m_SRCPORT = ntohs(m_PTCPH->source); //��Ʈ ntohs �� ���� ����ü�� source���µ�
											hsz = m_PTCPH->doff * 4; //tcp�������
											m_DATASZ = pksz - iphsz - hsz; // �⺻��Ŷ - ip��� - tcp��� = only ������ ����
											if (m_SRCPORT >= 8585 && m_SRCPORT <= 8589)
											{ // ��Ʈ�� 8585~8589 �Ͻ�
												if (m_DATASZ == 11)
												{
													count = 0;
												}
												if (count >= 5)
												{
													break;
												}
												count++;
												if (count <= 5)
												{
													if (m_DATASZ == 10)
													{
														TIMELOGWRITE("�� ������������");
														PostMessage(hMP, WM_KEYDOWN, 13, 1835009);
														PostMessage(hMP, WM_KEYDOWN, 13, 1835009);
														SignalCount++;
														sprintf(Buffer, "%d", SignalCount);
														SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
														TIMELOGWRITE("�� ������");
														if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
														{
															PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
														}
														Sleep(200);
														TIMELOGWRITE("�� ��������Ȯ����");
														if (!StoreOpenCheck(hMP))
														{
															TIMELOGWRITE("�� ���� �غ���");
															ClickStore();
															TIMELOGWRITE("�� ���� ����");
														}
														else
														{
															TIMELOGWRITE("�� ��������Ȯ�� WinSock����");
															LOGWRITE(Buffer);
															OpenStore0(hMP);
															SignalOnOff = 0;
															if (SendMessage(GetDlgItem(MainDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
															{
																LOGWRITE("�͸� ����.");
																KillTimer(MainDlg, 1);
																EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), TRUE);
															}
															if (SendMessage(GetDlgItem(MainDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
															{
																MTUONOFF(1500);
																LOGWRITE("MTU ����.");
																KillTimer(MainDlg, 2);
																EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), TRUE);
															}
															SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
														}
													}
												}
											}
										}
									}
								}
							}
							if (!strcmp(SignalSTORE, "�Ϲݻ��� + ������"))
							{
								PostMessage(hMP, WM_KEYDOWN, 13, 1835009);
								PostMessage(hMP, WM_KEYDOWN, 13, 1835009);
								SignalCount++;
								sprintf(Buffer, "%d", SignalCount);
								SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
								TIMELOGWRITE("�� ����������");
								if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
								{
									PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
								}
								Sleep(200);
								TIMELOGWRITE("�� ��������Ȯ����");
								if (!StoreOpenCheck(hMP))
								{
									TIMELOGWRITE("�� ���� �غ���");
									ClickStore();
									TIMELOGWRITE("�� ���� ����");
								}
								else
								{
									TIMELOGWRITE("�� ��������Ȯ�� WinSock����");
									LOGWRITE(Buffer);
									OpenStore0(hMP);
									SignalOnOff = 0;
									if (SendMessage(GetDlgItem(MainDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
									{
										LOGWRITE("�͸� ����.");
										KillTimer(MainDlg, 1);
										EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), TRUE);
									}
									if (SendMessage(GetDlgItem(MainDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
									{
										MTUONOFF(1500);
										LOGWRITE("MTU ����.");
										KillTimer(MainDlg, 2);
										EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), TRUE);
									}
									SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
								}
							}
							if (!strcmp(SignalSTORE, "Ŭ����"))
							{
								mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
								mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
								SignalCount++;
								sprintf(Buffer, "%d", SignalCount);
								SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
								TIMELOGWRITE("�� ������");
							}
						}
					}
					if (m_DATASZ == 10)
					{
						if (!strcmp(SignalSTORE, "�����"))
						{
							if (SendMessage(GetDlgItem(MainDlg, SignalColor_Check), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
							{
								TIMELOGWRITE("�� ���ν� ����.");
								PixelCheck = 0;
								ColorCheck = TRUE;
								for (int Index = 0; Index <= 1500; Index++)
								{
									GetPixelMP(StorePoint.x, StorePoint.y, hMP);
									if (color != StoreColor)
									{
										TIMELOGWRITE("�� �� Ȯ�ε�.");
										PixelCheck = 1;
										ColorCheck = FALSE;
										break;
									}
									if (Index >= 1500)
									{
										TIMELOGWRITE("�� ��ȣ�� ������ ���� ���� ����.");
										ColorCheck = FALSE;
									}
									Sleep(1);
								}
							}
							else
							{
								PixelCheck = 1;
							}
							if (PixelCheck == 1)
							{
								PostMessage(hMP, WM_KEYDOWN, 13, 1835009);
								PostMessage(hMP, WM_KEYDOWN, 13, 1835009);
								SignalCount++;
								sprintf(Buffer, "%d", SignalCount);
								SetDlgItemText(MainDlg, COUNT_STATIC, Buffer);
								TIMELOGWRITE("�� ����������");
								if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
								{
									PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
								}
								Sleep(200);
								TIMELOGWRITE("�� ��������Ȯ����");
								if (!StoreOpenCheck(hMP))
								{
									TIMELOGWRITE("�� ���� �غ���");
									ClickStore();
									TIMELOGWRITE("�� ���� ����");
								}
								else
								{
									TIMELOGWRITE("�� ��������Ȯ�� WinSock����");
									LOGWRITE(Buffer);
									OpenStore0(hMP);
									SignalOnOff = 0;
									if (SendMessage(GetDlgItem(MainDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
									{
										LOGWRITE("�͸� ����.");
										KillTimer(MainDlg, 1);
										EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), TRUE);
									}
									if (SendMessage(GetDlgItem(MainDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
									{
										MTUONOFF(1500);
										LOGWRITE("MTU ����.");
										KillTimer(MainDlg, 2);
										EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), TRUE);
									}
									SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
								}
							}
						}
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL TopSeven_Signal_Start(HWND hDlg)
{
	{
		static HANDLE hThreadFunc;
		if (SAVECHECK == FALSE)
		{
			LOGWRITE("��� ����� üũ���ּ���.");
			return FALSE;
		}
		if (SignalOnOff != 1)
		{
			if (SendMessage(GetDlgItem(MainDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
			{
				if (WARIONOFF == FALSE)
				{
					LOGWRITE("�͸��� ���� �������ּ���.");
					return FALSE;
				}
				LOGWRITE("�͸� ��� ����.");
				GetDlgItemText(MainDlg, WARI_MIN, Buffer, 256);
				WARIDELAY = (atoi(Buffer) * 60000);
				if (WARIDELAY == 0)
				{
					WARIDELAY = 1800000;
				}
				WARICOUNT = 0;
				SetTimer(MainDlg, 1, WARIDELAY, (TIMERPROC)WARITimerProc);
			}
			if (SendMessage(GetDlgItem(MainDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
			{
				LOGWRITE("MTU ��� ����.");
				MTUONOFF(56);
				SetTimer(MainDlg, 2, 120000, (TIMERPROC)MTUTimerProc);
			}
			if (SendMessage(GetDlgItem(MainDlg, SignalColor_Check), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
			{
				if (StoreColor == NULL)
				{
					LOGWRITE("���� �������ּ���.");
					LOGWRITE("��ȣ + ���� ������ ���콺�� ����");
					LOGWRITE("F10���� �����ּ���");
					return FALSE;
				}
				LOGWRITE("��ȣ + �� ����.");
			}
			SignalOnOff = 1;
			SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
			if (!strcmp(SignalMODE, "WinSock"))
			{
				LOGWRITE("WinSock ����");
				if (SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0) == 3)
				{
					LOGWRITE("���� ����.");
				}
				hThreadFunc = CreateThread(NULL, 0, WinSockStart, NULL, 0, &ThreadID);
				return TRUE;
			}
			else if (!strcmp(SignalMODE, "WinPcap"))
			{
				LOGWRITE("WinPcap ����");
				if (SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0) == 3)
				{
					LOGWRITE("���� ����.");
				}
				hThreadFunc = CreateThread(NULL, 0, WinPcapStart, NULL, 0, &ThreadID);
			}
			else if (!strcmp(SignalMODE, "NPF"))
			{
				LOGWRITE("NPF ����");
				if (SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0) == 3)
				{
					LOGWRITE("���� ����.");
				}
				hThreadFunc = CreateThread(NULL, 0, NPFStart, NULL, 0, &ThreadID);
			}
			return TRUE;
		}
		if (SignalOnOff == 1)
		{
			SignalOnOff = 0;
			PostMessageSend(hMP, 27, 65537);
			SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
			if (SendMessage(GetDlgItem(hDlg, WARI_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
			{
				LOGWRITE("�͸� ����.");
				KillTimer(MainDlg, 1);
				EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), TRUE);
			}
			if (SendMessage(GetDlgItem(hDlg, MTU_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
			{
				MTUONOFF(1500);
				LOGWRITE("MTU ����.");
				KillTimer(MainDlg, 2);
				EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), TRUE);
			}
			SetDlgItemText(MainDlg, START_BUTTON, "���� [F9]");
			if (!strcmp(SignalMODE, "WinSock"))
			{
				LOGWRITE("WinSock ����");
				if (SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0) == 3)
				{
					LOGWRITE("���� ����.");
				}
				TerminateThread(hThreadFunc, 0);
				return TRUE;
			}
			else if (!strcmp(SignalMODE, "WinPcap"))
			{
				LOGWRITE("WinPcap ����");
				if (SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0) == 3)
				{
					LOGWRITE("���� ����.");
				}
				pcap_breakloop(adhandle);
				TerminateThread(hThreadFunc, 0);
				return TRUE;
			}
			else if (!strcmp(SignalMODE, "NPF"))
			{
				LOGWRITE("NPF ����");
				if (SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0) == 3)
				{
					LOGWRITE("���� ����.");
				}
				TerminateThread(hThreadFunc, 0);
				return TRUE;
			}
			return TRUE;
		}
		return TRUE;
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	if (IsDebuggerPresent())
	{
		ExitProcess(1);
	}
	typedef LONG(WINAPI *ZWSETINFORMATIONTHREAD)
		(HANDLE, DWORD, PVOID, ULONG);
	HMODULE hNtDLL = GetModuleHandle("ntdll.dll");
	if (hNtDLL)
	{
		ZWSETINFORMATIONTHREAD SetThread = (ZWSETINFORMATIONTHREAD)GetProcAddress(hNtDLL, "ZWSETINFORMATIONTHREAD");
		if (SetThread)
		{
			SetThread((HANDLE)-2L, (DWORD)0x11, NULL, 0);
		}
	}
	WINSTATREMINATEPROC WinStationTerminateProcess = (WINSTATREMINATEPROC)GetProcAddress(LoadLibrary(TEXT("winsta.dll")), "WinStationTerminateProcess");
	if (!WinStationTerminateProcess)
	{
		MessageBox(MainDlg, "WinStationTerminateProcess �������� ã���� �������ϴ�.", "����", MB_OK | MB_ICONERROR);
		return 0;
	}
	OutputDebugString(TEXT("%s%s%s%s%s%s%s%s%s%s%s")
		TEXT("%s%s%s%s%s%s%s%s%s%s%s%s%s")
		TEXT("%s%s%s%s%s%s%s%s%s%s%s%s%s")
		TEXT("%s%s%s%s%s%s%s%s%s%s%s%s%s"));
	HANDLE hWinDbg = FindWindow(TEXT("WinDbgFrameClass"), NULL);
	if (hWinDbg)
		ExitProcess(0);
	HANDLE hOlly = FindWindow(TEXT("OLLYDBG"), NULL);
	if (hOlly)
		ExitProcess(0);
	if (IsDbgPresentPrefixCheck())
		return false;
	PushPopSS();
	if (Int2DCheck())
		return false;
	if (DebugObjectCheck())
		return false;
	if (CheckProcessDebugFlags())
		return false;
	if (MemoryBreakpointDebuggerCheck())
		return false;
	if (!IsWow64())
	{
		MessageBox(MainDlg, TEXT("The process is not running under WOW64."), TEXT("ERROR"), 16);
		return false;
	}
	if (AnotherInstance())
		return false;
	if (!IsUserAnAdmin())
	{
		MessageBox(MainDlg, TEXT("������ ������ �ƴմϴ�."), TEXT("ERROR"), 16);
		return false;
	}
	//VersionCheck();
	//ServerONOFF();

	g_hInst = hInstance;
	HddSerialLogin();
	DialogBox(g_hInst, MAKEINTRESOURCE(MAIN), HWND_DESKTOP, MainDlgProc);
	return 0;
}

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	START = GetDlgItem(hDlg, START_BUTTON);
	switch (iMessage)
	{
	case WM_INITDIALOG:
		PlaySound((LPCSTR)MAKEINTRESOURCE(OpeningWave), NULL, SND_ASYNC | SND_RESOURCE);
		MainDlg = hDlg;
		int nResult;
		HOTKEY(MainDlg);
		LOGWRITE("������ �ѱ� : F1");
		LOGWRITE("�ϻ� ��� : F3");
		LOGWRITE("��� ��� : F4");
		LOGWRITE("��ȣ + �� : F10");
		LOGWRITE("�͸� : F11");
		LOGWRITE("����Ű F1,����Ű F2");
		sprintf(Buffer, "�ڵ� : %X", Serial);
		LOGWRITE(Buffer);
		LOGWRITE("����������������������������������������������������������������");
		if (!MapleStoryReg())
		{
			LOGWRITE("������ ������ġ ã�� ����.");
		}
		sprintf(Buffer, "%s\\MapleStory.exe", MapleRoot);
		if (access(Buffer, 0) == -1)
		{
			LOGWRITE("��ο� ������ �������� �ʽ��ϴ�.");
		}
		if (GetMapleScreenSize() && MX1 != 0)
		{
			SetWindowPos(hDlg, HWND_TOP, GetMX1 + MX1, GetMY1, 0, 0, SWP_NOSIZE);
		}
		else
		{
			GetWindowRect(hDlg, &rect);
			SetWindowPos(hDlg, HWND_TOP, GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left), 0, 0, 0, SWP_NOSIZE);
		}
		SetTimer(MainDlg, 3, 50, (TIMERPROC)MapleStoryScreenXY);
		break;
	case WM_HOTKEY:
	{


		if (wParam != 1)
		{
			hMP = FindWindow("MapleStoryClass", NULL);
			if (hMP == NULL)
			{
				LOGWRITE("�������� �Ⱥ�����.");
				return 0;
			}
			else
			{
				MapleSotryActive(hMP);
			}
			if (wParam == 3)
			{
				GetPixelMP((MX1 / 2) + 50, 450, hMP);
				if (color == 0xFFFFFF)
				{
					GetPixelMP(550, 450, hMP);
					if (color == 0xFFFFFF)
					{
						MapleIven(hMP);
						ReStore0(hMP, hDlg);
						break;
					}
				}
				for (int Index = 0; Index <= 2; Index++)
				{
					MapleIven(hMP);
					ClickStore0(hMP);
					TitleSend();
					PostMessageSend(hMP, 13, 1835009);
					if (StoreOpenCheck(hMP))
					{
						OpenStore0(hMP);
						LOGWRITE("�ϻ󿭱� ����!");
						break;
					}
				}
			}

			if (wParam == 4)
			{
				GetPixelMP((MX1 / 2) + 50, 450, hMP);
				if (color == 0xFFFFFF)
				{
					GetPixelMP(550, 450, hMP);
					if (color == 0xFFFFFF)
					{
						MapleIven(hMP);
						ReStore1(hMP, hDlg);
						break;
					}
				}
				for (int Index = 0; Index <= 2; Index++)
				{
					MapleIven(hMP);
					ClickStore1(hMP);
					TitleSend();
					PostMessageSend(hMP, 13, 1835009);
					PostMessageSend(hMP, 13, 1835009);
					PostMessageSend(hMP, 13, 1835009);
					PostMessageSend(hMP, 13, 1835009);
					if (StoreOpenCheck(hMP))
					{
						OpenStore0(hMP);
						LOGWRITE("��󿭱� ����!");
						break;
					}
				}
			}

			if (wParam == 5)
			{
				HddSerialLogin();
			}

			if (wParam == 9)
			{
				if (hMP == NULL)
				{
					LOGWRITE("�������� ������ �������ּ���.");
					return FALSE;
				}
				if (SAVECHECK)
				{
					TopSeven_Signal_Start(hDlg);
				}
				else
				{
					LOGWRITE("������ ���� ���ּ���.");
				}
			}

			if (wParam == 10)
			{
				GetCursorPos(&StorePoint);
				GetPixelMP(StorePoint.x, StorePoint.y, hMP);
				StoreColor = color;
				sprintf(Buffer, "X : %d Y : %d Color : 0x%X", StorePoint.x, StorePoint.y, StoreColor);
				LOGWRITE("��ȣ + �� ��ǥ ����Ϸ�!");
				LOGWRITE(Buffer);
			}

			if (wParam == 11)
			{
				if (WARIINDEX == 1)
				{
					GetCursorPos(&WARI1);
					sprintf(Buffer, "%d��° �͸� ��ǥ ����Ϸ�!", WARIINDEX);
					LOGWRITE(Buffer);
					sprintf(Buffer, "X : %d Y : %d", WARI1.x, WARI1.y);
					LOGWRITE(Buffer);
				}
				if (WARIINDEX == 2)
				{
					GetCursorPos(&WARI2);
					sprintf(Buffer, "%d��° �͸� ��ǥ ����Ϸ�!", WARIINDEX);
					LOGWRITE(Buffer);
					sprintf(Buffer, "X : %d Y : %d", WARI2.x, WARI2.y);
					LOGWRITE(Buffer);
				}
				if (WARIINDEX == 3)
				{
					GetCursorPos(&WARI3);
					sprintf(Buffer, "%d��° �͸� ��ǥ ����Ϸ�!", WARIINDEX);
					LOGWRITE(Buffer);
					sprintf(Buffer, "X : %d Y : %d", WARI3.x, WARI3.y);
					LOGWRITE(Buffer);
				}
				if (WARIINDEX == 4)
				{
					WARIONOFF = TRUE;
					GetCursorPos(&WARI4);
					sprintf(Buffer, "%d��° �͸� ��ǥ ����Ϸ�!", WARIINDEX);
					LOGWRITE(Buffer);
					sprintf(Buffer, "X : %d Y : %d", WARI4.x, WARI4.y);
					LOGWRITE(Buffer);
					WARIINDEX = 0;
				}
				WARIINDEX++;
			}
			return TRUE;
		}
		else
		{
			if (wParam == 1)
			{
				hMP = FindWindow("MapleStoryClass", NULL);
				if (hMP != NULL)
				{
					LOGWRITE("�������� �̹� �����ֽ��ϴ�.");
					return 0;
				}
				sprintf(Buffer, "%s\\MapleStory.exe", MapleRoot);
				nResult = access(Buffer, 0);
				if (nResult == 0)
				{
					LOGWRITE("������ ����!");
					sprintf(Buffer, "%s\\MapleStory.exe  175.207.0.35 8484", MapleRoot);
					WinExec(Buffer, SW_SHOW);
				}
				else if (nResult == -1)
				{
					LOGWRITE("������ ���� �Ұ���!");
					LOGWRITE("��ο� ������ �������� �ʽ��ϴ�.");
				}
			}
		}
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case SETTING_BUTTON:
			WPWRITE = FALSE; WSWRITE = FALSE;
			EndDialog(g_hDlg, 0);
			DialogBox(g_hInst, MAKEINTRESOURCE(Setting), HWND_DESKTOP, SETTINGDlgProc);
			break;
		case START_BUTTON:
		{
			TopSeven_Signal_Start(hDlg);
			break;
		}
		case IDCANCEL:
			MTUONOFF(1500);
			CLOSEHOTKEY(MainDlg);
			KillTimer(MainDlg, 3);
			ExitProcess(0);
			break;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CALLBACK SETTINGDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		g_hDlg = hDlg;
		GetClientRect(hDlg, &rect);
		SetWindowPos(hDlg, HWND_TOP, GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left) - 15, 0, 0, 0, SWP_NOSIZE);
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)"WinSock");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)"WinPcap");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)"NPF");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"�Ϲݻ��� + ������");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"������");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"�����");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"����");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"Ŭ����");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO5), CB_ADDSTRING, 0, (LPARAM)"�Ϲݻ���");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO5), CB_ADDSTRING, 0, (LPARAM)"������");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO6), CB_ADDSTRING, 0, (LPARAM)"��");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO6), CB_ADDSTRING, 0, (LPARAM)"��");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO6), CB_ADDSTRING, 0, (LPARAM)"��");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO6), CB_ADDSTRING, 0, (LPARAM)"��");
		break;
	}
	return FALSE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_COMBO1:
		{
			switch (SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0))
			{
			case 0:
			{
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO4), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO3), TRUE);
				if (WSWRITE == FALSE)
				{
					WSWRITE = TRUE;
					char ac[255] = { 0, };
					struct in_addr addr;
					WSADATA wsa;
					if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
						return -1;
					SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
					if (sock == INVALID_SOCKET)
					{
						MessageBox(MainDlg, "expert 1.0 ERROR CODE 1", "ERROR", MB_OK);
						return 0;
					}
					if (gethostname(ac, sizeof(ac)) != SOCKET_ERROR)
					{
						struct hostent *phe = gethostbyname(ac);
						for (int i = 0; phe->h_addr_list[i] != NULL; i++)
						{
							memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
							SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)inet_ntoa(addr));
						}
					}
				}
				break;
			}
			case 1:
			{
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO3), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO4), TRUE);
				if (WPWRITE == FALSE)
				{
					WPWRITE = TRUE;
					if (pcap_findalldevs(&alldevs, errbuf) == -1)
					{
						MessageBox(MainDlg, TEXT("WinPcap ERROR"), TEXT("ERROR"), 16);
						return 0;
					}
					for (d = alldevs; d; d = d->next)
					{
						if (d->description)
						{
							SendMessage(GetDlgItem(hDlg, IDC_COMBO4), CB_ADDSTRING, 0, (LPARAM)d->description);
						}
						else
						{
							MessageBox(MainDlg, TEXT("WinPcap ERROR"), TEXT("ERROR"), 16);
							return 0;
						}
					}
				}
				break;
			}
			case 2:
			{
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO3), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO4), TRUE);
				if (WPWRITE == FALSE)
				{
					WPWRITE = TRUE;
					if (pcap_findalldevs(&alldevs, errbuf) == -1)
					{
						MessageBox(MainDlg, TEXT("WinPcap ERROR"), TEXT("ERROR"), 16);
						return 0;
					}
					for (d = alldevs; d; d = d->next)
					{
						if (d->description)
						{
							SendMessage(GetDlgItem(hDlg, IDC_COMBO4), CB_ADDSTRING, 0, (LPARAM)d->description);
						}
						else
						{
							MessageBox(MainDlg, TEXT("WinPcap ERROR"), TEXT("ERROR"), 16);
							return 0;
						}
					}
				}
				break;
			}
			}
			return TRUE;
		}

		case IDC_COMBO2:
		{
			if (SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0) == 3)
			{
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO6), TRUE);
			}
			else
			{
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO6), FALSE);
			}
			break;
		}

		case IDC_COMBO3:
			if ((GetDlgItem(hDlg, IDC_COMBO3), CB_GETCURSEL, 0, 0) != -1)
			{
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO5), TRUE);
				break;
			}
		case IDC_COMBO4:
			if ((GetDlgItem(hDlg, IDC_COMBO4), CB_GETCURSEL, 0, 0) != -1)
			{
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO5), TRUE);
				break;
			}
		case SAVE_BUTTON:
			GetDlgItemText(hDlg, StoreName_EDIT, StoreName, 256);
			GetDlgItemText(hDlg, IDC_COMBO1, SignalMODE, 256);
			GetDlgItemText(hDlg, IDC_COMBO2, SignalSTORE, 256);
			GetDlgItemText(hDlg, IDC_COMBO3, SignalIP, 256);
			GetDlgItemText(hDlg, IDC_COMBO4, SignalDevice, 256);
			GetDlgItemText(hDlg, IDC_COMBO5, SignalOPENSTORE, 256);
			if (!strcmp(SignalIP, ""))
			{
				if (!strcmp(SignalDevice, ""))
				{
					MessageBox(MainDlg, TEXT("��� ������ �������ּ���."), TEXT("ERROR"), 16);
					return FALSE;
				}
			}
			if (SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0) == 3)
			{
				GetDlgItemText(hDlg, IDC_COMBO6, Buffer, 256);
				if (!strcmp(Buffer, ""))
				{
					MessageBox(MainDlg, TEXT("��� ������ �������ּ���."), TEXT("ERROR"), 16);
					return FALSE;
				}
			}
			if (!strcmp(SignalOPENSTORE, ""))
			{
				MessageBox(MainDlg, TEXT("��� ������ �������ּ���."), TEXT("ERROR"), 16);
				return FALSE;
			}
			sprintf(Buffer, "�νĸ�� : %s�Դϴ�.", SignalMODE);
			LOGWRITE(Buffer);
			sprintf(Buffer, "�������� : %s�Դϴ�.", SignalSTORE);
			LOGWRITE(Buffer);
			if (SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0) == 3)
			{
				switch (SendMessage(GetDlgItem(hDlg, IDC_COMBO6), CB_GETCURSEL, 0, 0))
				{
				case 0:
					LOGWRITE("���� ������ ���Դϴ�.");
					Teleport = 0;
					break;
				case 1:
					LOGWRITE("���� ������ ���Դϴ�.");
					Teleport = 1;
					break;
				case 2:
					LOGWRITE("���� ������ ���Դϴ�.");
					Teleport = 2;
					break;
				case 3:
					LOGWRITE("���� ������ ���Դϴ�.");
					Teleport = 3;
					break;
				}
			}
			else
			{
				Teleport = -1;
			}
			if (!strcmp(SignalMODE, "WinSock"))
			{
				sprintf(Buffer, "IP : %s�Դϴ�.", SignalIP);
			}
			else if (!strcmp(SignalMODE, "WinPcap"))
			{
				sprintf(Buffer, "Device : %s�Դϴ�.", SignalDevice);
			}
			else if (!strcmp(SignalMODE, "NPF"))
			{
				sprintf(Buffer, "Device : %s�Դϴ�.", SignalDevice);
			}
			LOGWRITE(Buffer);
			sprintf(SignalDevice, "%d", SendMessage(GetDlgItem(hDlg, IDC_COMBO4), CB_GETCURSEL, 0, 0) + 1);
			sprintf(Buffer, "�������� : %s�Դϴ�.", SignalOPENSTORE);
			LOGWRITE(Buffer);
			LOGWRITE("�����Ϸ�");
			LOGWRITE("");
			SAVECHECK = TRUE;
			EnableWindow(START, TRUE);
			EndDialog(g_hDlg, 0);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

void CALLBACK WARITimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
	{
		PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
	}
	Sleep(2000);
	GetMapleScreenSize();
	WARISTART = TRUE;
	LOGWRITE("�͸����");
	EnableWindow(GetDlgItem(MainDlg, WARI_CHECK), FALSE);
	TIMELOGWRITE("�� �͸�����");
	PostMessageSend(hMP, 27, 65537);
	TIMELOGWRITE("�� �����ݱ�");
	Sleep(500);
	PostMessageSend(hMP, 27, 65537);
	Sleep(500);
	switch (WARICOUNT)
	{
	case 0:
		TIMELOGWRITE("�� 1����������");
		Click(2, WARI1.x, WARI1.y);
		if (StoreOpenCheck(hMP))
		{
			Sleep(300);
			TIMELOGWRITE("�� �������� ������");
			PostMessageSend(hMP, 27, 65537);
			WARICOUNT++;
		}
		break;
	case 1:
		TIMELOGWRITE("�� 2����������");
		Click(2, WARI2.x, WARI2.y);
		if (StoreOpenCheck(hMP))
		{
			Sleep(300);
			TIMELOGWRITE("�� �������� ������");
			PostMessageSend(hMP, 27, 65537);
			WARICOUNT++;
		}
		break;
	case 2:
		TIMELOGWRITE("�� 3����������");
		Click(2, WARI3.x, WARI3.y);
		if (StoreOpenCheck(hMP))
		{
			Sleep(300);
			TIMELOGWRITE("�� �������� ������");
			PostMessageSend(hMP, 27, 65537);
			WARICOUNT++;
		}
		break;
	case 3:
		TIMELOGWRITE("�� 4����������");
		Click(2, WARI4.x, WARI4.y);
		if (StoreOpenCheck(hMP))
		{
			Sleep(300);
			TIMELOGWRITE("�� �������� ������");
			PostMessageSend(hMP, 27, 65537);
			WARICOUNT++;
		}
		break;
	}
	if (WARICOUNT >= 3)
		WARICOUNT = 0;
	if (StoreOpenCheck(hMP))
	{
		Sleep(1000);
		PostMessageSend(hMP, 27, 65537);
		Sleep(300);
	}
	TIMELOGWRITE("�� �͸� ����");
	MapleIven(hMP);
	ClickStore();
	TIMELOGWRITE("�� ���� ����");
	if (SendMessage(GetDlgItem(MainDlg, SignalHide_CHECK), BM_GETCHECK, 0, 0) != BST_UNCHECKED)
	{
		PostMessage(hMP, WM_SYSCOMMAND, (WPARAM)SC_MINIMIZE, 0);
	}
	WARISTART = FALSE;
}

void CALLBACK MTUTimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	EnableWindow(GetDlgItem(MainDlg, MTU_CHECK), FALSE);
	TIMELOGWRITE("�� MTU�͸� ����");
	MTUONOFF(1500);
	Sleep(5000);
	MTUONOFF(56);
	TIMELOGWRITE("�� MTU�͸� ����");
}

void CALLBACK MapleStoryScreenXY(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	GetMapleScreenSize();
}
