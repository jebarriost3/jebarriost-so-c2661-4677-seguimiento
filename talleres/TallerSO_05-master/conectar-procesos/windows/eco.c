#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256
#define LINE_BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: eco <archivo> <origen> <destino> <modulo>\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "a-z") != 0 || strcmp(argv[3], "A-Z") != 0) {
        fprintf(stderr, "Esta version solo soporta la traduccion: a-z A-Z\n");
        return EXIT_FAILURE;
    }

    int modulo = atoi(argv[4]);
    if (modulo <= 0) {
        fprintf(stderr, "El modulo debe ser mayor que 0\n");
        return EXIT_FAILURE;
    }

    SECURITY_ATTRIBUTES saAttr;
    HANDLE catRead, catWrite;
    HANDLE trRead, trWrite;

    STARTUPINFO siCat, siTr;
    PROCESS_INFORMATION piCat, piTr;

    DWORD bytesRead;
    char buffer[BUFFER_SIZE];
    char lineBuffer[LINE_BUFFER_SIZE];
    int linePos = 0;
    int lineNumber = 1;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&catRead, &catWrite, &saAttr, 0)) {
        fprintf(stderr, "Error creando pipe cat->tr: %ld\n", GetLastError());
        return EXIT_FAILURE;
    }

    if (!CreatePipe(&trRead, &trWrite, &saAttr, 0)) {
        fprintf(stderr, "Error creando pipe tr->eco: %ld\n", GetLastError());
        CloseHandle(catRead);
        CloseHandle(catWrite);
        return EXIT_FAILURE;
    }

    SetHandleInformation(trRead, HANDLE_FLAG_INHERIT, 0);

    ZeroMemory(&siCat, sizeof(STARTUPINFO));
    ZeroMemory(&piCat, sizeof(PROCESS_INFORMATION));
    siCat.cb = sizeof(STARTUPINFO);
    siCat.dwFlags = STARTF_USESTDHANDLES;
    siCat.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    siCat.hStdOutput = catWrite;
    siCat.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    char cmdCat[512];
    snprintf(cmdCat, sizeof(cmdCat), "cmd /c type \"%s\"", argv[1]);

    if (!CreateProcess(
            NULL,
            cmdCat,
            NULL,
            NULL,
            TRUE,
            0,
            NULL,
            NULL,
            &siCat,
            &piCat)) {
        fprintf(stderr, "Error creando proceso type: %ld\n", GetLastError());
        CloseHandle(catRead);
        CloseHandle(catWrite);
        CloseHandle(trRead);
        CloseHandle(trWrite);
        return EXIT_FAILURE;
    }

    CloseHandle(catWrite);
    SetHandleInformation(catRead, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    SetHandleInformation(trWrite, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

    ZeroMemory(&siTr, sizeof(STARTUPINFO));
    ZeroMemory(&piTr, sizeof(PROCESS_INFORMATION));
    siTr.cb = sizeof(STARTUPINFO);
    siTr.dwFlags = STARTF_USESTDHANDLES;
    siTr.hStdInput = catRead;
    siTr.hStdOutput = trWrite;
    siTr.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    if (!CreateProcess(
            NULL,
            "concatenar",
            NULL,
            NULL,
            TRUE,
            0,
            NULL,
            NULL,
            &siTr,
            &piTr)) {
        fprintf(stderr, "Error creando proceso concatenar: %ld\n", GetLastError());
        CloseHandle(catRead);
        CloseHandle(trRead);
        CloseHandle(trWrite);
        CloseHandle(piCat.hProcess);
        CloseHandle(piCat.hThread);
        return EXIT_FAILURE;
    }

    CloseHandle(catRead);
    CloseHandle(trWrite);

    while (ReadFile(trRead, buffer, BUFFER_SIZE, &bytesRead, NULL) && bytesRead > 0) {
        for (DWORD i = 0; i < bytesRead; i++) {
            char c = buffer[i];

            if (c == '\r') {
                continue;
            }

            if (c == '\n') {
                lineBuffer[linePos] = '\0';

                if (lineNumber % modulo != 0) {
                    printf("%s\n", lineBuffer);
                }

                lineNumber++;
                linePos = 0;
            } else {
                if (linePos < LINE_BUFFER_SIZE - 1) {
                    lineBuffer[linePos++] = c;
                }
            }
        }
    }

    if (linePos > 0) {
        lineBuffer[linePos] = '\0';
        if (lineNumber % modulo != 0) {
            printf("%s\n", lineBuffer);
        }
    }

    CloseHandle(trRead);

    WaitForSingleObject(piCat.hProcess, INFINITE);
    WaitForSingleObject(piTr.hProcess, INFINITE);

    CloseHandle(piCat.hProcess);
    CloseHandle(piCat.hThread);
    CloseHandle(piTr.hProcess);
    CloseHandle(piTr.hThread);

    return EXIT_SUCCESS;
}
