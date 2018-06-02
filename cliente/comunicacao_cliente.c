#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include "comunicacao_cliente.h"
#include "../DlltpSO2/comunicacao.h"

#define BUFSIZE 512

int arrancaComunicacaoCliente()
{

	HANDLE hPipe;
	LPTSTR lpvMessage = TEXT("Default message from client.");
	//TCHAR  chBuf[sizeof(MsgCliGat)]; //trocar por sizeofMesgCliGAT
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");

	// Try to open a named pipe; wait for it, if necessary. 

	while (1)
	{
		hPipe = CreateFile(
			lpszPipename,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file 

							// Break if the pipe handle is valid. 

		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			_tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
			return -1;
		}

		// All pipe instances are busy, so wait for 20 seconds. 

		if (!WaitNamedPipe(lpszPipename, 20000))
		{
			printf("Could not open pipe: 20 second wait timed out.");
			return -1;
		}
	}

	// The pipe connected; change to message-read mode. 

	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess)
	{
		_tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
		return -1;
	}

	// Send a message to the pipe server. 

//	_tprintf(TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, lpvMessage);

	MsgCLI Teste;
	Teste.tipo_mensagem = INICIO;
	_tcscpy_s(Teste.nome, _countof(Teste.nome)  ,_T("ZE FINOS"));
	Teste.id = 123;
	//_tcscpy(Teste.tecla, TEXT("esquerda"));

	fSuccess = WriteFile(
		hPipe,                  // pipe handle 
		(void *)&Teste,             // message 
		sizeof(MsgCLI),              // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 

	if (!fSuccess)
	{
		_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
		return -1;
	}

	printf("\nMessage sent to server, receiving reply as follows:\n");

	do
	{
		// Read from the pipe. 

		MsgCliGat cenas;

		fSuccess = ReadFile(
			hPipe,    // pipe handle 
			(void *)&cenas,    // buffer to receive reply 
			sizeof(MsgCliGat),  // size of buffer 
			&cbRead,  // number of bytes read 
			NULL);    // not overlapped 

		if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
			break;

		_tprintf(TEXT("\"%d\"\n"), cenas.cenas3.altura);
	} while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

	if (!fSuccess)
	{
		_tprintf(TEXT("ReadFile from pipe failed. GLE=%d\n"), GetLastError());
		return -1;
	}

	printf("\n<End of message, press ENTER to terminate connection and exit>");
	_getch();
	CloseHandle(hPipe);

	return 0;
}