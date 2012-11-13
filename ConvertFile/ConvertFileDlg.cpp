
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
	((CButton *)GetDlgItem(IDC_TAB_OCC))->SetCheck(TRUE);//选上
	
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
	}

	if (this->Convert(PathName))
	{
		MessageBox(_T("转换完成"));
	}
	else
	{	
		MessageBox(_T(" 转换错误"));
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

	//创建输出文件
	HANDLE hNewFile = CreateFile(dest, GENERIC_WRITE, FILE_SHARE_READ, 0, \
								CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hNewFile == INVALID_HANDLE_VALUE)
		return false;

	for (WORD j = 0; j < 384; j++)
	{
		for (WORD i = 0; i < 288; i++)
		{	
			if (10 == cnt)
			{//第十个点时，才开始写数据
				cnt = 0;
				tmpBuf[10] = (BYTE)(((tmpBuf[0] + tmpBuf[2] + tmpBuf[5] + tmpBuf[8] + tmpBuf[9])/5) & 0xFF);
				WriteFile(hNewFile, tmpBuf, 11, &dwByteWrite, NULL);			
			}
			else
			{
				tmpBuf[cnt++] = *(p++);	
			}
		}

		//每一行剩下的20个数据
		for (BYTE i = 0; i < 20; i++)
			WriteFile(hNewFile, p++, 1, &dwByteWrite, NULL);			

		//对于386*289分辨率的数据，每行后面需要再加2字节数据
		if (111554 == size)
		{
			tmpBuf[0] = (BYTE)((~((tmpBuf[7] + tmpBuf[1] + tmpBuf[3])/3)) & 0xff);
			tmpBuf[1] = (BYTE)((~((tmpBuf[4] + tmpBuf[6] + tmpBuf[9])/3)) & 0xff);
			WriteFile(hNewFile, tmpBuf, 2, &dwByteWrite, NULL);			
		}	

		//剩下的行，一次写入
		if (270 == j)
		{
			WriteFile(hNewFile, p, (18 * 288), &dwByteWrite, NULL);			
			if (111554 == size)
			{//对于386*289分辨率的文件，需要再加上一行数据
				for (DWORD i = 0; i < 315; i++)
				{
					tmpBuf[0] = (BYTE)((GetTickCount() & 0xff00) + (GetTickCount() & 0xff));
					WriteFile(hNewFile, tmpBuf, 1, &dwByteWrite, NULL);			
				}				
			}
			return true;
		}

		//插入一行数据
		for (DWORD i = 0; i < 315; i++)
		{
			tmpBuf[0] = (BYTE)((GetTickCount() & 0xff00) + (GetTickCount() & 0xff));
			WriteFile(hNewFile, tmpBuf, 1, &dwByteWrite, NULL);			
		}
		if (111554 == size)
		{//对于386*289分辨率的数据，每行后面需要再加2字节数据
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
	DWORD hFileSize = 0;
	DWORD dwBytesRead = 0;
	
	//打开需要转换的文件
	HANDLE hFile =  CreateFile(src, GENERIC_READ, FILE_SHARE_READ, 0,\
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	hFileSize = GetFileSize(hFile, &hsize);
	
	BYTE *ReadBuffer = (BYTE *)malloc(hFileSize + 10);
	memset(ReadBuffer, '\0', hFileSize + 10);

	if( FALSE == ReadFile(hFile, (LPVOID)ReadBuffer, hFileSize, &dwBytesRead, NULL))
    {
		MessageBox(_T("读取文件错误 %wd"));
        CloseHandle(hFile);
        return false;
    }

	//去除回车换行符号
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
