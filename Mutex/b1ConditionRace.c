#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

pthread_t t1;	// Identificador da thread t1
pthread_t t2;	// Identificador da thread t2 

double saldo = 10000.0;		// Saldo inicial de 10 mil reais
// obs: uma variavel global ela eh enxergada para todas as threads.
// se uma thread esta executando determinada funcao e aquela funcao possui suas variaveis locais, estas variaveis locais serao isoladas de cada thread (privativas)
/*
por exemplo, se tivessemos 10 threads executando a mesma funcao, cada thread tem sua pilha e essas pilhas armazenariam as variaveis locais de cada thread separadamente em cada pilha,
neste caso abaixo, teriamos no total 10 instancias de saldo_velho e saldo_novo, uma para cada thread, de forma privativa e independente.
*/ 

/** Faz 100 retiradas de 10 reais */
void codigo_tarefa_1(void){
	double saldo_velho;
	double saldo_novo;
	sleep(1);	// Faz alguma inicializacao
	for( int ns=0; ns < 100; ++ns) {
		saldo_velho = saldo;
		saldo_novo = saldo_velho - 10;
		printf("Saldo passou de %0.2lf para o valor de %0.2lf\n", saldo_velho, saldo_novo);
		saldo = saldo_novo;
	}
}


/** Faz 100 depositos de 10 reais */
void codigo_tarefa_2(void){
	double saldo_velho;
	double saldo_novo;
	sleep(1);	// Faz alguma inicializacao
	for( int ns=0; ns < 100; ++ns) {
		saldo_velho = saldo;
		saldo_novo = saldo_velho + 10;
		printf("Saldo passou de %0.2lf para o valor de %0.2lf\n", saldo_velho, saldo_novo);
		saldo = saldo_novo;
	}
}


/** Função principal, cria as threads */
int main(void) {
	printf("Inicio\n");
	printf("Saldo inicial %0.2lf\n", saldo);

	//codigo_tarefa_1();
	//codigo_tarefa_2();

	pthread_create(&t1, NULL, (void *) codigo_tarefa_1, NULL);
	pthread_create(&t2, NULL, (void *) codigo_tarefa_2, NULL);

    /*  Se chamarmos primeiro codigo_tarefa_1 e depois codigo_tarefa_2, esperamos que após a thread1 executar codigo_tarefa_1, nossa variavel global saldo fosse
    9000 e depois da execução da thread2 em codigo_tarefa_2, a variavel global saldo retornasse ao valor de 10000. Entretanto, na pratica, vemos que isso nao acontece. Por que?
    Observacao1: execucoes diferentes resultam em resultados diferentes. 
    Observacao2: se rodarmos o codigo sem threads, isto eh, chamando sequencialmente codigo_tarefa_1() e codigo_tarefa_2() obtemos o resultado que realmente estavamos esperando.
    Ou seja, o problema nao eh na implementacao das funcoes e sim por algo sobre a execucao com threads.
    Problema: Nao temos controle sobre como o SO escalona as threads, podemos ter as duas threads compartilhando o mesmo core do processador, uma thread em um core e outra em outro,
    intercalando suas execucoes, compartilhando fatias de tempos iguais ou diferentes.. varias situacoes.
    Imagine a seguinte situacao: em determinado momento que a variavel global seja 10000, na thread1, que executa codigo_tarefa_1, a variavel privativa de codigo_tarefa_1 saldo_velho
    receberia 10000, calcula saldo novo como 9990, da um print na tela e entre o print e o saldo = saldo_novo, o SO decida executar a thread2 com codigo_tarefa_2, como o valor de saldo
    ainda era 10000, saldo_velho de codigo_tarefa_2 recebe 10000.. este eh o problema. Depende da corrida das threads. O valor do resultado esta dependendo da corrida das threads.
    Conclusao: nao podemos deixar duas threads mexerem livremente em uma variavel global compartilhada, pois gera condicoes de corrida e faz com o que resultado nao seja o esperado.
    Precisamos de mecanismos de sincronziacao de threads para que compartilhem da mesma variavel.
    */

   /*
   Isso seria "equivalente" a fazer sem threads, sequencialmente:
    pthread_create(&t1, NULL, (void *) codigo_tarefa_1, NULL);
    pthread_join(t1, NULL);
	pthread_create(&t2, NULL, (void *) codigo_tarefa_2, NULL);
    pthread_join(t2, NULL);
    Obteriamos sempre 10000 como resposta, pois nao estao sendo executadas em paralelo.
   */

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("Saldo final ficou %0.2lf\n", saldo);

	printf("Fim\n");
	return(0);
}



