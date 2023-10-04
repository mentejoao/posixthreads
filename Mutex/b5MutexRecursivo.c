#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


pthread_t t1;	// Identificador da thread t1
pthread_t t2;	// Identificador da thread t2 

double saldo = 10000.0;		// Saldo inicial de 10 mil reais

pthread_mutex_t mutex_saldo = PTHREAD_MUTEX_INITIALIZER;


/** Calcula juro de x porcento sobre o saldo */
double calcula_juro( double x) {
	double juros;
	pthread_mutex_lock( &mutex_saldo);
	juros = saldo * (x / 100.0);
	pthread_mutex_unlock( &mutex_saldo);
	return juros;
}


/** Deposita juros de y porcento na conta */
void deposita_juros( double y) {
	double juros;
	pthread_mutex_lock( &mutex_saldo);
	juros = calcula_juro(y);
	saldo = saldo + juros;
	pthread_mutex_unlock( &mutex_saldo);
}



/** Faz 10 depositos de 20 reais */
void codigo_tarefa_1(void) {
	for( int i=0; i < 10; ++i) {
		printf("Tarefa 1 vai depositar 20 reais, i=%d\n",i);
		pthread_mutex_lock( &mutex_saldo);
		saldo = saldo + 20;
		pthread_mutex_unlock( &mutex_saldo);

	}
}


/** Faz 10 correcoes na conta somando juros de 5% */
void codigo_tarefa_2(void) {
	for( int i=0; i < 10; ++i) {
		printf("Tarefa 2 vai depositar juros de 5%%\n");
		deposita_juros(5);
	}
}



/** Função principal, cria as threads */
int main(void){
	printf("Inicio\n");
	printf("Saldo inicial %0.2lf\n", saldo);

	/* Transforma mutex_saldo em um mutex recursivo - Para tirar o erro descomente as linhas abaixo (transformaca do mutex em mutex recursivo) */
	// pthread_mutexattr_t mat;		// Cria uma variável do tipo atributos de mutex
	// pthread_mutexattr_init(&mat);	// Inicializa com valores de atributos default
	// pthread_mutexattr_settype(&mat, PTHREAD_MUTEX_RECURSIVE);	// Muda para atributo RECURSIVE
	// pthread_mutex_init(&mutex_saldo, &mat);		// Muda mutex_saldo para o tipo recursivo
    
	pthread_create(&t1, NULL, (void *) codigo_tarefa_1, NULL);
	pthread_create(&t2, NULL, (void *) codigo_tarefa_2, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("Saldo final ficou %0.2lf\n", saldo);

	printf("Fim\n");
	return(0);
}

// No Linux o mutex default nao eh recursivo e o lock em 2 mutex trava a aplicacao



