#include "ConvertFileDlg.h"

#pragma once
class ConvertRatio: public CDialogEx
{
public:
	ConvertRatio(void);
	ConvertRatio(struct strFile *src);
	~ConvertRatio(void);	
public :
	BOOL Generate(void);
private:
	BOOL ConvertOCCTable(void);
	BYTE ConvertByte(BYTE data);
	BOOL InsertOneData(HANDLE hFile);
private:
	struct strFile *file;
	DWORD FileSize;
};

