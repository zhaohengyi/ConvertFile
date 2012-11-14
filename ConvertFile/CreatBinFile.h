#pragma once
#include "ConvertFileDlg.h"

class CCreatBinFile:public CDialogEx
{
public:
	CCreatBinFile(void);
	CCreatBinFile(struct strFile *src);
	~CCreatBinFile(void);
public:
	BOOL Generate(void);
private:
	BOOL GenerateBinFile(void);
private:
	struct strFile *file;
};

