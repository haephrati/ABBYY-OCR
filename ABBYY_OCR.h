#pragma once
/*

Application ID:	bc4d6eb0-e5af-45c4-9e91-0efc8a19a03b
Available:	500 pages / 2500 fields ADD PAGES
Expiration:	28 Apr 2021
Location:	United States of America
Processing Url:	https://cloud-westus.ocrsdk.com

*/
string ObtainURL(string Buffer);
string ObtainStatus(string Buffer);
bool SG_OCR(wstring ImageFile, wstring ResultFile);
