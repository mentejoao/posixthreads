#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// Resolvendo a condicao de corrida de b1ConditionRace
// pthread_mutex_lock
// pthread_mutex_unlock

// Precisamos que as threads manipulem, em suas respectivas funcoes, a variavel global saldo de forma isolada
/*
Conceito de Critical Section: Chamamos de sessao critica de código um trecho de código onde a thread manipula uma variavel global compartilhada.
E quando temos duas ou mais threads com sessão critica, isto eh, manipulando a mesma variavel global compartilhada precisamos que exista uma exclusao mutua entre elas, ou seja,
enquanto a thread1 esta mexendo em saldo, a thread2 nao pode mexer e vice-versa.
*/

pthread_t t1;	// Identificador da thread t1
pthread_t t2;	// Identificador da thread t2 

pthread_mutex_t mutex_saldo = PTHREAD_MUTEX_INITIALIZER;
// inicializacao padrao de um mutex ^^
// o mutex eh como uma fechadura, a inicializacao default eh como se a fechadura comecasse aberta, por isso chamamos o lock primeiro (porque ele ja comeca aberto)
// obs: para cada variavel global compartilhada, temos um mutex referente a ela

double saldo = 10000.0;		// Saldo inicial de 10 mil reais

// Caso todas as threads apenas leiam a variavel compartilhada, precisa de mutex? Nao tem necessidade.
// Se tenho varias threads, todas leem e apenas uma altera, precisa de mutex?
// Sim, pq caso a variavel compartilhada seja uma string, um double, podem resultar em varias linhas em assembly, o que pode ser tempo suficiente para que gere uma condicao de corrida

/** Faz 100 retiradas de 10 reais */
void codigo_tarefa_1(void){
	double saldo_velho;
	double saldo_novo;
	sleep(1);	// Faz alguma inicializacao
	for( int ns=0; ns < 100; ++ns) {
		pthread_mutex_lock(&mutex_saldo);	// inicio da sessao critica
		saldo_velho = saldo;
		saldo_novo = saldo_velho - 10;
		printf("Saldo passou de %0.2lf para o valor de %0.2lf\n", saldo_velho, saldo_novo);
		saldo = saldo_novo;
		pthread_mutex_unlock(&mutex_saldo);	// fim da sessao critica
	}
}

/** Faz 100 depositos de 10 reais */
void codigo_tarefa_2(void){
	double saldo_velho;
	double saldo_novo;
	sleep(1);	// Faz alguma inicializacao
	for( int ns=0; ns < 100; ++ns) {
		pthread_mutex_lock(&mutex_saldo);	
		saldo_velho = saldo;
		saldo_novo = saldo_velho + 10;
		printf("Saldo passou de %0.2lf para o valor de %0.2lf\n", saldo_velho, saldo_novo);
		saldo = saldo_novo;
		pthread_mutex_unlock(&mutex_saldo);	
	}
}


/** Função principal, cria as threads */
int main(void){
	printf("Inicio\n");
	printf("Saldo inicial %0.2lf\n", saldo);

	//codigo_tarefa_1();
	//codigo_tarefa_2();

	pthread_create(&t1, NULL, (void *) codigo_tarefa_1, NULL);
	pthread_create(&t2, NULL, (void *) codigo_tarefa_2, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("Saldo final ficou %0.2lf\n", saldo);

	printf("Fim\n");
	return(0);
}



