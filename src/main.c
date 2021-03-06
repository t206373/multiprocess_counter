/* Data de submissao:14/07/2020
 * Nome: Tony Tian Rui Li
 * RA:206373
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>

int primetest (int entry){
	int i;
	if(entry < 2)
		return 0;
	for (i=2; i < entry; i++)
 		if (entry % i == 0)
 			return 0;
 	return 1;
}

int main() {
	pid_t process[999];
	int entry;

	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;

	int *shared, *check;
	shared = (int*) mmap(NULL, sizeof(int)*999, protection, visibility, 0, 0);
	check = (int*) mmap(NULL, sizeof(int)*999, protection, visibility, 0, 0);

	int n, (*p), a = 1, b;
	p = &(shared[0]);
	
	do {	/*ESCREVENDO AS ENTRADAS NA MEMÓRIA COMPARTILHADA.*/
		scanf("%d", &entry);
		shared[a] = entry;
		check[a] = 0;
		a++;
	}
	while ((entry=getchar()) != '\n');
	
	(*p) = 1; /*SETANDO A QUANTIDADE DE PROCESSO INICIAL COMO 1(PROCESSO PAI).*/

	
	for (b = 1; b < a; b++) {	/*LOOP PARA LEITURA DAS ENTRADAS SALVOS NA MEMÓRIA COMPARTILHADA.*/
		while (*p == 3) {	/*LIMITANDO A QUANTIDADE MÁXIMA DE PROCESSOS PARALELOS EM 4.*/
			waitpid(-1, NULL , 0);
			(*p)--;
		}
		if (*p < 4) {	/*GERAÇÃO DE PROCESSOS FILHO PARA VERIFICAR NÚMEROS PRIMOS.*/
			process[b] = fork();
			(*p)++;
			if (process[b] == 0) {
				check[b] = primetest(shared[b]);
				(*p)--;
				exit(0);
			}
		}
	}
	while (*p > 1) {	/*ESPERANDO ENCERRAR TODOS OS PROCESSOS FILHOS PARA MOSTRAR RESPOSTA.*/
		waitpid(-1, NULL, 0);
		(*p)--;
	}
	for (b=1; b < a; b++) {
		n += check[b];
	}
	printf("%d\n", n);
	return 0;
}