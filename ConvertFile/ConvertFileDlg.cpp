
// ConvertFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConvertFile.h"
#include "ConvertFileDlg.h"
#include "afxdialogex.h"
#include <stdlib.h>
#include <stdio.h>

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
	((CButton *)GetDlgItem(IDC_TAB_OCC))->SetCheck(TRUE);//ѡ��
	
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

bool CConvertFileDlg::ConvertOCCTable(BYTE *src, int size, const CString & dest)
{
	BYTE *p = src;
	BYTE cnt = 0;	
	DWORD dwByteWrite = 0;
	BYTE tmpBuf[11];

	if (p == NULL)
		return false;

	//��������ļ�
	HANDLE hNewFile = CreateFile(dest, GENERIC_WRITE, FILE_SHARE_READ, 0, \
								CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hNewFile == INVALID_HANDLE_VALUE)
		return false;

	for (WORD j = 0; j < 384; j++)
	{
		for (WORD i = 0; i < 288; i++)
		{	
			if (10 == cnt)
			{//��ʮ����ʱ���ſ�ʼд����
				cnt = 0;
				tmpBuf[10] = (BYTE)(((tmpBuf[0] + tmpBuf[2] + tmpBuf[5] + tmpBuf[8] + tmpBuf[9])/5) & 0xFF);
				WriteFile(hNewFile, tmpBuf, 11, &dwByteWrite, NULL);			
			}
			else
			{
				tmpBuf[cnt++] = *(p++);	
			}
		}

		//ÿһ��ʣ�µ�20������
		for (BYTE i = 0; i < 20; i++)
			WriteFile(hNewFile, p++, 1, &dwByteWrite, NULL);			

		//����386*289�ֱ��ʵ����ݣ�ÿ�к�����Ҫ�ټ�2�ֽ�����
		if (111554 == size)
		{
			tmpBuf[0] = (BYTE)((~((tmpBuf[7] + tmpBuf[1] + tmpBuf[3])/3)) & 0xff);
			tmpBuf[1] = (BYTE)((~((tmpBuf[4] + tmpBuf[6] + tmpBuf[9])/3)) & 0xff);
			WriteFile(hNewFile, tmpBuf, 2, &dwByteWrite, NULL);			
		}	

		//ʣ�µ��У�һ��д��
		if (270 == j)
		{
			WriteFile(hNewFile, p, (18 * 288), &dwByteWrite, NULL);			
			if (111554 == size)
			{//����386*289�ֱ��ʵ��ļ�����Ҫ�ټ���һ������
				for (DWORD i = 0; i < 315; i++)
				{
					tmpBuf[0] = (BYTE)((GetTickCount() & 0xff00) + (GetTickCount() & 0xff));
					WriteFile(hNewFile, tmpBuf, 1, &dwByteWrite, NULL);			
				}				
			}
			return true;
		}

		//����һ������
		for (DWORD i = 0; i < 315; i++)
		{
			tmpBuf[0] = (BYTE)((GetTickCount() & 0xff00) + (GetTickCount() & 0xff));
			WriteFile(hNewFile, tmpBuf, 1, &dwByteWrite, NULL);			
		}
		if (111554 == size)
		{//����386*289�ֱ��ʵ����ݣ�ÿ�к�����Ҫ�ټ�2�ֽ�����
			tmpBuf[0] = (BYTE)((~((tmpBuf[7] + tmpBuf[1] + tmpBuf[3])/3)) & 0xff);
			tmpBuf[1] = (BYTE)((~((tmpBuf[4] + tmpBuf[6] + tmpBuf[9])/3)) & 0xff);
			WriteFile(hNewFile, tmpBuf, 2, &dwByteWrite, NULL);			
		}
	}

	return true;
}

bool CConvertFileDlg::CreateBinFile(BYTE *src, int size, const CString & dest)
{
	BYTE *p = src;
	DWORD dwByteWrite = 0;

	//��������ļ�
	HANDLE hNewFile = CreateFile(dest, GENERIC_WRITE, FILE_SHARE_READ, 0, \
								CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hNewFile == INVALID_HANDLE_VALUE)
		return false;
	
	WriteFile(hNewFile, p, 1, &dwByteWrite, NULL);
	return true;
}

bool CConvertFileDlg::Convert(CString & dest)
{	
	bool reslut = false;
	DWORD hsize = 0;
	DWORD hFileSize = 0;
	DWORD dwBytesRead = 0;
	
	//����Ҫת�����ļ�
	HANDLE hFile =  CreateFile(src, GENERIC_READ, FILE_SHARE_READ, 0,\
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	hFileSize = GetFileSize(hFile, &hsize);
	
	BYTE *ReadBuffer = (BYTE *)malloc(hFileSize + 10);
	memset(ReadBuffer, '\0', hFileSize + 10);

	if( FALSE == ReadFile(hFile, (LPVOID)ReadBuffer, hFileSize, &dwBytesRead, NULL))
    {
		MessageBox(_T("��ȡ�ļ����� %wd"));
        CloseHandle(hFile);
        return false;
    }

	//ȥ���س����з���
	BYTE *tmpBuffer =  (BYTE *)malloc(hFileSize + 10);
	BYTE *ptmp = tmpBuffer;
	memset(tmpBuffer, '\0', hFileSize + 10);
	for (DWORD i = 0; i < hFileSize; i++)
	{
		if((ReadBuffer[i] != 0x0D) && (ReadBuffer[i] != 0x0A))
			*(tmpBuffer++) = ReadBuffer[i];
	}
	
	if (((CButton *)GetDlgItem(IDC_TAB_OCC))->GetCheck())
	{
		//fileType = FileIsOcc;
		reslut = this->ConvertOCCTable(ptmp, hFileSize, dest);
	}
	else
	{
		//fileType = FileIsBin;
		reslut = this->CreateBinFile(ptmp, hFileSize, dest);
	}

	free(ReadBuffer);
	ReadBuffer = NULL;
	CloseHandle(hFile);
	return reslut;
}
