#include "StdAfx.h"
#include "ConvertRatio.h"


ConvertRatio::ConvertRatio(void)
{
}

ConvertRatio::~ConvertRatio(void)
{
}

ConvertRatio::ConvertRatio(struct strFile *src)
{
	this->file = src;
}

BYTE ConvertRatio::ConvertByte(BYTE data)
{	
	if (data > 57)
	{
		while (data > 57)
		{
			data--;
		}
	}
	else if (data < 48)
	{
		while (data < 48)
		{
			data++;
		}
	}
	
	return data;
}

BOOL ConvertRatio::InsertOneData(HANDLE hFile)
{
	BYTE 	tmpBuf[3];
	DWORD	dwByteWrite;

	tmpBuf[0] = ConvertByte((BYTE)((GetTickCount() & 0xff00) + (GetTickCount() & 0xff)));
	tmpBuf[1] = 0x0D;
	tmpBuf[2] = 0x0A;
	return WriteFile(hFile, tmpBuf, 3, &dwByteWrite, NULL);		
}

BOOL ConvertRatio::ConvertOCCTable()
{
	BYTE *p = NULL;//src;
	BYTE cnt = 0;	
	BYTE tmpBuf[128];
	BYTE line = 0;
	BYTE num = 0;
	DWORD dwByteWrite = 0;

	if (p == NULL)
		return false;

	//��������ļ�
	HANDLE hNewFile = CreateFile(this->file->DestFilePath, GENERIC_WRITE, FILE_SHARE_READ, 0, \
								CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hNewFile == INVALID_HANDLE_VALUE)
		return false;

	for (WORD j = 0; j < 288; j++)
	{
		memset(tmpBuf, '\0',128);
		num = 0;
		cnt = 0;
		for (WORD i = 0; i < 384; i++)
		{	
			if (i < 360)
			{//ǰ360����
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
					tmpBuf[cnt++] = ConvertByte((BYTE)((GetTickCount() & 0xff00) + (GetTickCount() & 0xff)));
					tmpBuf[cnt++] = 0x0D;
					tmpBuf[cnt++] = 0x0A;
					WriteFile(hNewFile, tmpBuf, cnt, &dwByteWrite, NULL);			
				
					cnt = 0;
					memset(tmpBuf, '\0',128);
				}	
			}
			else
			{//ʣ�µ�24����
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

		//����386*289�ֱ��ʵ����ݣ�����ʣ�µ���������
		//if (FileIsOcc_386 == ftype)
		{
			for (BYTE i = 0; i < 2; i++)
			{
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

		//ǰ270�У�ÿ��10�в���һ������
		if ((j < 270) && (9 == line))
		{
			for (DWORD i = 0; i < 420; i++)
			{
				this->InsertOneData(hNewFile);
			}
				
			//if (FileIsOcc_386 == ftype)
			{//����386*289�ֱ��ʵ����ݣ�ÿ�к�����Ҫ�ټ�2������
				this->InsertOneData(hNewFile);
				this->InsertOneData(hNewFile);
			}
			line = 0;
		}
		else
			line++;
	}

	//if (FileIsOcc_386 == ftype)
	{//����386*289�ֱ��ʵ��ļ�����Ҫ�ټ���һ������
		for (DWORD i = 0; i < 422; i++)
			this->InsertOneData(hNewFile);
	}
	
	CloseHandle(hNewFile);
	return false;
}

BOOL ConvertRatio::Generate(void)
{	
	BOOL reslut = false;
	DWORD hsize = 0;
	DWORD tableSize = 0;
	DWORD dwBytesRead = 0;
	
	//����Ҫת�����ļ�
	HANDLE hFile =  CreateFile(this->file->OCCFilePath, GENERIC_READ, FILE_SHARE_READ, 0,\
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	FileSize = GetFileSize(hFile, &hsize);
	
	BYTE *ReadBuffer = (BYTE *)malloc(FileSize + 10);
	memset(ReadBuffer, '\0', FileSize + 10);

	if( FALSE == ReadFile(hFile, (LPVOID)ReadBuffer, FileSize, &dwBytesRead, NULL))
    {
		MessageBox(_T("��ȡ�ļ����� %wd"));
        CloseHandle(hFile);
        return false;
    }

	
	reslut = this->ConvertOCCTable();

	free(ReadBuffer);
	ReadBuffer = NULL;
	CloseHandle(hFile);
	return reslut;
}


