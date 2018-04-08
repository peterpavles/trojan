// cclientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "cclient.h"
#include "cclientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CcclientDlg 对话框



CcclientDlg::CcclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcclientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_hIcon = AfxGetApp()->LoadIcon(IDI_FILE);
	WSADATA WSADa;
	WSAStartup(0x0202, &WSADa);

}
CcclientDlg::~CcclientDlg() {
	WSACleanup();
}

void CcclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_ip);
	DDX_Control(pDX, IDC_PORTEDIT, m_port);
	DDX_Control(pDX, IDC_PASSEDIT, m_pass);
	DDX_Control(pDX, IDC_PWD, m_pwd);
	DDX_Control(pDX, IDC_LOGIN, m_login);
	DDX_Control(pDX, IDC_LIST1, m_clientlist);
	DDX_Control(pDX, IDC_LIST2, m_filelist);
}

BEGIN_MESSAGE_MAP(CcclientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_QUIT, &CcclientDlg::OnBnClickedQuit)
	ON_BN_CLICKED(IDC_LOGIN, &CcclientDlg::OnBnClickedLogin)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CcclientDlg::OnDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CcclientDlg::OnDblclkList2)
	ON_BN_CLICKED(IDC_REFLASH, &CcclientDlg::OnBnClickedReflash)
	ON_EN_CHANGE(IDC_PASSEDIT, &CcclientDlg::OnEnChangePassedit)
	ON_BN_CLICKED(IDC_BACK, &CcclientDlg::OnBnClickedBack)
	ON_BN_CLICKED(IDC_UP, &CcclientDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_GO, &CcclientDlg::OnBnClickedGo)
END_MESSAGE_MAP()


// CcclientDlg 消息处理程序

BOOL CcclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	DWORD dwStyle = m_clientlist.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）

	m_clientlist.SetExtendedStyle(dwStyle);

	m_clientlist.InsertColumn(0, L"IP", LVCFMT_LEFT, 85);//插入列
	m_clientlist.InsertColumn(1, L"PORT", LVCFMT_LEFT, 50);
	m_clientlist.InsertColumn(2, L"GUID", LVCFMT_LEFT, 250);
	dwStyle = m_filelist.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）

	m_filelist.SetExtendedStyle(dwStyle);
	m_filelist.InsertColumn(0, L"FileName", LVCFMT_LEFT, 210);
	m_filelist.InsertColumn(1, L"FileType", LVCFMT_LEFT, 80);

	il.Create(16, 16, ILC_COLOR32, 1, 1);
	il.Add(AfxGetApp()->LoadIcon(IDI_FILE));
	il.Add(AfxGetApp()->LoadIcon(IDI_FILEMANAGER));
	il.Add(AfxGetApp()->LoadIcon(IDI_LOCK));
	m_filelist.SetImageList(&il, LVSIL_SMALL);
	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




char** splitArgs(char* cmd, int &total);
void freeArgs(char** argvs, int &total);


int getCount(char* c) {
	int result = 0;
	int g = 1;
	char* temp = strchr(c, '\r');
	while (c + 1 != temp--) {
		result += (*temp - '0') * g;
		g *= 10;
	}

	return result;
}

char* setPointer(char* cmd) {
	return strchr(cmd, '\n') + 1;
}

char* getString(char* cmd) {
	int len = strchr(cmd, '\r') - cmd;
	char *temp = new char[len + 1];
	temp[len] = '\0';
	strncpy(temp, cmd, len);
	return temp;
}

char** splitArgs(char* cmd, int &total) {
	int time = 1;
	char** argv = NULL;
	total = 1;
	while (cmd[0] != '\0' && time--) {
		switch (cmd[0]) {
		case '*':	total = getCount(cmd); cmd = setPointer(cmd); argv = new char*[total]; time = total; break;
		case '$':
			cmd = setPointer(cmd);
			if (total == 1) {
				argv = new char*[1];
				argv[0] = getString(cmd);
			}
			else
				argv[total - time - 1] = getString(cmd);
			cmd = setPointer(cmd);
			break;
		case '-':
			argv = NULL;
			total = 0;
			break;
		default:
			break;
		}
	}
	return argv;
}


