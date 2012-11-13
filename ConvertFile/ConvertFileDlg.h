
// ConvertFileDlg.h : 头文件
//

#pragma once

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
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedConvert();
	enum FileType {FileIsOcc_386,FileIsOcc_384, FileIsBin};
private:
	bool Convert(CString & dest);
	bool ConvertOCCTable(BYTE *src, FileType ftype, const CString & dest);
	BYTE ConvertByte(BYTE data);
	bool CreateBinFile(BYTE *src, int size, const CString & dest);
	BOOL InsertOneData(HANDLE hFile);
private:
	CString src;
	DWORD FileSize;
	FileType fileType;
public:
	afx_msg void OnBnClickedTabOcc386();
	afx_msg void OnBnClickedTabOcc384();
	afx_msg void OnBnClickedTabBin();
};
