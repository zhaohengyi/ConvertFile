
// ConvertFileDlg.cpp : ʵ���ļ�
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
	this->dlg = dlg = new CFileDialog(TRUE, _T("new"), _T("*.bin"),
									  OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, 
									  _T("Raw Files (*.bin)|*.bin|All Files (*.*)|*.*||"));
	FileList = new struct strFile;

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			MessageBox(_T("ת�����"));
		else
			MessageBox(_T(" ת������"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	fileType = FileIsOcc_386;

	((CButton*)GetDlgItem(IDOPEN_MCS))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_TWO_POINT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_VER))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_BLIND_TAB))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_MEMS))->EnableWindow(FALSE);
}


void CConvertFileDlg::OnBnClickedTabOcc384()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	fileType = FileIsOcc_384;

	((CButton*)GetDlgItem(IDOPEN_MCS))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_TWO_POINT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_VER))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_BLIND_TAB))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOPEN_MEMS))->EnableWindow(FALSE);
}


void CConvertFileDlg::OnBnClickedTabBin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (IDOK == dlg->DoModal())
	{
		this->FileList->MCSFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_MSC, this->FileList->MCSFilePath);
	
		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}


void CConvertFileDlg::OnBnClickedOcc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (IDOK == dlg->DoModal())
	{
		this->FileList->OCCFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_OCC, this->FileList->OCCFilePath);

		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}


void CConvertFileDlg::OnBnClickedTwoPoint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (IDOK == dlg->DoModal())
	{
		this->FileList->TwoPointFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_TWO_POINT, this->FileList->TwoPointFilePath);

		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}


void CConvertFileDlg::OnBnClickedVer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (IDOK == dlg->DoModal())
	{
		this->FileList->VerticalFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_VER, this->FileList->VerticalFilePath);

		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}


void CConvertFileDlg::OnBnClickedBlindTab()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (IDOK == dlg->DoModal())
	{
		this->FileList->BlindFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_BLIND, this->FileList->BlindFilePath);

		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}


void CConvertFileDlg::OnBnClickedMems()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (IDOK == dlg->DoModal())
	{
		this->FileList->MemsFilePath = dlg->GetPathName();
		SetDlgItemText(IDC_MEMS, this->FileList->MemsFilePath);

		((CButton*)GetDlgItem(IDCONVERT))->EnableWindow(TRUE);
	}
}
