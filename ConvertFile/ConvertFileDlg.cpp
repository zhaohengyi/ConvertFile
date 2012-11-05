
// ConvertFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConvertFile.h"
#include "ConvertFileDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CConvertFileDlg �Ի���



CConvertFileDlg::CConvertFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConvertFileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConvertFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CConvertFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOPEN, &CConvertFileDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDCONVERT, &CConvertFileDlg::OnBnClickedConvert)
END_MESSAGE_MAP()


// CConvertFileDlg ��Ϣ�������

BOOL CConvertFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CConvertFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CConvertFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CConvertFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CConvertFileDlg::OnBnClickedOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog *dlg = new CFileDialog(TRUE, _T("new"), _T("*.bin"),
      OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("Raw Files (*.bin)|*.bin|All Files (*.*)|*.*||"));
	if (IDOK == dlg->DoModal())
	{
		src = dlg->GetPathName();
		SetDlgItemText(IDC_SRC, src);

		delete(dlg);
		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}

void CConvertFileDlg::OnBnClickedConvert()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString PathName;
	CFileDialog *dlg = new CFileDialog(FALSE, _T("new"), _T("*.bin"),
										OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, 
										_T("Raw Files (*.bin)|*.raw|All Files (*.*)|*.*||"));
	if (IDOK == dlg->DoModal())
	{
		PathName = dlg->GetPathName();
		SetDlgItemText(IDC_SAVE, PathName);
		delete(dlg);
	}

	if (this->Convert(PathName))
	{
		MessageBox(_T("ת�����"));
	}
	else
	{	
		MessageBox(_T(" ת������"));
	}
}

//ʹ���ڴ�ӳ���������ļ�
bool CConvertFileDlg::Convert(CString & dest)
{
	//����Ҫת�����ļ�
	HANDLE hFile =  CreateFile(src, GENERIC_READ, FILE_SHARE_READ, 0,\
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if ((INVALID_HANDLE_VALUE == hFileMap) || (!hFileMap))
	{
		CloseHandle(hFile);
		return false;
	}

	if (NULL == MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0))
	{
		CloseHandle(hFile);
		CloseHandle(hFileMap);
		return false;
	}

	//��������ļ�
	HANDLE hNewFile = CreateFile(dest, GENERIC_WRITE, FILE_SHARE_READ, 0, \
								CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hNewFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		UnmapViewOfFile(hFileMap);
		CloseHandle(hFileMap);
		return false;
	}

	HANDLE hNewFileMap = CreateFileMapping(hNewFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if ((INVALID_HANDLE_VALUE == hNewFileMap) || (!hNewFileMap))
	{
		CloseHandle(hFile);
		UnmapViewOfFile(hFileMap);
		CloseHandle(hFileMap);

		CloseHandle(hNewFile);
		return false;
	}

	if (NULL == MapViewOfFile(hNewFileMap, FILE_MAP_READ, 0, 0, 0))
	{
		CloseHandle(hFile);
		UnmapViewOfFile(hFileMap);
		CloseHandle(hFileMap);

		CloseHandle(hNewFile);
		CloseHandle(hNewFileMap);
		return false;
	}

	LPBYTE pSrc = (LPBYTE)hFileMap;
	LPBYTE pDst = (LPBYTE)hNewFileMap;
	
	for (int i = 0; i < 384; i++)
	{
		for (int  j = 0; j < 288;  j++)
		{

			//TODO:

		}
	}

	FlushViewOfFile(hNewFileMap, 420*315);
	
	CloseHandle(hFile);
	UnmapViewOfFile(hFileMap);
	CloseHandle(hFileMap);

	CloseHandle(hNewFile);
	UnmapViewOfFile(hNewFileMap);
	CloseHandle(hNewFileMap);
	return true;
}
