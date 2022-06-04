#include "./lib/estruturas.h"
#include <time.h>
#include <unistd.h>

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

void carregarUltimoBloco(BlocoMinerado **ultimoBloco, MTRand *rng) {
  FILE *fp_blocos = fopen(__PATH_BLOCOS__, "rb");

  if (fp_blocos != NULL) {
    long t = 0;
    
    /* Verifica se o arquivo possui conteúdo. */
    fseek(fp_blocos, 0, SEEK_END);
    t = ftell(fp_blocos);

    if (t >= 256) {
      FILE *fp_rand = fopen(__PATH_SEQ_RAND__, "rb");

      if (fp_rand != NULL) {
        unsigned long it = 0;

        fread(&it, sizeof(it), 1, fp_rand);
        fclose(fp_rand);

        while (it > __SEQ_RAND__) {
          randChar(rng, 0, 1);
        }
      }

      BlocoMinerado *pb = (BlocoMinerado *)malloc(sizeof(BlocoMinerado));

      if (pb != NULL) {
        fseek(fp_blocos, -256, SEEK_END);
        fread(pb, sizeof(BlocoMinerado), 1, fp_blocos);
        *ultimoBloco = pb;
      }
    }

    fclose(fp_blocos);
  }
}

void salvarBloco(BlocoMinerado *bloco) {
  FILE *fp_blocos = fp_blocos = fopen(__PATH_BLOCOS__, "ab");
  FILE *fp_rand = fp_rand = fopen(__PATH_SEQ_RAND__, "wb");

  if (fp_rand == NULL || fp_blocos == NULL) {
    printf("Erro ao tentar salvar o conteúdo dos blocos.\n");
    return;
  }

  fwrite(&__SEQ_RAND__, sizeof(__SEQ_RAND__), 1, fp_rand);

  if (ferror(fp_rand)) {
    printf("Erro ao tentar salvar o número da sequência.\n");
    return;
  }

  fwrite(bloco, sizeof(BlocoMinerado), 1, fp_blocos);
  
  if (ferror(fp_blocos)) {
    printf("Erro ao tentar salvar o conteúdo do bloco.\n");
    return;
  }

  fclose(fp_rand);
  fclose(fp_blocos);
}

void minerarBlocos(BlocoMinerado **ultimo, MTRand *rng) {
  int qntdMinerar = 0;
  printf("Informe a quantidade de blocos: ");

  while (qntdMinerar <= 0) {
    if ((qntdMinerar = getIntInput("Quantidade invalida: ")) <= 0) {
      printf("Quantidade de blocos deve ser um inteiro positivo: ");
    }
  }

  clock_t begin;
  clock_t end;
  double time_spent;

  printf("------------\nIniciando mineracao...\n------------\n");

  int minerados = 0;

  while (minerados < qntdMinerar) {
    begin = clock();
    BlocoMinerado *pb = simularMineracao(*ultimo ? *ultimo : NULL, rng);

    if (pb == NULL) {
      printf("Falha em criar/simular mineração do bloco.\n");
      break;
    }

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Finalizou mineracao do bloco: %f\n", time_spent);
    printf("------------\n");
    
    /* Salva o bloco e define o recém minerado como o último bloco. */
    salvarBloco(pb);
    free(*ultimo);
    *ultimo = pb;

    minerados++;
  }
}

void listarTransacoes(BlocoMinerado *pb) {
  for (int t = 0; t < 61; t++) {
    /* Calcula o próximo indice no vetor de dados */
    int i = t * 3;
    printf("%u\t%u\t%u\n", pb->bloco.data[i], pb->bloco.data[i + 1], pb->bloco.data[i + 2]);
  }
}

int main() {
  MTRand rng = seedRand(1234567);
  BlocoMinerado *ultimo = NULL;

  carregarUltimoBloco(&ultimo, &rng);

  if (ultimo != NULL) {
    printf("--------------------\nÚltimo bloco minerado: ");
    printf("%u %u ", ultimo->bloco.numero, ultimo->bloco.nonce);
    printSHA256(ultimo->hash);
    printf("\n");
    // listarTransacoes(ultimo);
  }

  int continuar = 1;

  while (continuar) {
    printf("--------------------\n");
    continuar = mostrarMenu();

    switch (continuar) {
      case 1: minerarBlocos(&ultimo, &rng); break;
    }
  }
  
  return 0;
}