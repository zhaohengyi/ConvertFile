#include "StdAfx.h"
#include "CreatBinFile.h"


CCreatBinFile::CCreatBinFile(void)
{
	this->file = NULL;
}

CCreatBinFile::~CCreatBinFile(void)
{
}

CCreatBinFile::CCreatBinFile(struct strFile *src)
{
	this->file = src;
}

BOOL CCreatBinFile::GenerateBinFile()
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


BOOL CCreatBinFile::Generate()
{
	return false;
}
