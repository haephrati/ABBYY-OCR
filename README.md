### Video Demo ###
[![Demo Video](https://github.com/haephrati/ABBYY-OCR/blob/main/Video%20Img.jpg)](https://youtu.be/UwGeNdNocRs)

# ABBYY OCR Demo
ABBYY® Cloud OCR SDK is a web-based document processing service that will enhance your enterprise software systems, SaaS platforms, or your mobile apps with the ability to convert documents and utilize textual information from scans, PDFs, document images, smartphone photos, or screenshots.

:point_right: ABBYY OCR Open Source code can be found [here](https://github.com/abbyy).

ABBYY OCR Demo is a Command Line based software component created for the purpose of demonstration of the ABBYY OCR SDK using C++ (Visual Studio C++) and LibCurl.

## Before using
To try out the OCR technology using this demo, request ABBYY Mobile Capture trial version on [ABBYY Cloud OCR SDK](https://cloud.ocrsdk.com/Account/Register).
Make sure to replace the following lines:

```
#define APP_ID			<APP ID>
#define PASSWORD		<App Password>
```
with the Application ID you have received and an application password you need to generate.

## Starting the Demo
Open a Command Prompt (CMD) and type:
```
AbbyyOCR.exe <image file> <output file>
```
Then press **ENTER**.
 
**image file**:	Should be a .jpg or .png file which contains text to be recognized.

**Output file**:	The full path for a new file to be created with the output.

Example:
![example](https://github.com/haephrati/ABBYY-OCR/blob/main/ABBYY%20Example.jpg) 
 
## Logging
The **WriteLogFile()** function is used instead of wprintf() and carries the task of displaying debug messages in the DEBUG version and writing to a log file (in both DEBUG / RELEASE versions). 

## Technology
-	The Software was developed using Visual Studio Enterprise C++ 2017.
-	[LibCurl](https://curl.se/libcurl/) is used as a static library.