void freeArgs(char** argvs, int &total) {
	for (int i = 0; i < total; i++) {
		if (argvs[i])
			delete[] argvs[i];
	}
	if (argvs)
		delete[] argvs;
	total = 0;
}


void CcclientDlg::OnBnClickedQuit() {
	exit(0);
}

void CcclientDlg::OnBnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码

	// TODO:  在此添加控件通知处理程序代码
	CString str;
	sockaddr_in SockAddrIn;
	char buff[1024 * 16];



	//GetDlgItemText(, IDC_IPADDRESS1, buff, 24);
	DWORD ip;
	m_login.GetWindowText(str);
	if (str.Compare(L"登陆")) {
		m_login.SetWindowTextW(L"登陆");
		closesocket(CSocket);
		return;
	}

	m_ip.GetAddress(ip);
	SockAddrIn.sin_family = AF_INET;
	SockAddrIn.sin_addr.s_addr = htonl(ip);
	SockAddrIn.sin_port = htons(GetDlgItemInt(IDC_PORTEDIT));
	CSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	connect(CSocket, (struct sockaddr*)&SockAddrIn, sizeof(SockAddrIn));
	m_pass.GetWindowText(str);
	int n = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)str.GetBuffer(0), -1, NULL, 0, NULL, NULL);
	char *pBuffer = (char*)malloc(n * sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)str.GetBuffer(0), -1, pBuffer, n * sizeof(char), NULL, NULL);

	sprintf(buff, "enable 2 %s\r\n", pBuffer);
	send(CSocket, buff, strlen(buff), 0);
	ZeroMemory(buff, sizeof(buff));
	recv(CSocket, buff, sizeof(buff), 0);
	if (!strncmp(buff, "+OK\r\n", 5)) {
		m_login.SetWindowTextW(L"注销");
		MessageBox(L"登陆成功",L"提示", MB_OK);
	}
	else
		MessageBox(L"请检查连接信息", L"登陆失败", MB_OK);
	free(pBuffer);


}


void CcclientDlg::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	char buf[1024];
	TCHAR argvBuf[1024];

	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{
		// 获取被选择列表项第一个子项的文本   
		m_clientlist.GetItemText(pNMListView->iItem, 2, guid, 64);
		// 将选择的语言显示与编辑框中   
		int n = WideCharToMultiByte(CP_ACP, 0, guid, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, guid, -1, cguid, 64, NULL, NULL);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "ls %s\r\n", cguid);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count < 0) {
			return;
		}
		memset(buf, 0, sizeof(buf));
		count = recv(CSocket, buf, sizeof(buf), 0);
		if (count < 0) {
			return;
		}
		char **argv = splitArgs(buf, count);
		m_filelist.DeleteAllItems();
		for (int i = 0; i < count && argv; i++) {
			int length = MultiByteToWideChar(CP_ACP, 0, argv[i], -1, NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, argv[i], -1, argvBuf, length);


			if (argvBuf[0] == L'/') {
				m_filelist.InsertItem(i, argvBuf + 1, 1);
				m_filelist.SetItemText(i, 1, L"文件夹");
			}
			else if(argvBuf[0] == L'?'){
				m_filelist.InsertItem(i, argvBuf + 1, 2);
				m_filelist.SetItemText(i, 1, L"加密文件");
				//m_filelist.InsertItem(i, L"文件");
			}
			else {
				m_filelist.InsertItem(i, argvBuf, 0);
				m_filelist.SetItemText(i, 1, L"文件");
				//m_filelist.InsertItem(i, L"文件");
			}
				
			//m_filelist.SetItemText(i, 1, end1);
			//m_filelist.SetItemText(i, 2, end2);
		}
		freeArgs(argv, count);
	}
}


