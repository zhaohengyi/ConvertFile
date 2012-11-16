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
	struct strFile *file;
	BYTE *ImageBuf;

	static const DWORD IMAGE_SIZE = 16*1024*1024;

	static const DWORD McsAddr = 0x0;
	static const DWORD OCCAddr = 0x400000;
	static const DWORD TwoPointAddr = 0x900000;
	static const DWORD VerticalAddr = 0xE80000;
	static const DWORD BlindAddr = 0xF00000;
	static const DWORD MemsAddr = 0xF80000;

	static const DWORD McsSectionSize = 4*1024*1024;
	static const DWORD BlindSectionSize = 315*1024;
	static const DWORD MemsSectionSize = 1 * 1024;
private:
	BYTE *ReadBinFile(CString src, DWORD *ReadSize);
private:
	BOOL ReadMcsFile(void);
	BOOL ReadNUCFile(void);
	BOOL ReadTwoPointFile(void);
	BOOL ReadVerticalFile(void);
	BOOL ReadBlindFile(void);
	BOOL ReadMemsFile(void);
};

