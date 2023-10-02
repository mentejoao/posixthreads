#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>


pthread_t t1;	// Identificador da thread t1
pthread_t t2;	// Identificador da thread t2 

pthread_mutex_t em = PTHREAD_MUTEX_INITIALIZER;

/* Um principio fundamental sobre os Mutex eh que o codigo dentro de uma sessao critica seja o menor possivel, de modo que ela termine o mais rapido possivel.
Porque sempre que estamos em uma sessao critica, estamos segurando um mutex, ou seja, estamos impedindo outras threads de fazerem outras coisas, se elas tambem precisam do mutex..
*/

/** Faz 10 iterações pegando o mutex em cada uma */
void codigo_tarefa_1(void){
	for( int ns=0; ns < 10; ++ns) {
		sleep(1);
		printf("Tarefa 1: passaram %d iteracoes\n", ns+1);	// Chance de escalonamento
 		pthread_mutex_lock(&em); // inicio da sessao critica
		printf("Tarefa 1: estah com o Mutex\n");
		//sleep(1); // Que feiura!!
 		pthread_mutex_unlock(&em); // termino da sessao critica
 	}
}

// Na funcao codigo_tarefa_1, como o sleep ta acima da Sessao Critica, a tarefa 2 provavelmente pegara o lock para ela antes da tarefa1

void codigo_tarefa_1_malvada(void){
	for( int ns=0; ns < 10; ++ns) {
		//sleep(1);
		//printf("Tarefa 1: passaram %d iteracoes\n", ns+1);	// Chance de escalonamento
 		pthread_mutex_lock(&em); // inicio da sessao critica
		printf("Tarefa 1: estah com o Mutex\n");
		sleep(1); // Imagine uma tarefa que exija muito esforco aqui nesse sleep
        /* Como estamos dentro de uma sessao critica, se a thread 2 estiver executando a codigo_tarefa_2, ela tera que esperar este sleep aqui pq ele ta dentro da sessao critica,
         entao esse sleep ta valendo tambem pra tarefa 2, enquanto a tarefa1 ta dormindo, gerando um atraso em cascata. Por isso, as sessoes criticas devem ser rapidas */
 		pthread_mutex_unlock(&em); // termino da sessao critica
 	}
}

/* Com a codigo_tarefa_1_malvada sendo rodada em uma thread1 e o codigo_tarefa_2 sendo rodada em uma thread2, esperamos que quando o sleep da codigo_tarefa_1_malvada ela vai dar um
 unlock no m e dai sim a codigo_tarefa_2 prosseguira dando lock no m, funcionando lisin.. isso ocorre no Windows, mas no Linux isso nao acontece. No Linux, 
 existe a seguinte optimizacao: se uma thread possui um mutex e ela da o unlock, ela nao passa a bola imediatamente para a thread que esta bloqueada no lock do m, ela segue executando,
 entao teoricamente ela bateria na chave da linha 38, retorna para o for, incrementa o ns (supondo que nao tenha passado de 10 ne) e pimba ela entra no lock dnv
 resumindo: a tarefa 1 ja tava com o mutex, deu o unlock, eh verdade que tem outras threads estavam esperando para lockar ele, mas como a tarefa1 ja tava, o linux joga p ela pegar dnv
 a coitada da tarefa 2 continua esperando ate o final do for da tarefa 1 */

// Nos prints que tem na pasta mostra os diferentes resultados, em Windows, as threads sao intercaladas. Enquanto, no Linux, a tarefa2 espera a tarefa1 terminar

/** Faz 10 iterações pegando o mutex em cada uma */
void codigo_tarefa_2(void){
	for( int ns=0; ns < 10; ++ns) {
		sleep(1);
		printf("Tarefa 2: passaram %d iteracoes\n", ns+1);
 		pthread_mutex_lock(&em);
		printf("Tarefa 2: estah com o Mutex\n");
 		pthread_mutex_unlock(&em);
	}
}

/* em livros-texto, muitos autores dizem que o Mutex eh estritamente FIFO. Nesse sentido, apos o unlock da linha 37 a thread2 imediatamente executaria o lock da linha 55,
de modo que as threads fossem intercalando seus resultados, isto eh, operariam em paralelo, como ocorre no Windows. Mas o MUTEX verdadeiramente nao eh estritamento FIFO,
como ocorre no Linux. Neste caso do Linux, dizemos que enquanto a tarefa2 esta esperando a tarefa1 ela esta em postergacao indefinida */

// Observacao importante: Chamada de sistema
/* se a thread1 estivesse exeuctando a funcao codigo_tarefa_1_modificada ao inves das codigo_tarefa_1 acima, ela se intercalaria com a thread2, pq existe uma chamada de sistema
antes do inicio da sessao critica. Desse modo, ao passar pela linha 79 e dar o unlock no m, nao passariamos pela chave 80 e retornariamos ao loop. O SO passaria o m
para o lock da codigo_tarefa_2, pois ele entende que a tarefa1 vai fazer outras coisas (nao esta retomando o mutex imediatamente), ai sim o Mutex comporta como FIFO no Linux
*/
void codigo_tarefa_1_modificada(void){
	for( int ns=0; ns < 10; ++ns) {
		//sleep(1);
		printf("Tarefa 1: passaram %d iteracoes\n", ns+1);	// Chamada de sistema
 		pthread_mutex_lock(&em); // inicio da sessao critica
		printf("Tarefa 1: estah com o Mutex\n");
		//sleep(1); // Que feiura!!
 		pthread_mutex_unlock(&em); // termino da sessao critica
 	}
}

/** Função principal, cria as threads */
int main(void){
	printf("Inicio\n");

	pthread_create(&t1, NULL, (void *) codigo_tarefa_1, NULL);
	pthread_create(&t2, NULL, (void *) codigo_tarefa_2, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("Fim\n");
	return(0);
}




