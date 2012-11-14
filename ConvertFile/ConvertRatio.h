#pragma once
class ConvertRatio: public CDialogEx
{
public:
	ConvertRatio(void);
	ConvertRatio(CString &src, CString &dest);
	~ConvertRatio(void);	
public :
	BOOL Convert();
private:
	BOOL ConvertOCCTable();
	BYTE ConvertByte(BYTE data);
	BOOL InsertOneData(HANDLE hFile);
private:
	CString src;
	CString dest;
	DWORD FileSize;
};

