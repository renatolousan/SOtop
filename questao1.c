#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

// Programa simula o deposito e consulta de saldo concorrente de uma conta com 100 reais inicialmente
// A funçao de depósito deposita 15 reais!!
// a saída "esperada" é o valor final de 115 reais

int saldo;

void *deposito(void *t) // deposito de 15 reais na conta
{  long tid;
   int s;
   tid = (long)t;
   printf("Thread %ld de deposito lendo o saldo…\n",tid);
   sleep(2);
   s = saldo;
   printf("Thread %ld de deposito leu o saldo = %d\n",tid, s);
   s += 15;
   printf("Thread %ld de deposito escrevendo o saldo = %d\n",tid, s);
   sleep(3);
   saldo = s;
   printf("Thread %ld - valor do saldo = %d\n", tid, saldo);
   printf("Thread %ld done.\n",tid);
}

 void *leSaldo(void *t){
	long tid;
	int s;
	tid = (long) t;
	printf("Thread %ld de consulta lendo o saldo…\n",tid);
	sleep(2);
	s = saldo;
	printf("Thread %ld de consulta leu o saldo = %d\n",tid, s);
	sleep(3);
	//saldo = s;
	printf("Thread %ld - valor do saldo = %d\n", tid, saldo);
	printf("Thread %ld terminou.\n",tid);

} 

int main(int argc, char *argv[])
{
   int threadsLeitoras;
   int threadsEscritoras;
   printf("Digite a quantidade de threads leitoras: \n");
   scanf("%d", &threadsLeitoras);
   printf("Digite a quantidade de threads escritoras: \n");
   scanf("%d", &threadsEscritoras);
   pthread_t thread[threadsLeitoras + threadsEscritoras]; // vetor de threads
   pthread_attr_t attr;

   saldo=100;

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

   for(int i = 0; i<threadsEscritoras; i++) {
      printf("Main: criando thread escritora (deposito) %d\n", i);
	  	  pthread_create(&thread[i], &attr, deposito, (void *)i);
   }

      for(int i = threadsEscritoras; i<threadsEscritoras+threadsLeitoras; i++) {
      printf("Main: criando thread leitora %d\n", i);
	  pthread_create(&thread[i], &attr, leSaldo, (void *)i);
   }
   pthread_attr_destroy(&attr);

   sleep(15); // main dorme bastante para acordar após as thread terem terminado e pegar o saldo final 

   printf("Valor final do saldo = %d\n", saldo);
   printf("Main: program completed. Exiting.\n");
   pthread_exit(NULL);
   return(0);
}