void CcclientDlg::OnDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	char buf[1024 * 16];
	TCHAR argvBuf[1024];
	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{
		// 获取被选择列表项第一个子项的文本   
		TCHAR filename[1024];

		m_filelist.GetItemText(pNMListView->iItem, 1, filename, 1024);
		if (!wcscmp(filename, L"文件夹")) {
			m_filelist.GetItemText(pNMListView->iItem, 0, filename, 1024);
			int n = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
			char *pBuffer = (char*)malloc(n * sizeof(char));
			WideCharToMultiByte(CP_ACP, 0, filename, -1, pBuffer, n * sizeof(char), NULL, NULL);

			
			// 进入目录
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "cd %s %s\r\n", cguid, pBuffer);
			int count = send(CSocket, buf, strlen(buf), 0);
			if (count < 0) {
				return;
			}
			memset(buf, 0, sizeof(buf));
			count = recv(CSocket, buf, sizeof(buf), 0);
			if (count < 0) {
				return;
			}
			if (strncmp(buf, "+OK\r\n", sizeof("+OK\r\n")))
				return;

			// 罗列目录文件
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "ls %s\r\n", cguid);
			send(CSocket, buf, strlen(buf), 0);
			memset(buf, 0, sizeof(buf));
			recv(CSocket, buf, sizeof(buf), 0);
			char **argv = splitArgs(buf, count);
			m_filelist.DeleteAllItems();
			for (int i = 0; argv && i < count; i++) {
				int length = MultiByteToWideChar(CP_ACP, 0, argv[i], -1, NULL, 0);
				MultiByteToWideChar(CP_ACP, 0, argv[i], -1, argvBuf, length);

				if (argvBuf[0] == L'/') {
					m_filelist.InsertItem(i, argvBuf + 1, 1);
					m_filelist.SetItemText(i, 1, L"文件夹");
				}
				else if (argvBuf[0] == L'?') {
					m_filelist.InsertItem(i, argvBuf + 1, 2);
					m_filelist.SetItemText(i, 1, L"加密文件");
					//m_filelist.InsertItem(i, L"文件");
				}
				else {
					m_filelist.InsertItem(i, argvBuf, 0);
					m_filelist.SetItemText(i, 1, L"文件");
				}
					
				//m_filelist.SetItemText(i, 1, end1);
				//m_filelist.SetItemText(i, 2, end2);
			}
			freeArgs(argv, count);
			free(pBuffer);
		}
	}
}


void CcclientDlg::OnBnClickedReflash()
{
	// TODO: 在此添加控件通知处理程序代码

	char buff[1024 * 16];
	int argc;
	m_clientlist.DeleteAllItems();
	sprintf(buff, "getclients\r\n");
	int count = send(CSocket, buff, strlen(buff), 0);
	if (count < 0)
		return;

	ZeroMemory(buff, sizeof(buff));
	int data = recv(CSocket, buff, sizeof(buff), 0);
	if (data <= 0) {
		m_login.SetWindowTextW(L"登陆");
		MessageBox(L"请检查连接信息", L"失败", MB_OK);
		return;
	}

	char** argv = splitArgs(buff, argc);
	for (int i = 0; i < argc && argv; i++) {
		int n = MultiByteToWideChar(CP_ACP, 0, argv[i], -1, NULL, 0);
		wchar_t *pBuffer = (wchar_t*)malloc(n * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, argv[i], -1, pBuffer, n * sizeof(char));

		wchar_t *end1 = wcschr(pBuffer, L';') + 1;
		wchar_t *end2 = wcschr(end1, L';') + 1;
		int length = wcslen(pBuffer);
		for (int j = 0; j < length; j++) {
			if (pBuffer[j] == L';')
				pBuffer[j] = L'\0';
		}
		m_clientlist.InsertItem(i, pBuffer);
		m_clientlist.SetItemText(i, 1, end1);
		m_clientlist.SetItemText(i, 2, end2);

		free(pBuffer);
	}
	freeArgs(argv, argc);
}


void CcclientDlg::OnEnChangePassedit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

bool CcclientDlg::backCommand()
{
	
	if (cguid) {
		char sendbuf[64];
		sprintf(sendbuf, "back %s\r\n", cguid);
		int count = send(CSocket, sendbuf, strlen(sendbuf), 0);
		if (count < 0) {
			return false;
		}
		ZeroMemory(sendbuf, sizeof(sendbuf));
		count = recv(CSocket, sendbuf, sizeof(sendbuf), 0);
		if (count < 0) {
			return false;
		}
		if (!strcmp(sendbuf, "+OK\r\n")) {
			return true;
		}
			
	}
	return false;
}

