// THEMFile.cpp: implementation of the CTHEMFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "THEMFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTHEMFile::CTHEMFile()
{
	FillMemory(&desc, sizeof(desc), 0);
}

CTHEMFile::~CTHEMFile()
{

}

bool CTHEMFile::CheckPath(char *path)
{
	bool handled = true;
	HANDLE hf = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL,
						OPEN_EXISTING, NULL, NULL);

	if(hf != INVALID_HANDLE_VALUE)
	{
		DWORD nBytes;
		ReadFile(hf, &desc, sizeof(desc), &nBytes, NULL);

		if(nBytes != sizeof(desc))
		{
			FillMemory(&desc, sizeof(desc), 0);
			handled = false;
		}

		CloseHandle(hf);
	}
	return handled;
}

bool CTHEMFile::SwapChannels(char *path, char *newpath, int ch1, int ch2, int method)
{
	bool handled = true;
	HANDLE hIn, hOut;
	DWORD mode = GENERIC_READ;
	DWORD nBytes;
	char *buff;
	char *tmp;
	int i, fSize = 0, offset1 = 0, offset2 = 0;
	
	mode |= method != MTHD_CREATENEW ? GENERIC_WRITE : 0;

	hIn = CreateFile(path, mode, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if(hIn != INVALID_HANDLE_VALUE)
	{
		ReadFile(hIn, &desc, sizeof(desc), &nBytes, NULL);
		if(nBytes != sizeof(desc))
		{
			CloseHandle(hIn);
			::MessageBox(NULL, path, "Failed to open input file", MB_OK);
			return false;
		}

		for(i = 0; i < desc.nTypes; i++)
		{
			if(desc.element[i].type == ch1 && desc.element[i].size != 0)
			{
				fSize = desc.element[i].size * desc.element[i].nPoints;
				offset1 = desc.element[i].offset;
			}
			else if(desc.element[i].type == ch2)
			{
				offset2 = desc.element[i].offset;
			}
		}


		buff = new char[desc.size];
		tmp = new char[fSize];

//		skip = desc.size - (__max(offset1, offset2) + fSize);

		switch(method)
		{
			case MTHD_HEADERONLY:
				for(i = 0; i < desc.nTypes; i++)
				{
					if(desc.element[i].type == ch1 && desc.element[i].size != 0)
						desc.element[i].type = ch2;
					else if(desc.element[i].type == ch2)
						desc.element[i].type = ch1;
				}
				SetFilePointer(hIn, 0, 0, FILE_BEGIN);
				WriteFile(hIn, &desc, sizeof(desc), &nBytes, NULL);
				break;

			case MTHD_INPLACE:
				i = 0;
				while(handled)
				{
					handled &= (SetFilePointer(hIn, (desc.size * i) + sizeof(desc) + offset1, 0, FILE_BEGIN) != 0xffffffff);
					handled &= (ReadFile(hIn, buff, fSize, &nBytes, NULL) != 0);
					if(!nBytes || !handled)
						break;
					handled &= (SetFilePointer(hIn, (desc.size * i) + sizeof(desc) + offset2, 0, FILE_BEGIN) != 0xffffffff);
					handled &= (ReadFile(hIn, tmp, fSize, &nBytes, NULL) != 0);
					if(!nBytes || !handled)
						break;
					handled &= (SetFilePointer(hIn, (desc.size * i) + sizeof(desc) + offset2, 0, FILE_BEGIN) != 0xffffffff);
					handled &= (WriteFile(hIn, buff, fSize, &nBytes, NULL) != 0);
					handled &= (SetFilePointer(hIn, (desc.size * i) + sizeof(desc) + offset1, 0, FILE_BEGIN) != 0xffffffff);
					handled &= (WriteFile(hIn, tmp, fSize, &nBytes, NULL) != 0);
					i++;
				}
				break;

			case MTHD_CREATENEW:
				hOut = CreateFile(newpath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
				if(hOut != INVALID_HANDLE_VALUE)
				{
					if(buff)
					{
						WriteFile(hOut, &desc, sizeof(desc), &nBytes, NULL);

						while(ReadFile(hIn, buff, desc.size, &nBytes, NULL))
						{
							if(!nBytes || !handled)
								break;
							memcpy(tmp, buff+offset1, fSize);
							memcpy(buff+offset1, buff+offset2, fSize);
							memcpy(buff+offset2, tmp, fSize);
							WriteFile(hOut, buff, desc.size, &nBytes, NULL);
						}
					}
					CloseHandle(hOut);
				}
				else
					::MessageBox(NULL, newpath, "Failed to open output file", MB_OK);
				break;
		}
		CloseHandle(hIn);
		if(tmp)		delete [] tmp;
		if(buff)	delete [] buff;
	}
	return handled;
}