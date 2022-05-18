#include "./lib/estruturas.h"
#include <time.h>

int main() {
  clock_t begin = clock();

  MTRand r = seedRand(1234567);

  printf("--- Iniciando simulador da Blockchain ---\n");

  simularMineracao(NULL, &r);

  clock_t end = clock();

  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("Tempo de execução: %f\n", time_spent);
  printf("--- Finalizou a execução ---\n");
  
  return 0;
}