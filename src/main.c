#include "./lib/estruturas.h"
#include <time.h>
#include <string.h>

int mostrarMenu() {
  printf("0. Encerrar programa\n");
  printf("1. Minerar blocos\n");
  printf("2. Buscar hash de um bloco\n");
  printf("3. Buscar quantidade de bitcoins de um endereco\n");
  printf("4. Buscar endereco com maior quantidade de bitcoins\n");
  printf("5. Listar enderecos\n\n");
  printf("Informe o numero de acordo com a opcao desejada: ");

  int opt = -1;

  while (opt < 0) {
    opt = getIntInput("Opcao invalida: ");
  }

  return opt;
}

void minerarBlocos(TNoBloco **blockchain, TNoBloco **ult, MTRand *rng) {
  int qntdMinerar = 0;
  printf("Informe a quantidade de blocos: ");

  while (qntdMinerar <= 0) {
    if ((qntdMinerar = getIntInput("Quantidade invalida: ")) <= 0) {
      printf("Quantidade de blocos nao pode ser negativa: ");
    }
  }

  clock_t begin = clock();

  printf("------------\nIniciando mineracao...\n------------\n");

  int minerados = 0;

  while (minerados < qntdMinerar) {
    BlocoMinerado *pb = simularMineracao(*ult ? (*ult)->ref : NULL, rng);

    printf("------------\n");

    if (pb == NULL) {
      printf("Falha em criar/simular mineração do bloco.");
      break;
    }

    *ult = inserirBloco(blockchain, pb);

    if (*ult == NULL) {
      printf("Falha em inserir o bloco %d.\n", pb->bloco.numero);
      break;
    }

    minerados++;
  }

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Finalizou mineracao: %f\n", time_spent);
}

int main() {
  MTRand rng = seedRand(1234567);
  TNoBloco *blockchain = NULL;
  TNoBloco *ult = NULL;

  int continuar = 1;

  while (continuar) {
    printf("--------------------\n");
    continuar = mostrarMenu();

    switch (continuar) {
      case 1: minerarBlocos(&blockchain, &ult, &rng); break;
    }
  }

  return 0;
}