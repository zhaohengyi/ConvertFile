
// ConvertFileDlg.h : 头文件
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

// CConvertFileDlg 对话框
class CConvertFileDlg : public CDialogEx
{
// 构造
public:
	CConvertFileDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CONVERTFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
