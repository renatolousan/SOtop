#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>

typedef struct cont {
   int saldo;
   char nome[64];
} Conta;

Conta contaGlobal;
int valorDeposito;
char nomeConta[64];

void *atualiza(void *t)
{  long tid;
   int s;
   char n[64];
   tid = (long)t;
   printf("Thread %ld de atualizacao lendo o saldo…\n", tid);
   sleep(2);
   s = contaGlobal.saldo;
   printf("Thread %ld de atualizacao leu o saldo = %d\n",tid, s);
   printf("Thread %ld de atualizacao lendo o nome da conta…\n", tid);
   sleep(2);
   strcpy(n, contaGlobal.nome);
   printf("Thread %ld de atualizacao leu o nome = %s\n",tid, n);
   s += valorDeposito; 
   printf("Thread %ld de atualizacao escrevendo o saldo = %d\n",tid, s);
   sleep(3);
   contaGlobal.saldo = s;
   printf("Thread %ld de atualizacao escrevendo o novo nome = %s\n",tid, nomeConta);
   sleep(3);
   strcpy(contaGlobal.nome, nomeConta);
   printf("Thread %ld - valor do saldo = %d\n", tid, contaGlobal.saldo);
   printf("Thread %ld - nome da conta = %s\n", tid, contaGlobal.nome);
   printf("Thread %ld terminou.\n",tid);
}

 void *leConta(void *t){
	long tid;
	int s;
   char n[64];
	tid = (long) t;
	printf("Thread %ld de consulta lendo o saldo…\n",tid);
	sleep(2);
	s = contaGlobal.saldo;
	printf("Thread %ld de consulta leu o saldo = %d\n",tid, s);
	sleep(3);
	// contaGlobal.saldo = s;
	printf("Thread %ld - valor do saldo = %d\n", tid, contaGlobal.saldo);
   printf("Thread %ld de consulta lendo o nome da conta…\n",tid);
	sleep(2);
	strcpy(n, contaGlobal.nome);
	printf("Thread %ld de consulta leu o nome = %s\n",tid, n);
	sleep(3);
	// strcpy(contaGlobal.nome, n);
	printf("Thread %ld - nome da conta = %s\n", tid, contaGlobal.nome);
	printf("Thread %ld terminou.\n",tid);

} 

int main(int argc, char *argv[])
{
   int threadsLeitoras, threadsEscritoras;

   printf("Digite a quantidade de threads leitoras: \n");
   scanf("%d", &threadsLeitoras);
   printf("Digite a quantidade de threads escritoras: \n");
   scanf("%d", &threadsEscritoras);
   printf("Digite o saldo inicial da conta: \n");
   scanf("%d", &contaGlobal.saldo);
   printf("Digite o valor que deseja depositar (use numeros negativos para simular saque) \n");
   scanf("%d", &valorDeposito);
   printf("Digite um nome para conta \n");
   scanf("%s", contaGlobal.nome);
   printf("Digite um nome para qual deseja atualizar \n");
   scanf("%s", nomeConta);

   pthread_t thread[threadsLeitoras + threadsEscritoras]; // vetor de threads
   pthread_attr_t attr;

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

   for(int i = 0; i<threadsEscritoras; i++) {
      printf("Main: criando thread escritora (deposito) %d\n", i);
	  	  pthread_create(&thread[i], &attr, atualiza, (void *)i);
   }

      for(int i = threadsEscritoras; i<threadsEscritoras+threadsLeitoras; i++) {
      printf("Main: criando thread leitora %d\n", i);
	  pthread_create(&thread[i], &attr, leConta, (void *)i);
   }
   pthread_attr_destroy(&attr);

   sleep(15); // main dorme bastante para acordar após as thread terem terminado e pegar o saldo final 

   printf("Valor final do saldo = %d\n", contaGlobal.saldo);
   printf("Valor final do nome = %s\n", contaGlobal.nome);
   printf("Main: program completed. Exiting.\n");
   pthread_exit(NULL);
   return(0);
}