#include "StdAfx.h"
#include "CreatBinFile.h"

CCreatBinFile::CCreatBinFile(void)
{
	ImageBuf = NULL;
	this->file = NULL;
}

CCreatBinFile::~CCreatBinFile(void)
{
	if (ImageBuf != NULL)
	{
		free(ImageBuf);
		ImageBuf = NULL;
	}
}

CCreatBinFile::CCreatBinFile(struct strFile *src)
{
	ImageBuf = (BYTE *)malloc(IMAGE_SIZE);
	memset(ImageBuf, 0xff, IMAGE_SIZE);
	this->file = src;
}

#pragma region Read File

BYTE *CCreatBinFile::ReadBinFile(CString file, DWORD *ReadSize)
{
	BYTE *ReadBuffer = NULL;
	DWORD FileSize = 0;
	DWORD hsize = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;

	//打开需要转换的文件
	hFile =  CreateFile(this->file->OCCFilePath, GENERIC_READ, FILE_SHARE_READ, 0,\
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		goto _ERROR;

	FileSize = GetFileSize(hFile, &hsize);

	ReadBuffer = (BYTE *)malloc(FileSize + 10);
	memset(ReadBuffer, '\0', FileSize + 10);

	if( FALSE == ReadFile(hFile, (LPVOID)ReadBuffer, FileSize, ReadSize, NULL))
	{
		MessageBox(_T("读取文件错误 %wd"));
		goto _ERROR;
	}

_ERROR:
	return ReadBuffer;
}

BOOL CCreatBinFile::ReadMcsFile(void)
{
	if (this->file->MCSFilePath.IsEmpty())
		return FALSE;

	DWORD FileSize = 0;
	BYTE *p = this->ReadBinFile(this->file->MCSFilePath, &FileSize);

	memcpy_s((this->ImageBuf + this->McsAddr), this->McsSectionSzie, p, FileSize);

	if (p != NULL)
	{
		free(p);
		p = NULL;
	}

	return TRUE;
}

BOOL CCreatBinFile::ReadOCCFile(void)
{
	if (this->file->OCCFilePath.IsEmpty())
		return FALSE;

	DWORD FileSize = 0;
	BYTE *pHead = this->ReadBinFile(this->file->MCSFilePath, &FileSize);
	BYTE *p = pHead;
	BYTE *pDest = (this->ImageBuf + this->OCCAddr);

	for (DWORD i = 0; i < FileSize; i++)
	{
		if ((*p != 0x0D) && (*p != 0x0A))
		{
			pDest[i] = *p;
		}
	}

	if (pHead != NULL)
	{
		p = NULL;
		free(pHead);
		pHead = NULL;
	}	

	return TRUE;
}

BOOL CCreatBinFile::ReadTwoPointFile(void)
{
	if (this->file->TwoPointFilePath)
		return FALSE;

	return FALSE;
}

BOOL CCreatBinFile::ReadVerticalFile(void)
{
	if (this->file->VerticalFilePath)
		return FALSE;

	return FALSE;
}

BOOL CCreatBinFile::ReadBlindFile(void)
{
	if (this->file->BlindFilePath)
		return FALSE;

	return FALSE;
}

BOOL CCreatBinFile::ReadMemsFile(void)
{
	if (this->file->OCCFilePath.IsEmpty())
		return FALSE;

	DWORD FileSize = 0;
	BYTE *pHead = this->ReadBinFile(this->file->MemsFilePath, &FileSize);
	BYTE *p = pHead;
	BYTE *pDest = (this->ImageBuf + this->MemsAddr);
	
	BYTE seps[] = {0x0D, 0x0A};
	char *next_token = NULL;
	BYTE *token = (BYTE *)strtok_s((char *)pHead, (char *)seps, &next_token);

	WORD offset = *((WORD *)token);

	BYTE i = 0;
	while (token != NULL)
	{	
		token = (BYTE *)strtok_s(NULL, (char *)seps, &next_token);
		if (i == 0)
		{
			pDest += offset;
			*((WORD *)pDest) = *((WORD *)token);
			i++;
		}
		else
		{
			i = 0;
			offset = *((WORD *)token);
		}
	}

	if (pHead != NULL)
	{
		p = NULL;
		free(pHead);
		pHead = NULL;
	}	

	return TRUE;
}
#pragma endregion Read File


BOOL CCreatBinFile::Generate()
{	
	DWORD	dwByteWrite = 0;

	this->ReadMcsFile();
	this->ReadOCCFile();
	this->ReadTwoPointFile();
	this->ReadVerticalFile();
	this->ReadBlindFile();
	this->ReadMemsFile();

	//创建输出文件
	HANDLE hNewFile = CreateFile(this->file->DestFilePath, GENERIC_WRITE, FILE_SHARE_READ, 0, \
								 CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if ((hNewFile == INVALID_HANDLE_VALUE) || (hNewFile == NULL))
		return false;
	

	WriteFile(hNewFile, this->ImageBuf, IMAGE_SIZE, &dwByteWrite, NULL);

	if ((hNewFile != INVALID_HANDLE_VALUE) && (hNewFile != NULL))
		CloseHandle(hNewFile);

	return TRUE;
}
