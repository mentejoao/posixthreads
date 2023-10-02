#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_t t1;	// Identificador da thread t1
pthread_t t2;	// Identificador da thread t2 

/** Espera passar 10 segundos, de segundo em segundo */
void codigo_tarefa_1(void){
	for( int ns=0; ns < 10; ++ns) {
		sleep(1);
		printf("Tarefa 1: passaram %d segundos\n", ns+1);
	}
}


void codigo_tarefa_2(void){
/** Espera passar 15 segundos, de segundo em segundo */
	for( int ns=0; ns < 15; ++ns) {
		sleep(1);
		printf("Tarefa 2: passaram %d segundos\n", ns+1);
	}
}


/** Função principal, cria as threads */
int main(void){
	printf("Inicio\n");

	// primeiro parametro: endereco identificador da thread
	// segundo parametro: atributos/configuracoes das threads, NULL = configs default
	// terceiro parametro: nome da funcao a ser executada pela thread
	// quarto parametro: estes parametros sao passados no momento do criaçao da thread, sao os parametros da funcao que sera executado por ela
	pthread_create(&t1, NULL, (void *) codigo_tarefa_1, NULL);
	// uma thread t1 eh criada e executa a funcao codigo_tarefa_1, a thread main continua ate a linha de baixo, paralela a execucao da t1
	pthread_create(&t2, NULL, (void *) codigo_tarefa_2, NULL);
	// ... a partir da linha acima, temos 3 threads rodando, a principal e as outras duas criadas

	// primeiro parametro: thread que se espera o término
	// segundo parametro: recebe um codigo de terminacao da thread (retorno)
	pthread_join(t1, NULL);
	// bloqueia a thread main até que t1 termine
	pthread_join(t2, NULL);
	// bloqueia a thread main até que t2 termine

	// Qual a importância do join?
	/* Se a thread main chegar até o final, não só ela termina, mas ela encerra todas as threads criadas por ela, independente se elas ja terminaram ou não. Então, o join
	força que a thread main espere as filhas executarem o que possuem como tarefa para prosseguir. 
	Depois comentar os pthread_join's para avaliarmos o que acontece (provavelmente a main encerrara antes mesmo das threads comecarem).
	*/

	// padrao classico: a thread main inicializa, cria as threads filhas que executam verdadeiramente o que deve ser executado, enquanto a thread main espera as filhas terminarem.

	printf("Fim\n");
	return(0);
}



