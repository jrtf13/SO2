#pragma once
#include <windows.h>
#include <tchar.h>
#include "comunicacao.h"

//Definir uma constante para facilitar a leitura do prot�tipo da fun��o

//Esta macro � definida pelo sistema caso estejamos na DLL (<DLL_IMP>_EXPORTS definida)
//ou na app (<DLL_IMP>_EXPORTS n�o definida) onde DLL_IMP � o nome deste projeto

//


#ifdef DlltpSO2_EXPORTS 
#define DlltpSO2 __declspec(dllexport)
#else
#define DlltpSO2 __declspec(dllimport)
#endif

#define MAX 50 //tamanho do buffer 
#define SIZE 20 //dimens�o da mensagem e do nome 
#define mPartilhadaMensagens TEXT("memPartilhadaMensagens")
#define TAMANHOBUFFER sizeof(BufferMensagens)



HANDLE SemaforoEscrever;
HANDLE SemaforoLer;
HANDLE MutexRead;
HANDLE MutexWrite;
HANDLE hMemoriaBuffer;
HANDLE hMemoriaJogo;
HANDLE hEventActiva;
HANDLE hEventLida;
HANDLE hEvento, hMutexJogo;


//Apenas uma coisa de cada tipo de obecto do jogo porque vai ser feita a actualiza��o 1 a 1



//Gateway manda para o cliente uma estrutura com uma cena de cada
//e actualiza a cada altera�aao 



typedef enum _Direcao {
	direita,
	baixo,
	cima,
	esquerda
} Direcao;

#define MOVEMENT_INCREMENT 2



#ifdef __cpluplus
extern "C" {
#endif

	DlltpSO2 PBufferMensagens mensagens;
	DlltpSO2 PJogo jogo;
	//Fun��es a serem exportadas/importadas
	DlltpSO2 int IniciaSinc();
	DlltpSO2 void AcabaSinc();
	DlltpSO2 MsgCLI TrataMensagem();
	DlltpSO2 void EnviaMensagem(MsgCLI dados); 
	DlltpSO2 void IniciaBuffer();
	DlltpSO2 MsgCliGat EnviaUpdate(int id);


#ifdef __cplusplus
}
#endif
