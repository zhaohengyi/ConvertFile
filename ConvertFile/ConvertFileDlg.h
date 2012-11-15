
// ConvertFileDlg.h : ͷ�ļ�
//

#include "resource.h"

#pragma once

enum FileType {FileIsOcc_386,FileIsOcc_384, FileIsBin};

struct strFile
{
	enum FileType type;
	CString MCSFilePath;
	CString OCCFilePath;
	CString TwoPointFilePath;
	CString VerticalFilePath;
	CString BlindFilePath;
	CString MemsFilePath;
	CString DestFilePath;
};

// CConvertFileDlg �Ի���
class CConvertFileDlg : public CDialogEx
{
// ����
public:
	CConvertFileDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CONVERTFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConvert();
public:
	afx_msg void OnBnClickedTabOcc386();
	afx_msg void OnBnClickedTabOcc384();
	afx_msg void OnBnClickedTabBin();
	afx_msg void OnBnClickedMcs();
	afx_msg void OnBnClickedOcc();
	afx_msg void OnBnClickedTwoPoint();
	afx_msg void OnBnClickedVer();
	afx_msg void OnBnClickedBlindTab();
	afx_msg void OnBnClickedMems();
private: 
	FileType fileType;
	struct strFile *FileList;
private:
	void SafeFree(void);
	CString GetFilePath(CString ex);
};
