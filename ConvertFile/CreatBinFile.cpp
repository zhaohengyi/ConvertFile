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

BYTE *CCreatBinFile::ReadBinFile(CString src, DWORD *ReadSize)
{
	BYTE *ReadBuffer = NULL;
	DWORD FileSize = 0;
	DWORD hsize = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;

	//����Ҫת�����ļ�
	hFile =  CreateFile(src, GENERIC_READ, FILE_SHARE_READ, 0,\
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		goto _ERROR;

	FileSize = GetFileSize(hFile, &hsize);

	ReadBuffer = (BYTE *)malloc(FileSize + 10);
	memset(ReadBuffer, '\0', FileSize + 10);

	if( FALSE == ReadFile(hFile, (LPVOID)ReadBuffer, FileSize, ReadSize, NULL))
	{
		MessageBox(_T("��ȡ�ļ����� %wd"));
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

BOOL CCreatBinFile::ReadNUCFile(void)
{
	if (this->file->NUCFilePath.IsEmpty())
		return FALSE;

	DWORD FileSize = 0;
	BYTE *pHead = this->ReadBinFile(this->file->NUCFilePath, &FileSize);
	BYTE *p = pHead;
	BYTE *pDest = (this->ImageBuf + this->OCCAddr);

	BYTE seps[] = {0x0D, 0x0A};
	char *next_token = NULL;
	BYTE *token = (BYTE *)strtok_s((char *)pHead, (char *)seps, &next_token);
	
	WORD tmp = atoi((char *)token);
	*(pDest++) = (BYTE)((tmp & 0xff00) >> 8);
	*(pDest++) = (BYTE)tmp;	

	WORD cnt = 0;
	
	while (token != NULL)
	{	
		if(cnt == 419)
		{//�Ѿ�������һ���㣬����Ҫ�� -1 ~ 419
			cnt = -1;
			pDest += ((512 -420) * 2);
		}
		else
		{
			cnt++;
			token = (BYTE *)strtok_s(NULL, (char *)seps, &next_token);
			if (token != NULL)
			{
				tmp = (WORD)atoi((char *)token);
				*(pDest++) = (tmp & 0xff00) >> 8;
				*(pDest++) = tmp;	
			}
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
	if (this->file->MemsFilePath.IsEmpty())
		return FALSE;

	DWORD FileSize = 0;
	BYTE *pHead = this->ReadBinFile(this->file->MemsFilePath, &FileSize);
	BYTE *p = pHead;
	BYTE *pDest = (this->ImageBuf + this->MemsAddr);
	
	BYTE seps[] = {0x0D, 0x0A};
	char *next_token = NULL;

	//1���������������ַ�
	//2����һ�С��ڶ���Ҫɾ��
	BYTE *token = (BYTE *)strtok_s((char *)pHead, (char *)seps, &next_token);
	token = (BYTE *)strtok_s(NULL, (char *)seps, &next_token);
		
	DWORD i = 0;
	DWORD offset = 0;
	while (token != NULL)
	{	
		token = (BYTE *)strtok_s(NULL, (char *)seps, &next_token);
		if (token == NULL)
			break;

		if (i == 0)
		{//����ַ����ƫ��	
			offset = (atoi((char *)token) - 4096);
			i = 1;
		}
		else
		{//��εõ��������ݣ�������д����ַ����
			i = atoi((char *)token);
			*(pDest + offset) = (	BYTE)((i & 0xff00) >> 8);
			*(pDest + offset + 1) = (BYTE)i;	

			i = 0;
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
	this->ReadNUCFile();
	this->ReadTwoPointFile();
	this->ReadVerticalFile();
	this->ReadBlindFile();
	this->ReadMemsFile();

	//��������ļ�
	HANDLE hNewFile = CreateFile(this->file->DestFilePath, GENERIC_WRITE, FILE_SHARE_READ, 0, \
								 CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if ((hNewFile == INVALID_HANDLE_VALUE) || (hNewFile == NULL))
		return false;
	

	WriteFile(hNewFile, this->ImageBuf, IMAGE_SIZE, &dwByteWrite, NULL);

	if ((hNewFile != INVALID_HANDLE_VALUE) && (hNewFile != NULL))
		CloseHandle(hNewFile);

	return TRUE;
}
