/*
	Developed by Michael Haephrati
	https://www.securedglobe.net
	https://www.freelancer.com/u/NYCyber

	January 2021

	January 2021

*/
#pragma once
#define LOG_COLOR_DARKBLUE 9
#define LOG_COLOR_DARKGREEN 2
#define LOG_COLOR_WHITE 7
#define LOG_COLOR_GREEN 10
#define LOG_COLOR_YELLOW 14 
#define LOG_COLOR_MClientA 13
#define LOG_COLOR_CIAN 11

struct WriteThis
{
	const char *readptr;
	size_t sizeleft;
};

extern wchar_t LOGFILENAME[1024];
extern HANDLE hConsole;
void setcolor(int textcol, int backcol);

string ObtainValue(string Buffer, string Value);
std::string tail(std::string const& source, size_t const length);
std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
int WriteLogFile(LPCWSTR lpText, ...);
