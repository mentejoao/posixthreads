#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_t t1;	// Identificador da thread t1
pthread_t t2;	// Identificador da thread t2 

int ERRO_TH = 111;		// Codigo que indica erro na thread
int SUCESSO_TH = 222; 	// Codigo que indica sucesso na thread

/* OBS: Por que nao usar define neste caso?
Problema: precisamos retornar um ponteiro para um valor, se criassemos um #define para erro e sucesso, nao poderiamos retornar um ponteiro para define.
Entao isso nos obrigou a criar uma variavel global inteira para sucesso e erro.
*/

// Duas formas de retornar o codigo de Termino

// 1. Forma rustica
void *codigo_tarefa_1(void){
	for( int ns=0; ns < 10; ++ns) {
		sleep(1);
		printf("Tarefa 1: passaram %d segundos\n", ns+1);
	}
	return( (void *) &SUCESSO_TH);
    // como o tipo eh (void *), devemos fazer este cast no final
    // obs: da mesma forma que passar parametro para thread na criacao eh com ponteiro, a forma correta de passar codigo de terminacao da thread para a thread main eh com ponteiro.
}

// 2. Utilizando recursos da propria pthreads (mais elegante)
void codigo_tarefa_2(void){
	for( int ns=0; ns < 15; ++ns) {
		sleep(1);
		printf("Tarefa 2: passaram %d segundos\n", ns+1);
	}
	pthread_exit(&ERRO_TH);
    // pthread_exit significa: termine esta thread e retorne este codigo
    // obs: da mesma forma que passar parametro para thread na criacao eh com ponteiro, a forma correta de passar codigo de terminacao da thread para a thread main eh com ponteiro.
}


/** Função principal, cria as threads */
int main(void){
	int *retorno_t1;	// Codigo de termino da thread 1
	int *retorno_t2;	// Codigo de termino da thread 2
	
	printf("Inicio\n");

	pthread_create(&t1, NULL, (void *) codigo_tarefa_1, NULL);
	pthread_create(&t2, NULL, (void *) codigo_tarefa_2, NULL);

    // na thread main, queremos obter o retorno da thread 1 e da thread2.
	pthread_join(t1, (void **) &retorno_t1);
    // quando a thread1 acabar, o endereco de memoria do codigo de retorno dela sera apontado em retorno_t1
	pthread_join(t2, (void **) &retorno_t2);
    // quando a thread2 acabar, o endereco de memoria do codigo de retorno dela sera apontado em retorno_t2

    // Importante: Por que colocamos o int de retorno global e nao local de cada funcao?
    /* Quando a funcao executada pela thread termina, todas as variaveis locais da funcao especifica sao destruidas e retornariamos, para main, o endereco
    de uma variavel que nao existe mais, o que eh problematico.*/

	printf("Fim: t1 retornou %d, t2 retornou %d\n", *retorno_t1, *retorno_t2);
	return(0);
}



