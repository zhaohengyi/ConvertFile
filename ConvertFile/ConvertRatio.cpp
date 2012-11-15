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

	//��������ļ�
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
			{//ǰFrontPoint����
				while (*p != 0x0D)
				{
					tmpBuf[cnt++]	= *(p++);
				}
				tmpBuf[cnt++] = *(p++);
				tmpBuf[cnt++] = *(p++);

				num++;

				if (10 == num)
				{//��ʮ����ʱ���ſ�ʼд����
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
			{//ʣ�µ�(x - FrontPoint)����
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
	
		//ǰFrontRow�У�ÿ��10�в���һ������
		if ((j < FrontRow) && (9 == line))
		{
			for (DWORD i = 0; i < 420; i++)
			{
				this->InsertOneData(hNewFile);
			}

			line = 0;
		}
		else//ʣ�µ�(y - FrontRow)�У�ֱ��д��ȥ
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
	
	//����Ҫת�����ļ�
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
		MessageBox(_T("��ȡ�ļ����� %wd"));
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


