#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define	N_THREADS	10

pthread_t minhas_threads[N_THREADS];		// Identificadores para 10 threads
/* observacao: quando as threads vao executar o msmo codigo, eh conveniente que tratemos elas em um array. se elas executassem tarefas diferentes, o ideal seria criar threads diff
 aqui as 10 threads executarao exatamente o codigo, com um parametro diferente, logo o comportamente de cada uma sera um pouco diferente. */

// Threads fazem coisas completamente diferentes: crie uma funcao para cada thread
// Um monte de thread que fazem a mesma coisa (ou quase a mesma coisa): uma unica funcao para threads em array, diferenciando pelo parametro de criacao das threads.

// neste caso o que diferenciara o funcionamento de cada thread sera o tempo que cada uma passara nesta funcao
void codigo_tarefa(int *nst){
	for( int ns=0; ns < *nst; ++ns) {
		sleep(1);
		printf("Tarefa: passaram %d segundos do total de %d.\n", ns+1, *nst);
	}
}

/** Função principal, cria as threads */
int main(void){
	int param_threads[N_THREADS];

	printf("Inicio\n");

	// Prepara os parametros de todas as threads
	for( int i=0; i<N_THREADS; ++i)
		param_threads[i] = 5+i;

	// Cria todas as threads
	for( int i=0; i<N_THREADS; ++i)
		pthread_create(&minhas_threads[i], NULL, (void *) codigo_tarefa, (void *)&param_threads[i]);

	// Espera por todas as threads
	for( int i=0; i<N_THREADS; ++i)
		pthread_join(minhas_threads[i], NULL);

    // primeiro espera a primeira thread terminar (trava no pthread_join(minhas_threads[1], NULL)), depois espera a segunda thread terminar...
	printf("Fim\n");
	return(0);
}



