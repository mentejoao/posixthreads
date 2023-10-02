#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


/* Estrutura para passar parametros para uma thread */
struct param_t {
	int nst;		// Numero de segundos no total
	char *nome;		// Nome da thread
};
	

pthread_t t1;	// Identificador da thread t1
pthread_t t2;	// Identificador da thread t2 


/** Espera passar pparam->nst segundos, de segundo em segundo */
void codigo_tarefa_1(struct param_t *pparam){
	for( int ns=0; ns < pparam->nst; ++ns) {
		sleep(1);
		printf("%s: passaram %d segundos\n", pparam->nome, ns+1);
	}
}


/** Espera passar *nst segundos, de segundo em segundo */
void codigo_tarefa_2(int *nst){
	for( int ns=0; ns < *nst; ++ns) {
		sleep(1);
		printf("Tarefa 2: passaram %d segundos\n", ns+1);
        }
}


/** Função principal, cria as threads */
int main(void){
	struct param_t param1;
	int param2;
	
	// Parametros para a thread 1
	param1.nst = 5;
	param1.nome = "TAREFA 1"; 	
	
	// Parametros para a thread 2
	param2 = 15;

	printf("Inicio\n");

    // observacao: fornecemos o endereco do parametro
    // por padrao da pthreads, devemos passar o parametro fazendo o cast para (void *)
	pthread_create(&t1, NULL, (void *) codigo_tarefa_1, (void *) &param1);
    // ^^ como queremos passar varios parametros para as threads, um possivel recurso seria passar uma struct
	pthread_create(&t2, NULL, (void *) codigo_tarefa_2, (void *) &param2);
    // ^^ passando apenas um inteiro


    /*  a boniteza de passar parametros para threads eh que ao inves de criar varias funcoes para determinadas tarefas, podemos criar apenas uma funcao
    para determinada tarefa que sera executada, em paralelo por varias threads, com parametros diferentes.
     observacao: se alterarmos o valor de param1 e param2 antes do termino das threads, isto e, entre os creates acima e o pthread_join abaixo
     (supondo que a execucao demore tempo suficiente para alcanca-los) perderemos o controle sobre o que esta acontecendo, o que eh perigoso. 
     se quisermos usar variaveis compartilhadas precisamos utilizar outros recursos para controlar estes usos para evitar condicoes de corrida.
    */
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("Fim\n");
	return(0);
}


