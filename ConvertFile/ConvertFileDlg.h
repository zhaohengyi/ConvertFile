
// ConvertFileDlg.h : ͷ�ļ�
//

#pragma once

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
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedConvert();
	enum FileType {FileIsOcc, FileIsBin};
private:
	bool Convert(CString & dest);
	bool ConvertOCCTable(BYTE *src, int size, const CString & dest);
	bool CreateBinFile(BYTE *src, int size, const CString & dest);
private:
	CString src;
	//FileType fileType;
};
