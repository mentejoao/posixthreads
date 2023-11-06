#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>


pthread_t th_deposita[4];	// Identificadores das threads que depositam
pthread_t th_retira[4];		// Identificadores das threads que retiram

pthread_mutex_t mutex_saldo = PTHREAD_MUTEX_INITIALIZER;
double saldo = 0.0;			// Saldo inicial é zero

// Variavel do tipo condicao (pthread_cond_t)
pthread_cond_t tem_saldo = PTHREAD_COND_INITIALIZER;	// Espera pelo saldo necessário

/** Faz 10 depositos de 20 reais a cada 1 segundo */
void codigo_tarefa_deposita(void){
	for( int ns=0; ns < 10; ++ns) {
		sleep(1);
		printf("Vai depositar 20.00 reais\n");
		pthread_mutex_lock(&mutex_saldo);	
		saldo += 20;

		pthread_cond_signal( &tem_saldo);		// Sinaliza que tem algum saldo

		pthread_mutex_unlock(&mutex_saldo);	
		printf("Depositou 20.00 reais\n");
	}
}


/** Faz 10 retiradas de 20 reais a cada 1 segundo */
void codigo_tarefa_retira(void){
	for( int ns=0; ns < 10; ++ns) {
		sleep(1);
		printf("Vai tentar retirar 20.00 reais\n");
		pthread_mutex_lock(&mutex_saldo);

		// Testa se tem o saldo necessário		
		while( saldo < 20.0 ) 
			pthread_cond_wait( &tem_saldo, &mutex_saldo);	// Espera por saldo
        // A thread aqui fica parada, enquanto o mutex_saldo eh liberado e passado para outra thread, no caso aqui para codigo_tarefa_deposita,

		saldo -= 20.0;
		pthread_mutex_unlock(&mutex_saldo);	
		printf("Retirou 20.00 reais\n");
	}
}


/** Função principal, cria as threads */
int main(void){
	printf("Inicio\n");
	printf("Saldo inicial %0.2lf\n", saldo);

	// Cria todas as threads que retiram
	for( int i=0; i<4; ++i)
		pthread_create(&th_retira[i], NULL, (void *) codigo_tarefa_retira, NULL);

	// Cria todas as threads que depositam
	for( int i=0; i<4; ++i)
		pthread_create(&th_deposita[i], NULL, (void *) codigo_tarefa_deposita, NULL);

	// Espera por todas as threads que retiram
	for( int i=0; i<4; ++i)
		pthread_join(th_retira[i], NULL);

	// Espera por todas as threads que depositam
	for( int i=0; i<4; ++i)
		pthread_join(th_deposita[i], NULL);

	printf("Saldo final ficou %0.2lf\n", saldo);

	printf("Fim\n");
	return(0);
}



