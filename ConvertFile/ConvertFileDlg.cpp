
// ConvertFileDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CConvertFileDlg 对话框



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


// CConvertFileDlg 消息处理程序

BOOL CConvertFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	((CButton *)GetDlgItem(IDC_TAB_OCC_386))->SetCheck(TRUE);//选上
	this->fileType = FileIsOcc_386;
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CConvertFileDlg::OnPaint()
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
HCURSOR CConvertFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CConvertFileDlg::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
			MessageBox(_T("转换完成"));
		}
		else
		{	
			MessageBox(_T(" 转换错误"));
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

	//创建输出文件
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
			{//前360个点
				while (*p != 0x0D)
				{
					tmpBuf[cnt++]	= *(p++);
				}
				tmpBuf[cnt++] = *(p++);
				tmpBuf[cnt++] = *(p++);

				num++;

				if (10 == num)
				{//第十个点时，才开始写数据
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
			{//剩下的24个点
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

		//对于386*289分辨率的数据，加上剩下的两个数据
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

		//前270行，每隔10行插入一行数据
		if ((j < 270) && (9 == line))
		{
			for (DWORD i = 0; i < 420; i++)
			{
				this->InsertOneData(hNewFile);
			}
				
			if (FileIsOcc_386 == ftype)
			{//对于386*289分辨率的数据，每行后面需要再加2个数据
				this->InsertOneData(hNewFile);
				this->InsertOneData(hNewFile);
			}

			line = 0;
		}
		else
			line++;
	}

	if (FileIsOcc_386 == ftype)
	{//对于386*289分辨率的文件，需要再加上一行数据
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

	//创建输出文件
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
	
	//打开需要转换的文件
	HANDLE hFile =  CreateFile(src, GENERIC_READ, FILE_SHARE_READ, 0,\
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	FileSize = GetFileSize(hFile, &hsize);
	
	BYTE *ReadBuffer = (BYTE *)malloc(FileSize + 10);
	memset(ReadBuffer, '\0', FileSize + 10);

	if( FALSE == ReadFile(hFile, (LPVOID)ReadBuffer, FileSize, &dwBytesRead, NULL))
    {
		MessageBox(_T("读取文件错误 %wd"));
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
	// TODO: 在此添加控件通知处理程序代码
	fileType = FileIsOcc_386;
}


void CConvertFileDlg::OnBnClickedTabOcc384()
{
	// TODO: 在此添加控件通知处理程序代码
	fileType = FileIsOcc_384;
}


void CConvertFileDlg::OnBnClickedTabBin()
{
	// TODO: 在此添加控件通知处理程序代码
	fileType = FileIsBin;
}