bool CcclientDlg::lsCommand()
{
	char buf[1024 * 16];
	TCHAR argvBuf[1024];
	if (cguid) {
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "ls %s\r\n", cguid);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count < 0) {
			return false;
		}
		memset(buf, 0, sizeof(buf));
		count = recv(CSocket, buf, sizeof(buf), 0);
		if (count < 0) {
			return false;
		}
		if (!strncmp(buf, "-", 1))
			return false;
		char **argv = splitArgs(buf, count);
		m_filelist.DeleteAllItems();
		for (int i = 0; i < count && argv; i++) {
			int length = MultiByteToWideChar(CP_ACP, 0, argv[i], -1, NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, argv[i], -1, argvBuf, length);


			if (argvBuf[0] == L'/') {
				m_filelist.InsertItem(i, argvBuf + 1, 1);
				m_filelist.SetItemText(i, 1, L"文件夹");
			}
			else if (argvBuf[0] == L'?') {
				m_filelist.InsertItem(i, argvBuf + 1, 2);
				m_filelist.SetItemText(i, 1, L"加密文件");
				//m_filelist.InsertItem(i, L"文件");
			}
			else {
				m_filelist.InsertItem(i, argvBuf, 0);
				m_filelist.SetItemText(i, 1, L"文件");
				//m_filelist.InsertItem(i, L"文件");
			}

			//m_filelist.SetItemText(i, 1, end1);
			//m_filelist.SetItemText(i, 2, end2);
		}
		freeArgs(argv, count);
	}
		
	return false;
}

bool CcclientDlg::upCommand()
{
	if (cguid) {
		char sendbuf[64];
		sprintf(sendbuf, "up %s\r\n", cguid);
		send(CSocket, sendbuf, strlen(sendbuf), 0);
		ZeroMemory(sendbuf, sizeof(sendbuf));
		recv(CSocket, sendbuf, sizeof(sendbuf), 0);
		if (!strcmp(sendbuf, "+OK\r\n")) {
			return true;
		}	
	}
	return false;
}

bool CcclientDlg::cdCommand(LPCWSTR directory)
{

	char buf[1024 * 16];
	TCHAR argvBuf[1024];

	int n = WideCharToMultiByte(CP_ACP, 0, directory, -1, NULL, 0, NULL, NULL);
	char *pBuffer = (char*)malloc(n * sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, directory, -1, pBuffer, n * sizeof(char), NULL, NULL);

	if (cguid) {
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "cd  %s %s\r\n", cguid, directory);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count < 0) {
			return false;
		}
		memset(buf, 0, sizeof(buf));
		count = recv(CSocket, buf, sizeof(buf), 0);
		if (count < 0) {
			return false;
		}
		if (!strncmp(buf, "-", 1))
			return false;
		if (strcmp(buf, "+OK\r\n"))
			return true;
	}

	return false;
}


void CcclientDlg::OnBnClickedBack()
{
	// TODO: 在此添加控件通知处理程序代码
	backCommand();
	lsCommand();
}


void CcclientDlg::OnBnClickedUp()
{
	// TODO: 在此添加控件通知处理程序代码
	upCommand();
	lsCommand();
}


void CcclientDlg::OnBnClickedGo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_pwd.GetWindowText(str);

	if (cdCommand(str)){
		m_pwd.SetWindowText(str);
	}
	else
		lsCommand();
}

//void CcclientDlg::getPwdCommand()
//{
//	char buf[1024];
//	TCHAR argvBuf[1024];
//	if (cguid) {
//		memset(buf, 0, sizeof(buf));
//		sprintf(buf, "pwd %s\r\n", cguid);
//		int count = send(CSocket, buf, strlen(buf), 0);
//		if (count < 0) {
//			return;
//		}
//		memset(buf, 0, sizeof(buf));
//		count = recv(CSocket, buf, sizeof(buf), 0);
//		if (count < 0) {
//			return;
//		}
//		if (!strncmp(buf, "-", 1))
//			return;
//		int length = MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, 0);
//		MultiByteToWideChar(CP_ACP, 0, buf, -1, argvBuf, length);
//		m_pwd.SetWindowText(argvBuf);
//	}
//
//	return;
//}
