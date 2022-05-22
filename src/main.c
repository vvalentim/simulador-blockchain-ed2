#include "./lib/estruturas.h"
#include <time.h>
#include <string.h>

int main() {
  clock_t begin = clock();
  MTRand r = seedRand(1234567);

  printf("--- Iniciando simulador da Blockchain ---\n");

  TNoBloco *lista = NULL;
  TNoBloco *ult = NULL;

  int qntd = 10;

  while (qntd) {
    BlocoMinerado *pb = simularMineracao(ult ? ult->ref : NULL, &r);

    if (pb == NULL) {
      printf("Falha em criar/simular mineração do bloco.");
      break;
    }

    ult = inserirBloco(&lista, pb);

    if (ult == NULL) {
      printf("Falha em inserir o bloco %d.\n", pb->bloco.numero);
      break;
    }

    qntd--;
  }

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("Tempo de execução: %f\n", time_spent);
  printf("--- Finalizou a execução ---\n");
  
  return 0;
}