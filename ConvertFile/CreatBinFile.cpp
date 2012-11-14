#include "StdAfx.h"
#include "CreatBinFile.h"


CCreatBinFile::CCreatBinFile(void)
{
}

CCreatBinFile::CCreatBinFile(CString *src, CString &dest, BYTE cnt)
{
	;
}

CCreatBinFile::~CCreatBinFile(void)
{
}

BOOL CCreatBinFile::CreateBinFile()
{
	//BYTE *p = src;
	//DWORD dwByteWrite = 0;

	////创建输出文件
	//HANDLE hNewFile = CreateFile(dest, GENERIC_WRITE, FILE_SHARE_READ, 0, \
	//							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	//if (hNewFile == INVALID_HANDLE_VALUE)
	//	return false;
	//
	//WriteFile(hNewFile, p, 1, &dwByteWrite, NULL);
	return true;
}


BOOL CCreatBinFile::Create()
{
	return false;
}
