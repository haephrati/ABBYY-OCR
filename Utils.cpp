/*
	Developed by Michael Haephrati
	https://www.securedglobe.net
	https://www.freelancer.com/u/NYCyber

	January 2021

*/
#include "stdafx.h"
#include "ABBYY_OCR.h"
#include "utils.h"
wchar_t LOGFILENAME[1024]{ L"" };

void setcolor(int textcol, int backcol)
{
	if ((textcol % 16) == (backcol % 16))textcol++;
	textcol %= 16; backcol %= 16;
	unsigned short wAttributes = ((unsigned)backcol << 4) | (unsigned)textcol;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, wAttributes);
}

string ObtainValue(string Buffer, string Value)
{
	string URL{ "" };
	if (tail(Value, 1) != "=")
	{
		Value += "=";
	}
	//get resultURL from response
	size_t tid_ind = Buffer.find(Value);
	size_t tid_ind_s = Buffer.substr(tid_ind).find("\"") + 1;
	size_t tid_len = Buffer.substr(tid_ind + tid_ind_s).find("\"");
	URL = Buffer.substr((tid_ind + tid_ind_s), (tid_len));
	return URL;
}

std::string tail(std::string const& source, size_t const length)
{
	if (length >= source.size()) { return source; }
	return source.substr(source.size() - length);
} // tail

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}




// Function:	WriteLogFile
// Purpose:		Print a message to the log file and (in DEBUG) to the Console
int WriteLogFile(LPCWSTR lpText, ...)
{
	FILE *fp;
	CTime Today = CTime::GetCurrentTime();

	CAtlString sMsg;
	CAtlString sLine;
	va_list ptr;
	errno_t errCode;
	va_start(ptr, lpText);
	sMsg.FormatV(lpText, ptr);

	sLine.Format(L"%s: %s\n", (LPCTSTR)Today.FormatGmt(L"%d.%m.%Y %H:%M"), (LPCTSTR)sMsg);
	CStringA line = (CStringA)sMsg + "\n";
	try
	{
		errCode = _wfopen_s(&fp, LOGFILENAME, L"a,ccs=UTF-8");
		if (fp && (errCode == 0))
		{
			fwprintf(fp, L"%s\n", sLine.GetBuffer());

			// Added by BOZO
			sLine.ReleaseBuffer();

			if (fp->_Placeholder)
				fclose(fp);
		}
#ifdef _DEBUG
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#else
		wprintf(L"%S\n", line.GetBuffer());
#endif
		if (hConsole)
		{
			WriteFile(
				hConsole,               // output handle 
				line.GetBuffer(),       // prompt string 
				line.GetLength(),		// string length 
				NULL,					// bytes written 
				NULL);
		}
	}
	catch (...)
	{
		if (fp->_Placeholder)
			fclose(fp);

		return FALSE;
	}

	return TRUE;
}

