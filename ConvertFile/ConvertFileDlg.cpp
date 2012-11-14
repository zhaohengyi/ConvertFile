
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
	ON_BN_CLICKED(IDC_TAB_OCC_386, &CConvertFileDlg::OnBnClickedTabOcc386)
	ON_BN_CLICKED(IDC_TAB_OCC_384, &CConvertFileDlg::OnBnClickedTabOcc384)
	ON_BN_CLICKED(IDC_TAB_BIN, &CConvertFileDlg::OnBnClickedTabBin)
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
	((CButton *)GetDlgItem(IDC_TAB_OCC_386))->SetCheck(TRUE);//ѡ��
	this->fileType = FileIsOcc_386;
	
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
	
		if (this->Convert(PathName))
		{
			MessageBox(_T("ת�����"));
		}
		else
		{	
			MessageBox(_T(" ת������"));
		}
	}
}

BYTE CConvertFileDlg::ConvertByte(BYTE data)
{	
	if (data > 57)
	{
		while (data > 57)
		{
			data--;
		}
	}
	else if (data < 48)
	{
		while (data < 48)
		{
			data++;
		}
	}
	
	return data;
}

BOOL CConvertFileDlg::InsertOneData(HANDLE hFile)
{
	BYTE 	tmpBuf[3];
	DWORD	dwByteWrite;

	tmpBuf[0] = ConvertByte((BYTE)((GetTickCount() & 0xff00) + (GetTickCount() & 0xff)));
	tmpBuf[1] = 0x0D;
	tmpBuf[2] = 0x0A;
	return WriteFile(hFile, tmpBuf, 3, &dwByteWrite, NULL);		
}

bool CConvertFileDlg::ConvertOCCTable(BYTE *src, FileType ftype, const CString & dest)
{
	BYTE *p = src;
	BYTE cnt = 0;	
	BYTE tmpBuf[128];
	BYTE line = 0;
	BYTE num = 0;
	DWORD dwByteWrite = 0;

	if (p == NULL)
		return false;

	//��������ļ�
	HANDLE hNewFile = CreateFile(dest, GENERIC_WRITE, FILE_SHARE_READ, 0, \
								CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hNewFile == INVALID_HANDLE_VALUE)
		return false;

	for (WORD j = 0; j < 288; j++)
	{
		memset(tmpBuf, '\0',128);
		num = 0;
		cnt = 0;
		for (WORD i = 0; i < 384; i++)
		{	
			if (i < 360)
			{//ǰ360����
				while (*p != 0x0D)
				{
					tmpBuf[cnt++]	= *(p++);
				}
				tmpBuf[cnt++] = *(p++);
				tmpBuf[cnt++] = *(p++);

				num++;

				if (10 == num)
				{//��ʮ����ʱ���ſ�ʼд����
					num = 0;
					tmpBuf[cnt++] = ConvertByte((BYTE)((GetTickCount() & 0xff00) + (GetTickCount() & 0xff)));
					tmpBuf[cnt++] = 0x0D;
					tmpBuf[cnt++] = 0x0A;
					WriteFile(hNewFile, tmpBuf, cnt, &dwByteWrite, NULL);			
				
					cnt = 0;
					memset(tmpBuf, '\0',128);
				}	
			}
			else
			{//ʣ�µ�24����
				cnt = 0;
				while (*p != 0x0D)
				{
					tmpBuf[cnt++]	= *(p++);
				}
			
				tmpBuf[cnt++] = *(p++);
				tmpBuf[cnt++] = *(p++);

				WriteFile(hNewFile, tmpBuf, cnt, &dwByteWrite, NULL);			
			}
		}		

		//����386*289�ֱ��ʵ����ݣ�����ʣ�µ���������
		if (FileIsOcc_386 == ftype)
		{
			for (BYTE i = 0; i < 2; i++)
			{
				cnt = 0;
				while (*p != 0x0D)
				{
					tmpBuf[cnt++]	= *(p++);
				}
			
				tmpBuf[cnt++] = *(p++);
				tmpBuf[cnt++] = *(p++);

				WriteFile(hNewFile, tmpBuf, cnt, &dwByteWrite, NULL);			
			}
		}	

		//ǰ270�У�ÿ��10�в���һ������
		if ((j < 270) && (9 == line))
		{
			for (DWORD i = 0; i < 420; i++)
			{
				this->InsertOneData(hNewFile);
			}
				
			if (FileIsOcc_386 == ftype)
			{//����386*289�ֱ��ʵ����ݣ�ÿ�к�����Ҫ�ټ�2������
				this->InsertOneData(hNewFile);
				this->InsertOneData(hNewFile);
			}

			line = 0;
		}
		else
			line++;
	}

	if (FileIsOcc_386 == ftype)
	{//����386*289�ֱ��ʵ��ļ�����Ҫ�ټ���һ������
		for (DWORD i = 0; i < 422; i++)
			this->InsertOneData(hNewFile);
	}
	
	CloseHandle(hNewFile);
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
	DWORD tableSize = 0;
	DWORD dwBytesRead = 0;
	
	//����Ҫת�����ļ�
	HANDLE hFile =  CreateFile(src, GENERIC_READ, FILE_SHARE_READ, 0,\
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	FileSize = GetFileSize(hFile, &hsize);
	
	BYTE *ReadBuffer = (BYTE *)malloc(FileSize + 10);
	memset(ReadBuffer, '\0', FileSize + 10);

	if( FALSE == ReadFile(hFile, (LPVOID)ReadBuffer, FileSize, &dwBytesRead, NULL))
    {
		MessageBox(_T("��ȡ�ļ����� %wd"));
        CloseHandle(hFile);
        return false;
    }

	switch(fileType)
	{
	case FileIsOcc_384:
	case FileIsOcc_386:
		reslut = this->ConvertOCCTable(ReadBuffer, fileType, dest);break;
	case FileIsBin:
		reslut = this->CreateBinFile(ReadBuffer, FileSize, dest);break;
	default:break;
	}

	free(ReadBuffer);
	ReadBuffer = NULL;
	CloseHandle(hFile);
	return reslut;
}


void CConvertFileDlg::OnBnClickedTabOcc386()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	fileType = FileIsOcc_386;
}


void CConvertFileDlg::OnBnClickedTabOcc384()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	fileType = FileIsOcc_384;
}


void CConvertFileDlg::OnBnClickedTabBin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	fileType = FileIsBin;
}
