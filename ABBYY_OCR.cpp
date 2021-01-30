#include "stdafx.h"
#include "utils.h"
#include <curl\curl.h>
#include "ABBYY_OCR.h"

/*

	Developed by Michael Haephrati
	https://www.securedglobe.net
	https://www.freelancer.com/u/NYCyber

	January 2021

*/
// exportFormat -> https://support.abbyy.com/hc/en-us/articles/360017269960-Export-formats
// language -> https://support.abbyy.com/hc/en-us/articles/360017326859-Recognition-languages


// Here are the credentials of https://github.com/abbyy/ocrsdk.com :
#define APP_ID			"<Your Application ID>"
#define PASSWORD		"<Your Application Password"

// These settings are for the US server
#define PROCESSING_URL	"https://cloud-westus.ocrsdk.com/processImage?exportFormat=txt&language=English,Hebrew"
#define STATUS_URL		"https://cloud-westus.ocrsdk.com/getTaskStatus?taskId="

size_t CurlWrite_CallbackFunc_StdString(void* contents, size_t size, size_t nmemb, std::string* s)
{
	size_t newLength = size * nmemb;
	try
	{
		s->append((char*)contents, newLength);
	}
	catch (std::bad_alloc& e)
	{
		//handle memory problem
		return 0;
	}
	return newLength;
}


string ObtainURL(string Buffer)
{
	return(ObtainValue(Buffer, "resultUrl"));
}
string ObtainStatus(string Buffer)
{
	return(ObtainValue(Buffer, "status"));
}
// This function needs to be rewritten
bool SG_OCR(wstring ImageFile, wstring ResultFile)
{

	CURL* curl;
	CURL* status_curl;
	CURL* op_curl;
	CURLcode res;

	std::string readBuffer;

	string file_to_upload(ImageFile.begin(), ImageFile.end());
	string json_result_file(ResultFile.begin(), ResultFile.end());
	curl_mime* form = NULL;
	curl_mimepart* field = NULL;
	struct curl_slist* headerlist = NULL;
	static const char buf[] = "Expect:";

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	if (curl)
	{
		form = curl_mime_init(curl);
		field = curl_mime_addpart(form);
		curl_mime_name(field, "upload");
		curl_mime_filedata(field, file_to_upload.c_str());
		headerlist = curl_slist_append(headerlist, buf);
		curl_easy_setopt(curl, CURLOPT_URL, PROCESSING_URL);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
		curl_easy_setopt(curl, CURLOPT_USERNAME, APP_ID);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		long http_code;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		WriteLogFile(L"http_code: %d res = %d", http_code, res);
		if (res != CURLE_OK)
		{
			WriteLogFile(L"Error: curl_easy_perform() failed: %S", curl_easy_strerror(res));
			return false;
		}
		else
		{

			// Delete previously created json file
			DeleteFile(ResultFile.c_str());

			//getting task id
			size_t tid_ind = readBuffer.find("task id=");
			size_t tid_ind_s = readBuffer.substr(tid_ind).find("\"") + 1;
			size_t tid_len = readBuffer.substr(tid_ind + tid_ind_s).find("\"");
			string task_id = readBuffer.substr((tid_ind + tid_ind_s), (tid_len));

			WriteLogFile(L"Task ID : %S", task_id.c_str());
			//getting task status
			readBuffer = "";

			std::string status_url = STATUS_URL + task_id;
			std::string result_url;
			status_curl = curl_easy_init();
			std::string task_status;
			if (status_curl)
			{
				headerlist = curl_slist_append(headerlist, buf);
				curl_easy_setopt(status_curl, CURLOPT_URL, status_url.c_str());
				curl_easy_setopt(status_curl, CURLOPT_HTTPHEADER, headerlist);
				curl_easy_setopt(status_curl, CURLOPT_USERNAME, APP_ID);
				curl_easy_setopt(status_curl, CURLOPT_PASSWORD, PASSWORD);
				curl_easy_setopt(status_curl, CURLOPT_HTTPGET, 1L);
				curl_easy_setopt(status_curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
				curl_easy_setopt(status_curl, CURLOPT_WRITEDATA, &readBuffer);
				while (1)
				{
					res = curl_easy_perform(status_curl);
					if (res != CURLE_OK)
					{
						WriteLogFile(L"Error: %S", curl_easy_strerror(res));
					}
					else
					{
						WriteLogFile(L"Read Buffer:\n%S", readBuffer.c_str());
						task_status = ObtainStatus(readBuffer);
						WriteLogFile(L"task_status: %S", task_status.c_str());
					}
					if (task_status != "Completed")
					{
						//wait 5s before next check
						Sleep(2000);
					}
					else
					{
						setcolor(LOG_COLOR_DARKGREEN, 0);
						SetConsoleTitle(L"OCR completed");
						setcolor(LOG_COLOR_WHITE, 0);

						result_url = ObtainURL(readBuffer);
						//replace all &amp; to &
						result_url = ReplaceAll(result_url, "&amp;", "&");
						//downloading text file of response
						WriteLogFile(L"Downloading File from URL: %S", result_url.c_str());
						op_curl = curl_easy_init();
						if (op_curl)
						{
							headerlist = curl_slist_append(headerlist, buf);
							curl_easy_setopt(op_curl, CURLOPT_URL, result_url.c_str());
							curl_easy_setopt(op_curl, CURLOPT_HTTPHEADER, headerlist);
							curl_easy_setopt(op_curl, CURLOPT_HTTPGET, 1L);
							FILE* wfd = fopen(json_result_file.c_str(), "ab");
							fprintf(wfd, "\n");
							curl_easy_setopt(op_curl, CURLOPT_WRITEDATA, wfd);
							curl_easy_perform(op_curl);
							curl_easy_cleanup(op_curl);
							fclose(wfd);
							WriteLogFile(L"FILE saved");
						}
						break;
					}
					readBuffer = "";
				}	// While

			}
			curl_easy_cleanup(status_curl);
		}
		curl_easy_cleanup(curl);
		curl_mime_free(form);
		curl_slist_free_all(headerlist);
	}
	WriteLogFile(L"OCR Completed");
	return true;
}