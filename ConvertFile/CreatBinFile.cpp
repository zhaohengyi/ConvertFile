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

	//打开需要转换的文件
	hFile =  CreateFile(src, GENERIC_READ, FILE_SHARE_READ, 0,\
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		goto _ERROR;

	FileSize = GetFileSize(hFile, &hsize);

	ReadBuffer = (BYTE *)malloc(FileSize + 16);
	memset(ReadBuffer, '\0', FileSize + 10);

	if( FALSE == ReadFile(hFile, (LPVOID)ReadBuffer, FileSize, ReadSize, NULL))
	{
		MessageBox(_T("读取文件错误"));
		goto _ERROR;
	}

	if ((0x0D != ReadBuffer[(*ReadSize) - 2]) && (0x0A != ReadBuffer[(*ReadSize) - 1]))
	{
		ReadBuffer[*ReadSize] = 0x0D;
		ReadBuffer[(*ReadSize) + 1] = 0x0A;

		*ReadSize += 2;
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

	//mcs文件为二进制文件，添加的回车换行符号需要删除
	memcpy_s((this->ImageBuf + this->McsAddr), this->McsSectionSize, p, (FileSize - 2));

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

	WORD cnt = 0;

	BYTE tmp[16] = {0};
	WORD j = 0;
	for (DWORD i = 0; i < FileSize;)
	{
		if(cnt == 420)
		{
			cnt = 0;
			for (int i = 0; i < ((512 - 420) * 2); i++)
				*(pDest++) = 0;
		}
		else
		{
			if (p[i] != 0x0D)
			{
				tmp[j++] = p[i];
				i++;
			}
			else
			{
				j = (WORD)atoi((char *)tmp);
				*(pDest++) = (BYTE)((j & 0xff00)>> 8);
				*(pDest++) = (BYTE)(j & 0xff);
				i += 2;

				memset(tmp, '\0', 16);
				cnt++;
				j = 0;
			}
		}
	}

	for (WORD i = 0; i < ((512 - 420) * 2); i++)
		*(pDest++) = 0;

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
	if (this->file->TwoPointFilePath.IsEmpty())
		return FALSE;

	DWORD FileSize = 0;
	BYTE *pHead = this->ReadBinFile(this->file->TwoPointFilePath, &FileSize);
	BYTE *p = pHead;
	BYTE *pDest = (this->ImageBuf + this->TwoPointAddr);

	WORD cnt = 0;

	BYTE tmp[16] = {0};
	WORD j = 0;
	for (DWORD i = 0; i < FileSize;)
	{
		if(cnt == 420)
		{
			cnt = 0;
			for (int i = 0; i < ((512 - 420) * 2); i++)
				*(pDest++) = 0;
		}
		else
		{
			if (p[i] != 0x0D)
			{
				tmp[j++] = p[i];
				i++;
			}
			else
			{
				j = (WORD)atoi((char *)tmp);
				*(pDest++) = (BYTE)((j & 0xff00)>> 8);
				*(pDest++) = (BYTE)(j & 0xff);
				i += 2;

				memset(tmp, '\0', 16);
				cnt++;
				j = 0;
			}
		}
	}

	for (WORD i = 0; i < ((512 - 420) * 2); i++)
		*(pDest++) = 0;

	if (pHead != NULL)
	{
		p = NULL;
		free(pHead);
		pHead = NULL;
	}	

	return TRUE;
}

BOOL CCreatBinFile::ReadVerticalFile(void)
{
	if (this->file->VerticalFilePath.IsEmpty())
		return FALSE;

	DWORD FileSize = 0;
	BYTE *pHead = this->ReadBinFile(this->file->VerticalFilePath, &FileSize);
	BYTE *p = pHead;
	BYTE *pDest = (this->ImageBuf + this->VerticalAddr);

	DWORD tmp = 0;
	BYTE seps[] = {0x0D, 0x0A};
	char *next_token = NULL;

	//1、读回来的数是字符
	//2、第一行、第二行要删除
	BYTE *token = (BYTE *)strtok_s((char *)pHead, (char *)seps, &next_token);
	token = (BYTE *)strtok_s(NULL, (char *)seps, &next_token);
		
	if (token != NULL)
	{
		tmp = atoi((char *)token);
		*(pDest++) = (BYTE)((tmp & 0xff00) >> 8);
		*(pDest++) = (BYTE)tmp;	
	}
	else
		return FALSE;
	
	while (token != NULL)
	{	
		token = (BYTE *)strtok_s(NULL, (char *)seps, &next_token);
		if (token == NULL)
			break;

		tmp = atoi((char *)token);
		*(pDest++) = (	BYTE)((tmp & 0xff00) >> 8);
		*(pDest++) = (BYTE)tmp;	
	}

	return TRUE;
}

BOOL CCreatBinFile::ReadBlindFile(void)
{
	if (this->file->BlindFilePath.IsEmpty())
		return FALSE;

	DWORD FileSize = 0;
	BYTE *pHead = this->ReadBinFile(this->file->BlindFilePath, &FileSize);
	BYTE *p = pHead;
	BYTE *pDest = (this->ImageBuf + this->BlindAddr);

	DWORD x = 0;
	DWORD y = 0;
	DWORD BadPointNum = 0;
	DWORD i = 0;
	BYTE buf[16];
	memset(buf, '\0', 16);

	memset(pDest, 0, this->BlindSectionSize);		

	//读取坏点总数
	while (*p != 0x0D)
	{
		buf[i] = *(p++);
	}
	p += 2;
	BadPointNum = atoi((char *)buf);

	//跳过常0
	p += 3;
	BYTE j = 0;
	for (i = 0; i < BadPointNum; i++)
	{
		//每次取出一组点
		memset(buf, '\0', 16);
		while (*p != 0x0D)
		{
			buf[j++] = *(p++);
		}
		j = 0;
		p += 2;//跳过 0x0D 0X0A
		y = atoi((char *)buf);
		
		//取坐标y
		memset(buf, '\0', 16);
		while (*p != 0x0D)
		{
			buf[j++] = *(p++);
		}
		j = 0;
		p += 2;//跳过 0x0D 0X0A
		x = atoi((char *)buf);

		*(pDest + ((y * 512) + x) * 2) = 0;
		*(pDest + ((y * 512) + x) * 2 + 1) = 1;
	}

	if (pHead != NULL)
	{
		p = NULL;
		free(pHead);
		pHead = NULL;
	}	

	return TRUE;
}

BOOL CCreatBinFile::ReadMemsFile(void)
{
	if (this->file->MemsFilePath.IsEmpty())
		return FALSE;

	DWORD FileSize = 0;
	BYTE *pHead = this->ReadBinFile(this->file->MemsFilePath, &FileSize);
	BYTE *p = pHead;
	BYTE *pDest = (this->ImageBuf + this->MemsAddr);
	
	memset(pDest, 0, 1024);		

	BYTE seps[] = {0x0D, 0x0A};
	char *next_token = NULL;

	//1、读回来的数是字符
	//2、第一行、第二行要删除
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
		{//将地址往后偏移	
			offset = (atoi((char *)token) - 4096);
			i = 1;
		}
		else
		{//这次得到的是数据，将数据写到地址后面
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
