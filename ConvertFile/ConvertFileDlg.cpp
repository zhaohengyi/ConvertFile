
// ConvertFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ConvertFile.h"
#include "ConvertFileDlg.h"
#include "afxdialogex.h"
#include <stdlib.h>
#include <stdio.h>
#include "CreatBinFile.h"
#include "ConvertRatio.h"

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
	ON_BN_CLICKED(IDCONVERT, &CConvertFileDlg::OnBnClickedConvert)
	ON_BN_CLICKED(IDC_TAB_OCC_386, &CConvertFileDlg::OnBnClickedTabOcc386)
	ON_BN_CLICKED(IDC_TAB_OCC_384, &CConvertFileDlg::OnBnClickedTabOcc384)
	ON_BN_CLICKED(IDC_TAB_BIN, &CConvertFileDlg::OnBnClickedTabBin)
	ON_BN_CLICKED(IDOPEN_MCS, &CConvertFileDlg::OnBnClickedMcs)
	ON_BN_CLICKED(IDOPEN_OCC, &CConvertFileDlg::OnBnClickedOcc)
	ON_BN_CLICKED(IDOPEN_TWO_POINT, &CConvertFileDlg::OnBnClickedTwoPoint)
	ON_BN_CLICKED(IDOPEN_VER, &CConvertFileDlg::OnBnClickedVer)
	ON_BN_CLICKED(IDOPEN_BLIND_TAB, &CConvertFileDlg::OnBnClickedBlindTab)
	ON_BN_CLICKED(IDOPEN_MEMS, &CConvertFileDlg::OnBnClickedMems)
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
	this->dlg = dlg = new CFileDialog(TRUE, _T("new"), _T("*.bin"),
									  OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, 
									  _T("Raw Files (*.bin)|*.bin|All Files (*.*)|*.*||"));
	FileList = new struct strFile;

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

void CConvertFileDlg::SafeFree()
{
	if (this->dlg != NULL)
	{
		delete(this->dlg);
		this->dlg = NULL;
	}

	if (this->FileList != NULL)
	{
		delete(this->FileList);
		this->FileList = NULL;
	}

	if (this->dlg != NULL)
	{
		delete(dlg);
		dlg = NULL;
	}
}

void CConvertFileDlg::OnBnClickedConvert()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL ret = 0;
	CFileDialog *dlg = NULL;
	CCreatBinFile *CreatBinFile = NULL;
	ConvertRatio *convertRation = NULL;
	
	dlg = new CFileDialog(FALSE, _T("new"), _T("*.bin"),
						  OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, 
						  _T("Raw Files (*.bin)|*.raw|All Files (*.*)|*.*||"));

	if (IDOK == dlg->DoModal())
	{
		this->FileList->DestFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_SAVE, this->FileList->DestFilePath);

		this->FileList->type = this->fileType;

		if (fileType == FileIsBin)
		{
			CreatBinFile = new CCreatBinFile(this->FileList);
			ret = CreatBinFile->Generate();
		}
		else
		{
			convertRation = new ConvertRatio(this->FileList);
			ret = convertRation->Generate();
		}
		if (ret)
			MessageBox(_T("转换完成"));
		else
			MessageBox(_T(" 转换错误"));
	}

	if (convertRation != NULL)
	{
		delete(convertRation);
		convertRation = NULL;
	}
	if (CreatBinFile == NULL)
	{
		delete(CreatBinFile);
		CreatBinFile = NULL;
	}

	this->SafeFree();
}


void CConvertFileDlg::OnBnClickedTabOcc386()
{
	// TODO: 在此添加控件通知处理程序代码
	fileType = FileIsOcc_386;

	((CButton*)GetDlgItem(IDOPEN_MCS))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_TWO_POINT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_VER))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_BLIND_TAB))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_MEMS))->EnableWindow(FALSE);
}


void CConvertFileDlg::OnBnClickedTabOcc384()
{
	// TODO: 在此添加控件通知处理程序代码
	fileType = FileIsOcc_384;

	((CButton*)GetDlgItem(IDOPEN_MCS))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_TWO_POINT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_VER))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_BLIND_TAB))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_MEMS))->EnableWindow(FALSE);
}


void CConvertFileDlg::OnBnClickedTabBin()
{
	// TODO: 在此添加控件通知处理程序代码
	fileType = FileIsBin;

	((CButton*)GetDlgItem(IDOPEN_MCS))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDOPEN_OCC))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDOPEN_TWO_POINT))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDOPEN_VER))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDOPEN_BLIND_TAB))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDOPEN_MEMS))->EnableWindow(TRUE);
}


void CConvertFileDlg::OnBnClickedMcs()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK == dlg->DoModal())
	{
		this->FileList->MCSFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_MSC, this->FileList->MCSFilePath);
	
		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}


void CConvertFileDlg::OnBnClickedOcc()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK == dlg->DoModal())
	{
		this->FileList->OCCFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_OCC, this->FileList->OCCFilePath);

		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}


void CConvertFileDlg::OnBnClickedTwoPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK == dlg->DoModal())
	{
		this->FileList->TwoPointFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_TWO_POINT, this->FileList->TwoPointFilePath);

		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}


void CConvertFileDlg::OnBnClickedVer()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK == dlg->DoModal())
	{
		this->FileList->VerticalFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_VER, this->FileList->VerticalFilePath);

		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}


void CConvertFileDlg::OnBnClickedBlindTab()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK == dlg->DoModal())
	{
		this->FileList->BlindFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_BLIND, this->FileList->BlindFilePath);

		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}


void CConvertFileDlg::OnBnClickedMems()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK == dlg->DoModal())
	{
		this->FileList->MemsFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_MEMS, this->FileList->MemsFilePath);

		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}
