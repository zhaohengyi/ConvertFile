#include "StdAfx.h"
#include "ConvertRatio.h"
#include <stdlib.h>


ConvertRatio::ConvertRatio(void)
{
	this->file = NULL;
}

ConvertRatio::~ConvertRatio(void)
{
}

ConvertRatio::ConvertRatio(struct strFile *src)
{
	this->file = src;
}

BOOL ConvertRatio::InsertOneData(HANDLE hFile)
{
	BYTE 	tmpBuf[16];
	DWORD	dwByteWrite;

	itoa(rand(), (char *)tmpBuf, 10);
	tmpBuf[2] = 0x0D;
	tmpBuf[3] = 0x0A;
	return WriteFile(hFile, tmpBuf, 4, &dwByteWrite, NULL);		
}

BOOL ConvertRatio::ConvertOCCTable(BYTE *src)
{
	BOOL ret = FALSE;
	BYTE cnt = 0;	
	BYTE tmpBuf[128];
	BYTE line = 0;
	BYTE num = 0;
	BYTE *p = src;

	WORD x, y;
	WORD FrontPoint;
	WORD FrontRow;
	
	DWORD dwByteWrite = 0;
	HANDLE hNewFile = INVALID_HANDLE_VALUE;

	if (src == NULL)
	{
		ret = FALSE;
		goto _ERROR;
	}

	switch(this->file->type)
	{
	case FileIsOcc_386:
		x = 386;
		y = 289;
		FrontPoint = 340;

		FrontRow = 260;
		break;
	case FileIsOcc_384:
		x = 384;
		y = 288;

		FrontPoint = 360;
		FrontRow = 270;
		break;
	default:
		ret = FALSE;
		goto _ERROR;
	}

	//创建输出文件
	hNewFile = CreateFile(this->file->DestFilePath, GENERIC_WRITE, FILE_SHARE_READ, 0, \
						  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hNewFile == INVALID_HANDLE_VALUE)
	{
		ret = FALSE;
		goto _ERROR;
	}

	for (WORD j = 0; j < y; j++)
	{
		memset(tmpBuf, '\0',128);
		num = 0;
		cnt = 0;
		for (WORD i = 0; i < x; i++)
		{	
			if (i < FrontPoint)
			{//前FrontPoint个点
				while (*p != 0x0D)
				{
					tmpBuf[cnt++]	= *(p++);
				}
				tmpBuf[cnt++] = *(p++);
				tmpBuf[cnt++] = *(p++);

				num++;

				if (10 == num)
				{//第十个点时，才开始写数据
					num = 0;
					//_itoa_s(GetTickCount(), (char *)tmpBuf, 4, 10);
					itoa(rand(), (char *)(&tmpBuf[cnt]), 10);
					tmpBuf[cnt + 2] = 0x0D;
					tmpBuf[cnt + 3] = 0x0A;
					WriteFile(hNewFile, tmpBuf, cnt + 4, &dwByteWrite, NULL);			

					cnt = 0;
					memset(tmpBuf, '\0',128);
				}	
			}
			else
			{//剩下的(x - FrontPoint)个点
				cnt = 0;
				
				while (*p != 0x0D)
				{
					tmpBuf[cnt++]	= *(p++);
				}

				tmpBuf[cnt++] = *(p++);
				tmpBuf[cnt++] = *(p++);

				WriteFile(hNewFile, tmpBuf, cnt, &dwByteWrite, NULL);			
			}
		}		
	
		//前FrontRow行，每隔10行插入一行数据
		if ((j < FrontRow) && (9 == line))
		{
			for (DWORD i = 0; i < 420; i++)
			{
				this->InsertOneData(hNewFile);
			}

			line = 0;
		}
		else//剩下的(y - FrontRow)行，直接写进去
			line++;
	}

	ret = TRUE;
_ERROR:
	if ((hNewFile != INVALID_HANDLE_VALUE) && (hNewFile != NULL))
	{
		CloseHandle(hNewFile);
	}

	return ret;
}

BOOL ConvertRatio::Generate(void)
{	
	BOOL reslut = false;
	BYTE *ReadBuffer = NULL;
	DWORD hsize = 0;
	DWORD tableSize = 0;
	DWORD dwBytesRead = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	
	//打开需要转换的文件
	hFile =  CreateFile(this->file->NUCFilePath, GENERIC_READ, FILE_SHARE_READ, 0,\
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		reslut = FALSE;
		goto _ERROR;
	}

	FileSize = GetFileSize(hFile, &hsize);
	
	ReadBuffer = (BYTE *)malloc(FileSize + 10);
	memset(ReadBuffer, '\0', FileSize + 10);

	if( FALSE == ReadFile(hFile, (LPVOID)ReadBuffer, FileSize, &dwBytesRead, NULL))
    {
		MessageBox(_T("读取文件错误 %wd"));
		reslut = FALSE;
		goto _ERROR;
    }

	if (ReadBuffer[dwBytesRead - 2] != 0x0D)
	{
		ReadBuffer[dwBytesRead] = 0x0D;
		ReadBuffer[dwBytesRead+1] = 0x0A;
	}

	reslut = this->ConvertOCCTable(ReadBuffer);

_ERROR:
	if (ReadBuffer != NULL)
	{
		free(ReadBuffer);
		ReadBuffer = NULL;
	}
	if ((hFile != INVALID_HANDLE_VALUE) && (hFile != NULL))
	{
		CloseHandle(hFile);
	}	
	
	return reslut;
}


