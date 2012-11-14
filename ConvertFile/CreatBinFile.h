#pragma once
class CCreatBinFile
{
public:
	CCreatBinFile(void);
	CCreatBinFile(CString *src, CString &dest, BYTE cnt);
	~CCreatBinFile(void);
public:
	BOOL Create();
private:
	BOOL CreateBinFile();
};

