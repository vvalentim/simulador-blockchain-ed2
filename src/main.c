#include "./lib/estruturas.h"
#include <time.h>
#include <string.h>
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

void salvarBlocos(TNoBloco *blockchain) {
  FILE *fp = NULL;
  fp = fopen("./bin/blocos.dat", "w+");

  if (fp != NULL) {
    TNoBloco *aux = blockchain;
    
    while (aux != NULL) {
      fwrite(aux->ref, sizeof(BlocoMinerado), 1, fp);

      if (ferror(fp)) {
        printf("Erro ao tentar salvar o conteúdo da blockchain.\n");
        break;
      }

      aux = aux->prox;
    }
  } else {
    printf("Falha ao tentar abrir/criar o arquivo da blockchain.");
  }

  fclose(fp);
}

void carregarBlocos(TNoBloco **blockchain) {
  FILE *fp = NULL;
  fp = fopen("./bin/blocos.dat", "rb");

  if (fp != NULL) {
    long qntd = 0;
    long tamanho = 0;

    fseek(fp, 0, SEEK_END);
    tamanho = ftell(fp);
    rewind(fp);

    if (tamanho % 256 == 0) {
      BlocoMinerado *buffer = NULL;
      unsigned char falha = 0;

      /* Define a quantidade blocos que o arquivo possui, levando em consideração que cada bloco possui 256 bytes. */
      qntd = tamanho / 256;

      while (qntd) {
        buffer = (BlocoMinerado *)malloc(sizeof(BlocoMinerado));

        if (buffer == NULL) {
          falha = 1;
          printf("Falha ao tentar alocar a memória necessária para um dos blocos.\n");
          break;
        }

        if (!fread(buffer, 256, 1, fp)) {
          falha = 1;
          printf("Falha na leitura de um dos blocos do arquivos (byte %lu).\n", ftell(fp));
          break;
        }

        if (inserirBloco(blockchain, buffer) == NULL) {
          falha = 1;
          printf("Falha na inserção de um dos blocos na blockchain.\n");
          break;
        }

        qntd--;
      }

      if (falha) {
        free(buffer);
        buffer = NULL;
      }
    } else {
      printf("Falha na leitura do arquivo, tamanho do arquivo não corresponde a uma quantidade válida de blocos.\n");
    }

    fclose(fp);
  }
}

void listagemBlocos(TNoBloco *blockchain) {
  while (blockchain != NULL) {
    printf(
      "%d\t%d\t", 
      blockchain->ref->bloco.numero,
      blockchain->ref->bloco.nonce
    );

    printSHA256(blockchain->ref->bloco.hashAnterior);
    printf("\t");
    printSHA256(blockchain->ref->hash);
    printf("\n");

    blockchain = blockchain->prox;
  }
}

TNoBloco *buscaUltimoBloco(TNoBloco *blockchain) {
  while (blockchain->prox != NULL) {
    blockchain = blockchain->prox;
  }

  return blockchain;
}

// Blocos: 1021, 1022, 1095, 1099 possuem 5 zeros iniciais!!

int main() {
  MTRand rng = seedRand(1234567);
  TNoBloco *blockchain = NULL;

  carregarBlocos(&blockchain);

  int continuar = 1;
  TNoBloco *ult = buscaUltimoBloco(blockchain);

  while (continuar) {
    printf("--------------------\n");
    continuar = mostrarMenu();

    switch (continuar) {
      case 1: minerarBlocos(&blockchain, &ult, &rng); break;
    }
  }

  salvarBlocos(blockchain);
  listagemBlocos(blockchain);
  
  return 0;
}